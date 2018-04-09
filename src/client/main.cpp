	#include <client/component/ComponentKeyInput.hpp>
#include <client/component/ComponentRenderButtonGroup.hpp>
#include <client/component/ComponentRenderSpikes.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>
#include <client/network/client.hpp>
#include <client/system/SystemAttack.hpp>
#include <client/system/SystemButtonRenderer.hpp>
#include <client/system/SystemCombo.hpp>
#include <client/system/SystemEntity.hpp>
#include <client/system/SystemGraphicsCreation.hpp>
#include <client/system/SystemMovement.hpp>
#include <client/system/SystemParseKeyInput.hpp>
#include <client/system/SystemRenderGUI.hpp>
#include <client/system/SystemRenderHealing.hpp>
#include <client/system/SystemRenderHealthBars.hpp>
#include <client/system/SystemRenderSceneNode.hpp>
#include <client/system/SystemRenderSpikes.hpp>
#include <client/system/SystemStageRenderer.hpp>
#include <client/system/SystemUpdateKeyInput.hpp>
#include <client/system/SystemTranslationAnimation.hpp>

#include <tempo/component/ComponentButtonGroup.hpp>
#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentSpikes.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
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
		void lessJank() {
			// uncomment this for more jank:
			//return;

			auto& entities = getEntities();

			for (auto& entity : entities) {
				tempo::ComponentStageTranslation& trans = entity.getComponent<tempo::ComponentStageTranslation>();
				tempo::ComponentStage& stage = entity.getComponent<tempo::ComponentStage>();
				glm::ivec2 origin = entity.getComponent<tempo::ComponentStagePosition>().getOrigin();



				glm::ivec2 dest = origin + trans.delta;

				if (!stage.existstTile(dest) || stage.getHeight(dest) >= 5) {
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
			}
		}
	};

}  // namespace client

void sync_time(tempo::Clock &clock)
{
	sf::Int64 offset = tempo::timeSyncClient(&clock);
	clock.set_time(clock.get_time() + sf::microseconds(offset));
}

anax::Entity createEntityStage(anax::World &world)
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
// 	entity_player.addComponent<tempo::ComponentStagePosition>(glm::ivec2(5, 5));
// 	entity_player.addComponent<client::ComponentRenderSceneNode>(nullptr);
// 	entity_player.addComponent<client::ComponentKeyInput>();
// 	entity_player.activate();

// 	return entity_player;
// }

