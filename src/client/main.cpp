#include <client/component/ComponentKeyInput.hpp>
#include <client/component/ComponentRenderButtonGroup.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>
#include <client/network/client.hpp>
#include <client/system/SystemAttack.hpp>
#include <client/system/SystemButtonRenderer.hpp>
#include <client/system/SystemGraphicsCreation.hpp>
#include <client/system/SystemMovement.hpp>
#include <client/system/SystemParseKeyInput.hpp>

#include <client/system/SystemRenderHealthBars.hpp>
#include <client/system/SystemRenderSceneNode.hpp>
#include <client/system/SystemStageRenderer.hpp>
#include <client/system/SystemUpdateKeyInput.hpp>

#include <tempo/component/ComponentButtonGroup.hpp>
#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/entity/EntityCreation.hpp>
#include <tempo/network/ID.hpp>
#include <tempo/song.hpp>
#include <tempo/system/SystemCombo.hpp>
#include <tempo/system/SystemGridAi.hpp>
#include <tempo/system/SystemHealth.hpp>
#include <tempo/system/SystemLevelManager.hpp>
#include <tempo/system/SystemTransform.hpp>
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

#define BPM 120              // Beats per minutes
#define DELTA 125            // Delta around a beat a player can hit (millisecs)
#define TIME 60000000 / BPM  // Time between beats (microsecs)

void sync_time(tempo::Clock &clock, tempo::Song *song)
{
	sf::Int64 offset = tempo::timeSyncClient(&clock);
	clock.set_time(clock.get_time() + sf::microseconds(offset), song);
}

void new_entity_check(anax::World &world)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(tempo::QueueID::ENTITY_CREATION);
	while (!q->empty()) {
		sf::Packet p = q->front();
		tempo::addComponent(world, p);
		q->pop();
	}
	world.refresh();
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

anax::Entity createButtonGroup(anax::World &world, std::vector<glm::ivec2> positions)
{
	printf("Creating button\n");
	anax::Entity entity_button = world.createEntity();
	entity_button.addComponent<tempo::ComponentButtonGroup>(positions);
	entity_button.addComponent<client::ComponentRenderButtonGroup>();
	entity_button.activate();

	return entity_button;
}

