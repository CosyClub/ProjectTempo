#include <client/component/ComponentKeyInput.hpp>
#include <client/component/ComponentRenderButtonGroup.hpp>
#include <client/component/ComponentRenderSpikes.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>
#include <client/misc/Color.hpp>
#include <client/misc/Lighting.hpp>
#include <client/misc/RGBtoHSV.hpp>
#include <client/network/client.hpp>
#include <client/system/SystemAttack.hpp>
#include <client/system/SystemButtonRenderer.hpp>
#include <client/system/SystemCombo.hpp>
#include <client/system/SystemEntity.hpp>
#include <client/system/SystemGraphicsCreation.hpp>
#include <client/system/SystemLighting.hpp>
#include <client/system/SystemMovement.hpp>
#include <client/system/SystemParseKeyInput.hpp>
#include <client/system/SystemRenderGUI.hpp>
#include <client/system/SystemRenderHealing.hpp>
#include <client/system/SystemRenderHealthBars.hpp>
#include <client/system/SystemRenderSceneNode.hpp>
#include <client/system/SystemRenderAttack.hpp>
#include <client/system/SystemRenderSpikes.hpp>
#include <client/system/SystemStageRenderer.hpp>
#include <client/system/SystemUpdateKeyInput.hpp>
#include <client/system/SystemTranslationAnimation.hpp>
#include <client/misc/Camera.hpp>

#include <tempo/component/ComponentButtonGroup.hpp>
#include <tempo/component/ComponentParty.hpp>
#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentSpikes.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentAttack.hpp>
#include <tempo/network/ID.hpp>
#include <tempo/song.hpp>
#include <tempo/system/SystemHealth.hpp>
#include <tempo/system/SystemTrigger.hpp>
#include <tempo/time.hpp>

#include <anax/Entity.hpp>
#include <anax/World.hpp>

#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include <irrlicht.h>
#include <vector3d.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <chrono>
#include <cstdio>
#include <functional>  // maybe not needed (std::ref when calling listenForServerUpdates())
#include <iostream>
#include <thread>

#include "tempo/constants.hpp"

#include <tempo/component/ComponentStage.hpp>
namespace client
{
	 // TLDR: enforce the game logic on client side and server side
	 class SystemLessJank
	 	: public anax::System<
	 	anax::Requires<
	 	tempo::ComponentStageTranslation,
	 	tempo::ComponentStagePosition,
	 	tempo::ComponentStage>
	 	>
	 {
	 public:
		typedef std::unordered_map<glm::ivec2,
		                           bool,
		                           vec2eq,
		                           vec2eq,
		                           std::allocator<std::pair<const glm::ivec2, bool>>> collMap;
		std::map<unsigned long int, glm::ivec2> posMap;

		collMap collisionMap;
	 	void lessJank(const glm::ivec2 playerpos) {
	 		// uncomment this for more jank:
	 		//return;

	 		auto& entities = getEntities();
			collisionMap.clear();

			// Update collision map
	 		for (auto& entity : entities) {
				auto& sp = entity.getComponent<tempo::ComponentStagePosition>();
	 			glm::ivec2 origin = sp.getOrigin();
				if (!sp.isPhased)
				if (entity.hasComponent<tempo::ComponentHealth>())
				if (entity.getComponent<tempo::ComponentHealth>().current_health > 0)
				{
					collisionMap[origin] = true;
				}
			}

			// Check if we need to clear movements
	 		for (auto& entity : entities) {
	 			glm::ivec2 origin = entity.getComponent<tempo::ComponentStagePosition>().getOrigin();

				tempo::ComponentStage &stage = entity.getComponent<tempo::ComponentStage>();

	 			tempo::ComponentStageTranslation& trans = entity.getComponent<tempo::ComponentStageTranslation>();

				if (origin.x < playerpos.x - 24 || origin.x > playerpos.x + 7 ||
				    origin.y < playerpos.y - 33 || origin.y > playerpos.y + 33)
				{
					continue;
				}

				if (trans.delta == glm::ivec2(0, 0)) continue;

	 			glm::ivec2 dest = origin + trans.delta;

				bool can_move = true;
				if (collisionMap.find(dest) == collisionMap.end())
					collisionMap[dest] = false;
				can_move &= !collisionMap[dest];
	 			if (!stage.existstTile(dest) || abs(stage.getHeight(origin) - stage.getHeight(dest)) >= 5 || stage.getHeight(dest) <= -3 || !can_move) {
	 				// consume the moment before the server rejects you
	 				// currently combos aren't server protected, so maybe this should move into lib-tempo?
	 				// this produces a lovely jumping against the wall animation!
	 				trans.delta = glm::ivec2(0, 0);
	 			}
	 		}
	 	}
	 };

}  // namespace client