int main(int argc, const char **argv)
{
	sf::SoundBuffer clickbuf;
	clickbuf.loadFromFile("resources/sound/tick.ogg");
	sf::Sound click;
	click.setBuffer(clickbuf);

	// Clock
	tempo::Clock clock = tempo::Clock(sf::microseconds(TIME), sf::milliseconds(DELTA));

	KeyInput             receiver;

	// This makes it full-screen
	// irr::IrrlichtDevice *nulldevice = irr::createDevice(irr::video::EDT_NULL);
	// irr::core::dimension2d<irr::u32> deskres = nulldevice->getVideoModeList()->getDesktopResolution();
	// nulldevice -> drop();
	// irr::IrrlichtDevice *device = irr::createDevice(
	//   irr::video::EDT_OPENGL, deskres, 16, true, false, false);


	irr::IrrlichtDevice *device = irr::createDevice(
	  irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(1280, 720), 16, false, false, false);
	if (!device) {
		printf("Failed to create Irrlicht Device\n");
		return 1;
	}
	device->setWindowCaption(L"RaveCave");
	irr::video::IVideoDriver * driver  = device->getVideoDriver();
	irr::scene::ISceneManager *smgr    = device->getSceneManager();
	irr::gui::IGUIEnvironment *gui_env = device->getGUIEnvironment();
	// Debug
	smgr->setAmbientLight(irr::video::SColorf(0.3f, 0.3f, 0.3f));

	/////////////////////////////////////////////////
	// Setup ECS
	anax::World world;
	// tempo::SystemRender           system_render(app);
	tempo::SystemHealth            system_health;
	tempo::SystemTrigger           system_trigger(world);
	client::SystemAttack           system_attack;
	client::SystemButtonRenderer   system_button_renderer;
	client::SystemCombo             system_combo;
	client::SystemEntity           system_entity;
	client::SystemGraphicsCreation system_gc;
	client::SystemMovement         system_movement;
	client::SystemStageRenderer    system_stage_renderer;
	client::SystemParseKeyInput    system_parse_key_input;
	client::SystemRenderGUI        system_render_gui;
	client::SystemRenderHealing    system_render_healing(driver, smgr);
	client::SystemRenderHealthBars system_render_health_bars;
	client::SystemRenderSceneNode  system_render_scene_node;
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
	world.addSystem(system_trigger);
	world.addSystem(system_button_renderer);
	world.addSystem(system_stage_renderer);

	world.addSystem(system_render_healing);
	world.addSystem(system_render_health_bars);
	world.addSystem(system_render_scene_node);
	world.addSystem(system_render_spikes);
	world.addSystem(system_update_key_input);
	world.addSystem(system_parse_key_input);
	world.addSystem(system_movement);
	world.addSystem(system_translation_animation);
	world.addSystem(system_less_jank);

	anax::Entity entity_stage = createEntityStage(world);
	world.refresh();

	// Initialise Systems
	system_update_key_input.setup(device);
	system_stage_renderer.setup(smgr, driver,{255, 175, 0, 0}, {255, 50, 50, 50});
	system_render_scene_node.setup(smgr, driver);

	// must be after system_render_scene_node.setup(smgr);
	system_render_health_bars.setup(smgr);

	// Set up remote address, local ports and remote handshake port
	// Note, IF statement is to change ports for local development, bit
	// hacky and should be removed in due course!
	tempo::addr_r = "127.0.0.1";
	if (argc == 2)
		tempo::addr_r = argv[1];
	if (tempo::addr_r == "127.0.0.1") {
		std::srand(time(NULL));
		int d          = std::rand() % 10;
		tempo::port_ci = DEFAULT_PORT_IN + 10 + d;
		tempo::port_co = DEFAULT_PORT_OUT + 10 + d;
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
	std::thread       listener(tempo::listenForServerUpdates, std::ref(running));
	// Hack to allow printouts to line up a bit nicer :)
	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	tempo::ClientRole     role     = tempo::ClientRole::PLAYER;
	tempo::ClientRoleData roleData = {"Bilbo Baggins"};

	// Connect to server and handshake information
	tempo::connectToAndSyncWithServer(role, roleData, world);

	// Sort out graphics after handshake
	system_gc.addEntities(driver, smgr, world);
	system_render_scene_node.setup(smgr, driver);
	system_render_health_bars.setup(smgr);
	system_button_renderer.setup(smgr, driver);
	system_render_spikes.setup(smgr, driver);

	// Start and Sync Song
	sync_time(clock);
	// long offset = 0;

	// Player
	// TODO: use better way to find out player, for now this is a search
	anax::Entity entity_player;
	for (auto &entity : world.getEntities()) {
		if (entity.hasComponent<tempo::ComponentPlayerLocal>()) {
			entity_player = entity;
			break;
		}
	}
	entity_player.addComponent<client::ComponentKeyInput>();
	entity_player.activate();
	client::ComponentRenderSceneNode &sn =
	  entity_player.getComponent<client::ComponentRenderSceneNode>();

	auto &combo = entity_player.getComponent<tempo::ComponentCombo>().comboCounter;
	auto &comp_health = entity_player.getComponent<tempo::ComponentHealth>();

	irr::scene::ICameraSceneNode *camera_node;
	if (false) {
		float rotateSpeed = 25.0f;
		float moveSpeed   = 0.1f;
		camera_node       = smgr->addCameraSceneNodeFPS(nullptr, rotateSpeed, moveSpeed);
		device->getCursorControl()->setVisible(false);
	} else {
		float rotate    = 0.0f;
		float translate = 0.0f;  //-100
		float zoom      = 0.0f;  // 100
		float distance  = 0.0f;
		// camera_node = smgr->addCameraSceneNodeMaya(sn.node, rotate, translate, zoom, -1,
		// distance); camera_node->setPosition(irr::core::vector3df(0.0f, 0.0f, 0.0f));
		camera_node = smgr->addCameraSceneNode();
		camera_node->setPosition(irr::core::vector3df(14, 9, 0));
		camera_node->setTarget(sn.node->getPosition());
		// camera_node->setRotation(irr::core::vector3df(0,0,90));
		device->getCursorControl()->setVisible(true);
	}

	// irr::scene::ISceneNode* camera_light;
	// camera_light = smgr->addLightSceneNode(camera_node,
	//                                        irr::core::vector3df(0.0f, 0.0f, 0.0f),
	//                                        irr::video::SColorf(0.8f, 0.8f, 0.8f),
	//                                        10.0f);
	// debug static light
	irr::scene::ILightSceneNode *light_node;
	light_node = smgr->addLightSceneNode(0, irr::core::vector3df(10.0f, 10.0f, 10.0f),
	                                     irr::video::SColorf(0.8f, 0.8f, 0.8f), 5.0f);
	// irr::video::SLight& light_data = light_node->getLightData();

	/////////////////////////////////////////////////
	// Main loop
	int       frame_counter = 0;
	sf::Clock fps_timer;
	// sf::Clock dt_timer;

	int j = 0;

	sf::Int64 tick               = clock.get_time().asMicroseconds() / sf::Int64(TIME);
	sf::Clock frame_clock        = sf::Clock();
	sf::Clock update_floor_clock = sf::Clock();
	frame_clock.restart();
	update_floor_clock.restart();

	printf("Entering main loop\n");
	while (device->run()) {
		// sf::Int64 tick1 = update_floor_clock.getElapsedTime().asMilliseconds();
		// float dt = dt_timer.getElapsedTime().asSeconds();
		// dt_timer.restart();

		////////////////
		// Events all the time
		{
			// Check for new entities from server
			system_entity.creationCheck(world);
			system_entity.deletionCheck(world);

			// Initialise Graphics for new entities
			system_gc.addEntities(driver, smgr, world);
			system_render_scene_node.setup(smgr,driver);
			system_render_health_bars.setup(smgr);

			// Receive updates from the server
			system_movement.processIntents(world);
			system_movement.processCorrections(world);
			system_combo.checkForUpdates(world);

			// Deal with local input
			system_update_key_input.clear();
			system_update_key_input.addKeys();
			system_parse_key_input.parseInput(clock, device);

			// Deprecated/To-be-worked-on
			system_health.CheckHealth();
			system_health.client_receiveHealth(world);

			system_less_jank.lessJank();
			// Update animations from translations received from server
			system_translation_animation.updateAnimations();

			// Graphics updates
			// std::cout << "START OF CRASH LINE 312 CLIENT MAIN.CPP" << std::endl;
			system_render_scene_node.update();
			// std::cout << "IF YOU SEE THIS AFTER A SECOND CLIENT CONNECTS YOU FIXED IT" <<
			// std::endl;
			system_render_health_bars.update();
			system_render_healing.update();

			// TODO: Make a system for updating camera position
			irr::core::vector3df camera_target = sn.node->getAbsolutePosition();
			camera_node->setPosition(camera_target + irr::core::vector3df(14, 9, 0));
			camera_node->updateAbsolutePosition();
			camera_node->setTarget(camera_target);
		}

		////////////////
		// Events at "Delta Start"
		if (clock.passed_delta_start()) {
			// std::cout << "Start" << std::endl;
		}

		////////////////
		// Events at "Beat Passed"
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
			system_render_spikes.updateSpikes(driver);

			system_translation_animation.endBeat();
			// sf::Int64 tick2 = update_floor_clock.getElapsedTime().asMilliseconds();
			// std::cout << "Time to update floor: " << (int)(tick2-tick1)<<"ms"
			// << std::endl;
		}
		glm::ivec2 playerpos =
		  entity_player.getComponent<tempo::ComponentStagePosition>().getOrigin();
		system_stage_renderer.updateStage(smgr, driver, j, playerpos);

		////////////////
		// Events at "Delta End"
		if (clock.passed_delta_end()) {
			// std::cout << "End" << std::endl;
			system_combo.advanceBeat();
		}

		// Rendering Code
		// if (!device->isWindowActive()) {
		//	device->yield();
		//	continue;
		//}

		// sf::Int64 tick2 = update_floor_clock.getElapsedTime().asMilliseconds();
		// std::cout << "Time to update floor: " << (int)(tick2-tick1)<<"ms"
		// << std::endl;

		driver->beginScene(true, true);
		smgr->drawAll();
		gui_env->drawAll();

		system_render_gui.update(driver, gui_env, clock, combo, comp_health);
		driver->endScene();

		++frame_counter;
		if (fps_timer.getElapsedTime().asSeconds() > 1.0f) {
			float seconds = fps_timer.getElapsedTime().asSeconds();
			std::cout << "FPS: " << (int) (frame_counter / seconds) << std::endl;
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