int main(int argc, const char **argv)
{
	tempo::Song mainsong("resources/sound/focus.ogg");
	mainsong.set_volume(0.f);

	sf::SoundBuffer clickbuf;
	clickbuf.loadFromFile("resources/sound/tick.ogg");
	sf::Sound click;
	click.setBuffer(clickbuf);

	// Clock
	tempo::Clock clock = tempo::Clock(sf::microseconds(TIME), sf::milliseconds(DELTA));

	KeyInput             receiver;
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
	tempo::SystemLevelManager      system_level(world, "../bin/resources/levels/levelTest.bmp",
                                           "../bin/resources/levels/zonesTest.bmp");
	tempo::SystemUpdateTransforms  system_update_transforms;
	tempo::SystemGridAi            system_grid_ai;
	tempo::SystemCombo             system_combo;
	tempo::SystemHealth            system_health;
	tempo::SystemTrigger           system_trigger(world);
	client::SystemAttack           system_attack;
	client::SystemButtonRenderer   system_button_renderer;
	client::SystemGraphicsCreation system_gc;
	client::SystemMovement         system_movement;
	client::SystemStageRenderer    system_stage_renderer;
	client::SystemParseKeyInput    system_parse_key_input;
	client::SystemRenderHealthBars system_render_health_bars;
	client::SystemRenderSceneNode  system_render_scene_node;
	client::SystemUpdateKeyInput   system_update_key_input;

	// Add Systems
	world.addSystem(system_level);
	world.addSystem(system_attack);
	world.addSystem(system_update_transforms);
	world.addSystem(system_grid_ai);
	world.addSystem(system_combo);
	world.addSystem(system_health);
	world.addSystem(system_gc);
	world.addSystem(system_trigger);
	world.addSystem(system_button_renderer);
	world.addSystem(system_stage_renderer);
	world.addSystem(system_render_scene_node);
	world.addSystem(system_render_health_bars);
	world.addSystem(system_update_key_input);
	world.addSystem(system_parse_key_input);
	world.addSystem(system_movement);

	createEntityStage(world);
	world.refresh();

	// Initialise Systems
	system_update_key_input.setup(device);
	system_stage_renderer.setup(smgr, driver);
	system_render_scene_node.setup(smgr);

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
	system_render_scene_node.setup(smgr);
	system_render_health_bars.setup(smgr);

	// Start and Sync Song
	// mainsong.start();
	sync_time(clock, &mainsong);
	mainsong.set_volume(20.f);
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

	auto& combo = entity_player.getComponent<tempo::ComponentCombo>().comboCounter;

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
		camera_node->setParent(sn.node);
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

	// buttons
	anax::Entity entity_button = createButtonGroup(world, {{8, 8}, {9, 9}});
	world.refresh();
	system_button_renderer.setup(smgr, driver);

	printf("Entering main loop\n");
	while (device->run()) {
		// float dt = dt_timer.getElapsedTime().asSeconds();
		// dt_timer.restart();

		////////////////
		// Events all the time
		{
			// Check for new entities from server
			new_entity_check(world);
			system_gc.addEntities(driver, smgr, world);
			system_render_scene_node.setup(smgr);
			system_render_health_bars.setup(smgr);

			// Recieve updates from the server
			system_movement.processIntents(world);
			system_movement.processCorrections(world);

			// Deal with local input
			system_update_key_input.clear();
			system_update_key_input.addKeys();
			system_parse_key_input.parseInput(clock);

			// Deprecated/To-be-worked-on
			system_health.CheckHealth();

			// Graphics updates
			// std::cout << "START OF CRASH LINE 312 CLIENT MAIN.CPP" << std::endl;
			system_render_scene_node.update();
			// std::cout << "IF YOU SEE THIS AFTER A SECOND CLIENT CONNECTS YOU FIXED IT" << std::endl;
			system_render_health_bars.update();
			// TODO: Make a system for updating camera position
			camera_node->setTarget(sn.node->getPosition());
		}

		////////////////
		// Events at "Delta Start"
		if (clock.passed_delta_start()) {
			// std::cout << "Start" << std::endl;
		}

		////////////////
		// Events at "Beat Passed"
		if (clock.passed_beat()) {
			click.play();
			if (tick++ % 20 == 0)
				std::cout << "TICK (" << tick << ") " << clock.get_time().asMilliseconds()
				          << "+++++++++++++++" << std::endl;

			j++;
			j = j % 22;
			// sf::Int64 tick1 = update_floor_clock.getElapsedTime().asMilliseconds();
			system_stage_renderer.updateStage({255, 175, 0, 0}, {255, 50, 50, 50}, driver, j);
			system_trigger.updateButtons();
			system_button_renderer.updateButtons(driver);
			// sf::Int64 tick2 = update_floor_clock.getElapsedTime().asMilliseconds();
			// std::cout << "Time to update floor: " << (int)(tick2-tick1)<<"ms"
			// << std::endl;

			system_grid_ai.update();
		}

		////////////////
		// Events at "Delta End"
		if (clock.passed_delta_end()) {
			// std::cout << "End" << std::endl;
			system_combo.advanceBeat();
		}

		// Rendering Code
		if (!device->isWindowActive()) {
			device->yield();
			continue;
		}

		driver->beginScene(true, true);
		smgr->drawAll();
		gui_env->drawAll();

		irr::gui::IGUIFont* font = gui_env->getFont("resources/fonts/liberation_sans.xml");
		if (font) {
			irr::core::stringw str = L"Combo: ";
			str += combo;
			font->setKerningWidth(6);
			font->setKerningHeight(6);
			font->draw(str.c_str(),
					irr::core::rect<s32>(10,650,300,300),
					irr::video::SColor(255,255,255,255));
				}

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

	device->drop();
	listener.join();

	return 0;
}