void sync_time(tempo::Clock& clock)
{
	sf::Int64 offset = tempo::timeSyncClient(&clock);
	clock.set_time(clock.get_time() + sf::microseconds(offset));
}

anax::Entity createEntityStage(anax::World& world)
{
	printf("Creating entity stage\n");
	anax::Entity entity_stage = world.createEntity();
	entity_stage.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	entity_stage.activate();
	world.refresh();

	return entity_stage;
}

void the_end(uint32_t             code,
             std::string          message,
             anax::World         &world,
             std::atomic<bool>   &running,
             std::thread         &listener,
             irr::IrrlichtDevice *device)
{
	running.store(false);

	// Close server listener and destroy the game
	listener.join();
	world.clear();
	device->drop();

	std::cout << message << std::endl;
	exit(code);
}

// ./RaveCave [ServerIP] [PlayerParty] HUD DELTA BPM
int main(int argc, const char** argv)
{
	unsigned int delta = DELTA;
	unsigned int bpm = BPM;
	
	if (argc >= 5) {
		delta = atoi(argv[4]);
	}
	if (argc >= 6) {
		bpm = atoi(argv[5]);
	}

	// Clock
	tempo::Clock clock = tempo::Clock(sf::microseconds(sf::Int64(TIME(bpm))),sf::milliseconds(delta));

	std::cout << delta << std::endl << bpm << std::endl;

	bool enable_hud = false;
	if (argc >= 4) {
		std::string HUD = argv[3];
		enable_hud = (HUD == "HUD" || HUD == "hud" || HUD == "42");
	}

	irr::IrrlichtDevice *device = irr::createDevice(
	irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(1920, 1080), 16, enable_hud, false, false);

	if (!device) {
		printf("Failed to create Irrlicht Device\n");
		return 1;
	}
	device->setWindowCaption(L"RaveCave");
	irr::video::IVideoDriver* driver = device->getVideoDriver();
	irr::scene::ISceneManager* smgr = device->getSceneManager();
	irr::gui::IGUIEnvironment* gui_env = device->getGUIEnvironment();

	irr::video::ITexture* splash_texture[4];
	splash_texture[0] = driver->getTexture("resources/materials/textures/splash-full.png");
	splash_texture[1] = driver->getTexture("resources/materials/textures/splash-minimal.png");
	splash_texture[2] = driver->getTexture("resources/materials/textures/splash-loading.png");
	splash_texture[3] = driver->getTexture("resources/materials/textures/splash-loading-alt.png");

	// Put up the intial splash image
	irr::gui::IGUIImage* splashScreen;
	if (enable_hud) {
		std::string HUD = argv[3];
		int loading_texture = std::rand() % 1000 == 42 || HUD == "42" ? 3 : 2;
		splashScreen = device->getGUIEnvironment()->addImage(
		                                  splash_texture[loading_texture],
		                                  irr::core::position2d<irr::s32>(0,0), true);
		driver->beginScene(true, true);
		smgr->drawAll();
		gui_env->drawAll();
		driver->endScene();
	}

	/////////////////////////////////////////////////
	// Setup ECS
	anax::World world;

	tempo::SystemHealth            system_health;
	tempo::SystemTrigger           system_trigger(world);
	client::SystemAttack           system_attack;
	client::SystemButtonRenderer   system_button_renderer;
	client::SystemCombo            system_combo;
	client::SystemEntity           system_entity;
	client::SystemGraphicsCreation system_gc;
	client::SystemLighting         system_lighting;
	client::SystemMovement         system_movement;
	client::SystemStageRenderer    system_stage_renderer;
	client::SystemParseKeyInput    system_parse_key_input;
	client::SystemRenderGUI        system_render_gui;
	client::SystemRenderHealing    system_render_healing(driver, smgr);
	client::SystemRenderHealthBars system_render_health_bars;
	client::SystemRenderSceneNode  system_render_scene_node;
	client::SystemRenderAttack     system_render_attack;
	client::SystemRenderSpikes     system_render_spikes;
	client::SystemUpdateKeyInput   system_update_key_input;
	client::SystemTranslationAnimation system_translation_animation(&world, device, clock);
	client::SystemLessJank         system_less_jank;

	// Add Systems
	world.addSystem(system_attack);
	world.addSystem(system_entity);
	world.addSystem(system_combo);
	world.addSystem(system_health);
	world.addSystem(system_gc);
	world.addSystem(system_lighting);
	world.addSystem(system_trigger);
	world.addSystem(system_button_renderer);
	world.addSystem(system_stage_renderer);
	world.addSystem(system_render_healing);
	world.addSystem(system_render_health_bars);
	world.addSystem(system_render_attack);
	world.addSystem(system_render_scene_node);
	world.addSystem(system_render_spikes);
	world.addSystem(system_update_key_input);
	world.addSystem(system_parse_key_input);
	world.addSystem(system_movement);
	world.addSystem(system_translation_animation);
	world.addSystem(system_less_jank);
	world.refresh();

	createEntityStage(world);

	// Initialise Systems
	system_update_key_input.setup(device);
	system_render_scene_node.setup(smgr, driver);
	system_render_gui.init(device, driver, enable_hud);

	// Warning: Must be after system_render_scene_node.setup(smgr);
	system_render_health_bars.setup(smgr);

	// Set up remote address, local ports and remote handshake port
	// Note, IF statement is to change ports for local development, bit
	// hacky and should be removed in due course!
	tempo::addr_r = "127.0.0.1";
	if (argc >= 2)
		tempo::addr_r = argv[1];
	if (tempo::addr_r == "127.0.0.1") {
		std::srand(time(NULL));
		int d          = std::rand() % 1000;
		tempo::port_ci = DEFAULT_PORT_IN + 1000 + d;
		tempo::port_co = DEFAULT_PORT_OUT + 1000 + d;
	} else {
		tempo::port_ci = DEFAULT_PORT_IN;
		tempo::port_co = DEFAULT_PORT_OUT;
	}

	// Bind sockets
	// Note: other server ports aquired dynamically on handshake
	tempo::port_si = DEFAULT_PORT_IN;
	tempo::bindSocket('i', tempo::port_ci);
	tempo::bindSocket('o', tempo::port_co);

	// Start Listener Thread to catch server updates after connecting
	std::atomic<bool> running(true);
	std::thread listener(tempo::listenForServerUpdates, std::ref(running));
	// Hack to allow printouts to line up a bit nicer :)
	std::this_thread::sleep_for(std::chrono::milliseconds(5));


	// Connect to server and sync level/time
	if (!tempo::connectToAndSyncWithServer(world))
		the_end(1, "Failed to connect/sync with server.", world, running, listener, device);
	sync_time(clock);

	// Display HUD
	if (enable_hud) {
		bool waiting = true;
		int  flash = 0;
		sf::Clock splash_timer;
		splash_timer.restart();
		while (device->run() && waiting) {
			std::vector<client::KeyEvent> keys = system_update_key_input.getKeys();
			for (unsigned int i = 0; i < keys.size(); i++) {
				if (keys[i].press) waiting = false;
			}

			if (clock.passed_beat()) tempo::sendHeatbeat();

			if (splash_timer.getElapsedTime().asSeconds() > 1.0f) {
				splash_timer.restart();
				splashScreen->setImage(splash_texture[flash++ % 2]);
			}

			driver->beginScene(true, true);
			smgr->drawAll();
			gui_env->drawAll();
			driver->endScene();
		}
	}
	device->getGUIEnvironment()->clear();

	// Join the game as a player
	int party_number = 0;
	if (argc >= 3)
		party_number = atoi(argv[2]);

	tempo::ClientRole     role     = tempo::ClientRole::PLAYER;
	tempo::ClientRoleData roleData = {"Bilbo Baggins", party_number};
	if (!tempo::joinGame(role, roleData, world)) {
		the_end(1, "Failed to join game.", world, running, listener, device);
	}

	// Sort out graphics after handshake
	system_stage_renderer.setup(smgr, driver);
	system_gc.addEntities(driver, smgr, world);
	system_render_scene_node.setup(smgr, driver);
	system_render_health_bars.setup(smgr);
	system_render_gui.setup(device, driver, enable_hud);
	system_button_renderer.setup(smgr, driver);
	system_render_spikes.setup(smgr, driver);
	system_lighting.setup(smgr, driver);
	system_trigger.syncFloorWithButtons();

	// Player
	// TODO: use better way to find out player, for now this is a search
	anax::Entity entity_player;
	for (auto& entity : world.getEntities()) {
		if (entity.hasComponent<tempo::ComponentPlayerLocal>()) {
			entity_player = entity;
			break;
		}
	}
	entity_player.addComponent<client::ComponentKeyInput>();
	entity_player.activate();

	auto& combo             = entity_player.getComponent<tempo::ComponentCombo>().comboCounter;
	auto& comp_health       = entity_player.getComponent<tempo::ComponentHealth>();
	auto& comp_player_input = entity_player.getComponent<client::ComponentKeyInput>();

	glm::ivec2 startingPos = entity_player.getComponent<tempo::ComponentStagePosition>().getOrigin();

	int emptySpace = 40;
	int fheight = 69 + emptySpace;
	int feeder_areas = 10;

	for (int i=0; i < feeder_areas; i++) {
		client::createLasers(smgr, driver, { {40 + (i*fheight),12}, {40 + (i*fheight),52}, {40 + (i*fheight),92} }, startingPos);
		client::createDiscoBalls(smgr, driver, { {40 + (i*fheight),6} }, startingPos);
	}

	/////////////////////////////////////////////////
	// Main loop
	int frame_counter = 0;
	sf::Clock fps_timer;
	sf::Clock frame_clock = sf::Clock();
	sf::Clock update_floor_clock = sf::Clock();
	update_floor_clock.restart();

	srand(clock.get_time().asMicroseconds());

	client::ComponentRenderSceneNode& sn = entity_player.getComponent<client::ComponentRenderSceneNode>();
	irr::scene::ICameraSceneNode *camera_node = new irr::scene::Camera(
		sn.node,
		smgr,
		-1,
		irr::core::vector3df(7, 7, 0),
		irr::core::vector3df(0, 0, 0));

	smgr->setActiveCamera(camera_node);
	float dt;
	sf::Int64 synced_tick = clock.get_time().asMicroseconds() / sf::Int64(TIME(bpm));
	client::next_palette(synced_tick % client::palettes.size());

	printf("Entering main loop\n");
	while (device->run()) {

		// Work out a frame delta time.
		// const irr::u32 now = device->getTimer()->getTime();
		dt = frame_clock.restart().asSeconds();
		/// frameDeltaTime = (f32)(now - then)/1000.f; // Time in seconds

		glm::ivec2 playerpos;


		// std::clock_t    start;
		//
		// start = std::clock();
		////////////////
		// Events all the time
		{
			// Check for new entities from server
			bool ent_created = system_entity.creationCheck(world);
			system_entity.deletionCheck(world);

			// Initialise Graphics for new entities
			if(ent_created) {
				system_gc.addEntities(driver, smgr, world);
			}
			//std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

			system_render_scene_node.setup(smgr, driver);
			system_render_health_bars.setup(smgr);
			system_button_renderer.setup(smgr, driver);

			// Receive updates from the server
			system_movement.processIntents(world);
			system_movement.processCorrections(world);
			system_attack.processServerResponses(world);
			system_combo.checkForUpdates(world);
			system_attack.processServerResponses(world);
			playerpos = entity_player.getComponent<tempo::ComponentStagePosition>().getOrigin();

			// Deal with local input
			system_update_key_input.clear();
			system_update_key_input.addKeys();
			system_parse_key_input.parseInput(clock, device);

			// Deprecated/To-be-worked-on
			system_health.check_health();
			system_health.client_receiveHealth(world);

			system_less_jank.lessJank(playerpos);
			// Update animations from translations received from server
			system_translation_animation.updateAnimations();

			// Graphics updates
			system_render_attack.update(system_stage_renderer,
			                            client::curr_pallette.attack);
			system_render_scene_node.update(playerpos);
			system_render_health_bars.update(playerpos);
			system_render_healing.update();

			// TODO: Make a system for updating camera position
			//camera_node->setPosition(sn.node->getAbsolutePosition() + irr::core::vector3df(7, 9, 0));
			//camera_node->setTarget(sn.node->getAbsolutePosition());
		}

		////////////////
		// Events at "Delta Start"
		if (clock.passed_delta_start()) {
			// std::cout << "Start" << std::endl;
			tempo::sendHeatbeat();
		}

		////////////////
		// Events at "Beat Passed"
		glm::vec4 c1;
		glm::vec4 c2;
		if (clock.passed_beat()) {
			// For christ sake, leave this code alone
			synced_tick = clock.get_time().asMicroseconds() / sf::Int64(TIME(bpm));
			if (synced_tick % 20 == 0)
				std::cout << "SYNCED_TICK (" << synced_tick << ") "
				          << clock.get_time().asMilliseconds()
				          << "+++++++++++++++" << std::endl;
			// End of leave this code alone

			// click.play();

			system_trigger.updateButtons(world);
			system_button_renderer.updateButtons(driver);

			system_render_healing.endBeat();
			system_render_spikes.updateSpikes(driver, playerpos);

			system_translation_animation.endBeat();

			client::next_palette((synced_tick * 7) % client::palettes.size());
			system_lighting.update(client::curr_pallette.light);
		}

		////////////////
		// Events at "Delta End"
		if (clock.passed_delta_end()) {
			system_combo.advanceBeat();
		}

		system_stage_renderer.Update(smgr, driver, playerpos,
		                             client::curr_pallette.floor1,
		                             combo < 20 ? irr::video::SColor(255, 50, 50, 50)
		                                        : client::curr_pallette.floor2,
		                             synced_tick, dt);

		driver->beginScene(true, true);
		smgr->drawAll();
		gui_env->drawAll();

		system_render_gui.update(driver, gui_env, clock, combo,
		                         comp_health, comp_player_input,
		                         synced_tick % client::palettes.size(),
		                         enable_hud);
		driver->endScene();

		++frame_counter;
		if (fps_timer.getElapsedTime().asSeconds() > 1.0f) {
			float seconds = fps_timer.getElapsedTime().asSeconds();
			std::cout << "FPS: " << (int)(frame_counter / seconds) << std::endl;
			fps_timer.restart();
			frame_counter = 0;
		}

	}  // main loop

	tempo::disconnectFromServer(entity_player);
	the_end(0, "Goodbye!\r\n", world, running, listener, device);
}
