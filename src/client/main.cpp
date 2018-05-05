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

#define BPM 138              // Beats per minutes
#define DELTA 100            // Delta around a beat a player can hit (millisecs)
#define TIME 60000000 / BPM  // Time between beats (microsecs)

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

	 		for (auto& entity : entities) {
	 			glm::ivec2 origin = entity.getComponent<tempo::ComponentStagePosition>().getOrigin();

				if(posMap.find(entity.getId().index) == posMap.end())
				{
					posMap[entity.getId().index] = origin;
				}
				if(posMap[entity.getId().index] == origin)
				{
					if (!entity.getComponent<tempo::ComponentStagePosition>().isPhased)
					{
						collisionMap[posMap[entity.getId().index]] = false;
						posMap[entity.getId().index] = origin;
						collisionMap[posMap[entity.getId().index]] = true;
					}
				}

				tempo::ComponentStage &stage = entity.getComponent<tempo::ComponentStage>();

				auto &positions = entity.getComponent<tempo::ComponentStagePosition>().occupied;
				if (entity.hasComponent<tempo::ComponentHealth>())
				{
					if (entity.getComponent<tempo::ComponentHealth>().current_health <= 0)
					{
						for (auto &position : positions) {
							if (!entity.getComponent<tempo::ComponentStagePosition>().isPhased)
							{
								collisionMap[position] = false;
							}
						}
						continue;
					}
				}
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
	 			if (!stage.existstTile(dest) || stage.getHeight(dest) >= 5 || !can_move) {
	 				// consume the moment before the server rejects you
	 				// currently combos aren't server protected, so maybe this should move into lib-tempo?
	 				// this produces a lovely jumping against the wall animation!
	 				trans.delta = glm::ivec2(0, 0);
	 				//if (entity.hasComponent<tempo::ComponentCombo>()) {
	 				//	// what the heck this is a jank class anyway
	 				//	tempo::ComponentCombo& combo = entity.getComponent<tempo::ComponentCombo>();
	 				//	combo.advanceBeat();
	 				//}
	 			}
				else
				{
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

	return entity_stage;
}

// anax::Entity createEntityPlayer(anax::World& world) {
// 	printf("Creating entity player\n");
// 	anax::Entity entity_player = world.createEntity();
// 	entity_player.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
// 	entity_player.addComponent<tempo::ComponentStagePosition>(glm::ivec2(5,
// 5));
// 	entity_player.addComponent<client::ComponentRenderSceneNode>(nullptr);
// 	entity_player.addComponent<client::ComponentKeyInput>();
// 	entity_player.activate();

// 	return entity_player;
// }

int main(int argc, const char** argv)
{
	sf::SoundBuffer clickbuf;
	clickbuf.loadFromFile("resources/sound/tick.ogg");
	sf::Sound click;
	click.setBuffer(clickbuf);

	// Clock
	tempo::Clock clock = tempo::Clock(sf::microseconds(TIME), sf::milliseconds(DELTA));

	KeyInput receiver;

	// This makes it full-screen
	// irr::IrrlichtDevice *nulldevice = irr::createDevice(irr::video::EDT_NULL);
	// irr::core::dimension2d<irr::u32> deskres =
	// nulldevice->getVideoModeList()->getDesktopResolution();
	// nulldevice -> drop();
	// irr::IrrlichtDevice *device = irr::createDevice(
	//   irr::video::EDT_OPENGL, deskres, 16, true, false, false);


	bool enable_hud = false;
	if (argc == 4) {
		std::string HUD = argv[3];
		enable_hud = (HUD == "HUD" || HUD == "hud");
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
	// Debug
	//smgr->setAmbientLight(irr::video::SColorf(0.3f, 0.3f, 0.3f));

	/////////////////////////////////////////////////
	// Setup ECS
	anax::World world;
	// tempo::SystemRender           system_render(app);


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
	client::SystemRenderSpikes  	 system_render_spikes;

	client::SystemUpdateKeyInput   system_update_key_input;
	client::SystemTranslationAnimation system_translation_animation(&world, device, clock);
	client::SystemLessJank system_less_jank;

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

	createEntityStage(world);
	world.refresh();

	// Initialise Systems
	system_update_key_input.setup(device);
	system_stage_renderer.setup(smgr, driver);
	system_render_scene_node.setup(smgr, driver);
	system_render_gui.init(device, driver, enable_hud);

	// must be after system_render_scene_node.setup(smgr);
	system_render_health_bars.setup(smgr);


	if (enable_hud) {

		irr::video::ITexture* splash_texture[2];
		splash_texture[0] = driver->getTexture("resources/materials/textures/splash-full.png");
		splash_texture[1] = driver->getTexture("resources/materials/textures/splash-minimal.png");

		irr::gui::IGUIImage* splashScreen = device->getGUIEnvironment()->addImage(
		                                  splash_texture[1],
		                                  irr::core::position2d<irr::s32>(0,0), true);
		bool waiting = true;
		int i = 0;
		sf::Clock splash_timer;
		splash_timer.restart();
		while (device->run() && waiting) {
			std::vector<client::KeyEvent> keys = system_update_key_input.getKeys();
			for (unsigned int i = 0; i < keys.size(); i++) {
				if (keys[i].press) waiting = false;
			}

			if(splash_timer.getElapsedTime().asSeconds() > 1.0f) {
				splash_timer.restart();
				i = (i+1) % 2;
				splashScreen->setImage(splash_texture[i]);
			}
			driver->beginScene(true, true);
			smgr->drawAll();
			gui_env->drawAll();
			driver->endScene();
		}

		device->getGUIEnvironment()->clear();
	}
	system_render_gui.setup(device, driver, enable_hud);

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
	// Other server ports aquired dynamically on handshake
	tempo::port_si = DEFAULT_PORT_IN;

	// Bind sockets
	tempo::bindSocket('i', tempo::port_ci);
	tempo::bindSocket('o', tempo::port_co);

	// Start Listener Thread to catch server updates after connecting
	std::atomic<bool> running(true);
	std::thread listener(tempo::listenForServerUpdates, std::ref(running));
	// Hack to allow printouts to line up a bit nicer :)
	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	tempo::ClientRole     role     = tempo::ClientRole::PLAYER;

	int party_number = 0;
	if(argc >= 3 ){
		party_number = atoi(argv[2]);
	}

	tempo::ClientRoleData roleData = {"Bilbo Baggins", party_number};

	// Connect to server and handshake information
	if (!tempo::connectToAndSyncWithServer(role, roleData, world)) {
		std::cout << "Failed to connect/join server." << std::endl;
		running.store(false);
		listener.join();
		world.clear();
		device->drop();
		return 1;
	}

	// Sort out graphics after handshake
	system_gc.addEntities(driver, smgr, world);
	system_render_scene_node.setup(smgr, driver);
	system_render_health_bars.setup(smgr);
	system_button_renderer.setup(smgr, driver);
	system_render_spikes.setup(smgr, driver);
	system_lighting.setup(smgr, driver);

	// Start and Sync Song
	sync_time(clock);
	// long offset = 0;

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

	auto& combo = entity_player.getComponent<tempo::ComponentCombo>().comboCounter;
	auto& comp_health = entity_player.getComponent<tempo::ComponentHealth>();

	glm::ivec2 startingPos = entity_player.getComponent<tempo::ComponentStagePosition>().getOrigin();

	int emptySpace = 40;

	int fheight = 69 + emptySpace;

	int feeder_areas = 10;

	for (int i=0; i < feeder_areas; i++){

	client::createLasers(smgr, driver, { {40 + (i*fheight),12}, {40 + (i*fheight),52}, {40 + (i*fheight),92} }, startingPos);

	client::createDiscoBalls(smgr, driver, { {40 + (i*fheight),6} }, startingPos);

}

	/////////////////////////////////////////////////
	// Main loop
	int frame_counter = 0;
	sf::Clock fps_timer;
	// sf::Clock dt_timer;

	int j = 0;
	int colour_index;

	sf::Int64 tick = clock.get_time().asMicroseconds() / sf::Int64(TIME);
	sf::Clock frame_clock = sf::Clock();
	sf::Clock update_floor_clock = sf::Clock();
	update_floor_clock.restart();

	client::init_palettes();
	irr::video::SColor colour;
	irr::video::SColor colour_red(255, 255, 0, 0);
	irr::video::SColor colour_purple(255, 255, 0, 255);
	irr::video::SColor colour_grey(255, 50, 50, 50);
	irr::video::SColor random_colour;
	srand(clock.get_time().asMicroseconds());

	client::ComponentRenderSceneNode& sn = entity_player.getComponent<client::ComponentRenderSceneNode>();
	irr::scene::ICameraSceneNode *camera_node = new irr::scene::Camera(
		sn.node,
		smgr,
		-1,
		irr::core::vector3df(7, 7, 0),
		irr::core::vector3df(0, 0, 0));
	//irr::core::matrix4 cpm = camera_node->getProjectionMatrix();

	//camera_node->setFOV(1.0f);

	smgr->setActiveCamera(camera_node);
	float dt;


	printf("Entering main loop\n");
	while (device->run()) {


		// Work out a frame delta time.
		// const irr::u32 now = device->getTimer()->getTime();
		dt = frame_clock.restart().asSeconds();
		/// frameDeltaTime = (f32)(now - then)/1000.f; // Time in seconds

		glm::ivec2 playerpos;

		////////////////
		// Events all the time
		{
			playerpos =
				entity_player.getComponent<tempo::ComponentStagePosition>().getOrigin();

			//system_stage_renderer.colorStage(j, playerpos, random_colour, colour_grey);

			// Check for new entities from server
			system_entity.creationCheck(world);
			system_entity.deletionCheck(world);

			// Initialise Graphics for new entities
			system_gc.addEntities(driver, smgr, world);
			system_render_scene_node.setup(smgr, driver);
			system_render_health_bars.setup(smgr);
			system_render_attack.update(system_stage_renderer);
			system_button_renderer.setup(smgr, driver);

			// Receive updates from the server
			system_movement.processIntents(world);
			system_movement.processCorrections(world);
			system_attack.processServerResponses(world);
			system_combo.checkForUpdates(world);
			system_attack.processServerResponses(world);

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
			// click.play();
			if (tick++ % 20 == 0)
				std::cout << "TICK (" << tick << ") " << clock.get_time().asMilliseconds()
				          << "+++++++++++++++" << std::endl;

			j++;
			j = j % 22;
			system_trigger.updateButtons(world);
			system_button_renderer.updateButtons(driver);

			system_render_healing.endBeat();
			system_render_spikes.updateSpikes(driver, playerpos);

			system_translation_animation.endBeat();


			colour_index = rand() % 10;
			random_colour = client::randomHSV(colour_index);


			system_lighting.update(random_colour);
			// sf::Int64 tick2 = update_floor_clock.getElapsedTime().asMilliseconds();
			// std::cout << "Time to update floor: " << (int)(tick2-tick1)<<"ms"
			// << std::endl;
			// system_lighting.update();

		}

		playerpos =
		  entity_player.getComponent<tempo::ComponentStagePosition>().getOrigin();


		////////////////
		// Events at "Delta End"
		if (clock.passed_delta_end()) {
			// std::cout << "End" << std::endl;
			system_combo.advanceBeat();
		}


		// std::clock_t    start;
		//
		// 				start = std::clock();
		//system_stage_renderer.AnimateTiles(dt, playerpos);
		// std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

		system_stage_renderer.Update(smgr, driver, playerpos, random_colour, colour_grey, j, dt);


		//system_stage_renderer.Update(smgr, driver, playerpos);


		driver->beginScene(true, true);
		smgr->drawAll();
		gui_env->drawAll();

		system_render_gui.update(driver, gui_env, clock, combo, comp_health, colour_index, enable_hud);
		driver->endScene();

		++frame_counter;
		if (fps_timer.getElapsedTime().asSeconds() > 1.0f) {
			float seconds = fps_timer.getElapsedTime().asSeconds();
			std::cout << "FPS: " << (int)(frame_counter / seconds) << std::endl;
			fps_timer.restart();
			frame_counter = 0;
		}

	}  // main loop
	running.store(false);
	printf("Left main loop\n");

	// Tell server we are gone
	tempo::disconnectFromServer(entity_player);

	// Close server listener and destroy the game
	listener.join();
	world.clear();
	device->drop();

	return 0;
}
