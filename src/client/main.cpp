#include <cstdio>
#include <iostream>
#include <irrlicht.h>

// #include <tempo/Application.hpp>
// #include <tempo/song.hpp>
// #include <tempo/time.hpp>
#include <tempo/entity/EntityCreation.hpp>
// #include <tempo/entity/LevelRenderer.hpp>
#include <client/network/client.hpp>
#include <client/system/SystemGameInput.hpp>
#include <client/system/SystemGraphicsCreation.hpp>

#include <tempo/network/QueueID.hpp>
// #include <tempo/system/SystemRenderHealth.hpp>
#include <tempo/system/SystemTransform.hpp>
#include <tempo/system/SystemCombo.hpp>
#include <tempo/system/SystemGridAi.hpp>
#include <tempo/system/SystemHealth.hpp>
#include <tempo/system/SystemLevelManager.hpp>
#include <tempo/system/SystemPlayer.hpp>
// #include <tempo/system/SystemRender.hpp>
#include <anax/World.hpp>
#include <anax/Entity.hpp>

#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <tempo/song.hpp>
#include <tempo/time.hpp>


#include <client/system/SystemStageRenderer.hpp>
#include <client/system/SystemRenderSceneNode.hpp>
#include <client/system/SystemUpdateKeyInput.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>
#include <client/component/ComponentKeyInput.hpp>

#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>

#include <glm/vec2.hpp>

#include <thread>
#include <chrono>

#define BPM 120
#define DELTA 200
#define TIME 60000000 / BPM

void sync_time(tempo::Clock& clock, tempo::Song *song)
{
	sf::Int64 offset = tempo::timeSyncClient(&clock);
	clock.set_time(clock.get_time() + sf::microseconds(offset), song);
}

void new_entity_check(anax::World &world, tempo::SystemLevelManager system_level)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(tempo::QueueID::ENTITY_CREATION);
	while (!q->empty())
	{
		sf::Packet p = q->front();
		tempo::addComponent(world, p);
		q->pop();
	}
	world.refresh();
}

anax::Entity createEntityStage(anax::World& world){
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

int main(int argc, const char** argv){

	tempo::Song mainsong("resources/sound/focus.ogg");
	mainsong.set_volume(0.f);

	sf::SoundBuffer clickbuf;
	clickbuf.loadFromFile("resources/sound/tick.ogg");
	sf::Sound click;
	click.setBuffer(clickbuf);

	// Clock
	tempo::Clock clock = tempo::Clock(sf::microseconds(TIME), sf::milliseconds(DELTA));

	KeyInput receiver;
	irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_OPENGL,
	                                                irr::core::dimension2d<irr::u32>(1280, 720),
	                                                16,
	                                                false, false, false);
	if(!device){
		printf("Failed to create Irrlicht Device\n");
		return 1;
	}
	device->setWindowCaption(L"RaveCave");
	irr::video::IVideoDriver*  driver  = device->getVideoDriver();
	irr::scene::ISceneManager* smgr    = device->getSceneManager();
	irr::gui::IGUIEnvironment* gui_env = device->getGUIEnvironment();
	// Debug
	smgr->setAmbientLight(irr::video::SColorf(0.1f, 0.1f, 0.1f));

	/////////////////////////////////////////////////
	// Setup scene
	anax::World world;
	// tempo::SystemRender           system_render(app);
	tempo::SystemLevelManager     system_level(world,
	                                           "../bin/resources/levels/levelTest.bmp",
	                                           "../bin/resources/levels/zonesTest.bmp"
	                                           );
	tempo::SystemUpdateTransforms system_update_transforms;
	tempo::SystemGridAi           system_grid_ai;
	tempo::SystemGameInput        system_input(clock);
	tempo::SystemPlayer           system_player(clock);
	tempo::SystemCombo            system_combo;
	tempo::SystemHealth           system_health;
	// tempo::RenderHealth           render_health;
	client::SystemGraphicsCreation system_gc;
	client::SystemStageRenderer system_stage_renderer;
	client::SystemRenderSceneNode system_render_scene_node;
	client::SystemUpdateKeyInput system_update_key_input;


	// Setup Systems
	// world.refresh();

	world.addSystem(system_stage_renderer);
	world.addSystem(system_render_scene_node);
	world.addSystem(system_update_key_input);
	world.addSystem(system_level);
	world.addSystem(system_update_transforms);
	world.addSystem(system_grid_ai);
	// world.addSystem(system_render);
	world.addSystem(system_input);
	world.addSystem(system_gc);
	world.addSystem(system_player);
	world.addSystem(system_combo);
	world.addSystem(system_health);
	// world.addSystem(render_health);
	
	createEntityStage(world);
	world.refresh();

	system_stage_renderer.setup(smgr);
	system_render_scene_node.setup(smgr);
	system_update_key_input.setup(device);


	// Set up remote address, local ports and remote handshake port
	// Note, IF statement is to change ports for local development, bit
	// hacky and should be removed in due course!
	tempo::addr_r = "127.0.0.1";
	if (argc == 2) tempo::addr_r = argv[1];
	if (tempo::addr_r == "127.0.0.1") {
		std::srand (time(NULL));
		int d = std::rand() % 10;
		tempo::port_ci = DEFAULT_PORT_IN+10+d;
		tempo::port_co = DEFAULT_PORT_OUT+10+d;
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
	std::thread listener (tempo::listenForServerUpdates);

	tempo::ClientRole role = tempo::ClientRole::PLAYER;
	tempo::ClientRoleData roleData = {"Bilbo Baggins"};

	// Connect to server and handshake information
	tempo::connectToAndSyncWithServer(role, roleData, world, system_level);

	//Sort out graphics after handshake	
	world.refresh();
	system_gc.addEntities(driver, smgr);
	world.refresh();
	system_render_scene_node.setup(smgr);

	// Start and Sync Song
	// mainsong.start();
	sync_time(clock, &mainsong);
	mainsong.set_volume(20.f);
	long offset = 0;

	// Player
	/* anax::Entity entity_player = tempo::newPlayer(world, scene, 0, tempo::EID_PLAYER, system_level, 2, 2); */
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

	irr::scene::ICameraSceneNode* camera_node;
	if (false) {
		float rotateSpeed = 25.0f;
		float moveSpeed = 0.1f;
		camera_node = smgr->addCameraSceneNodeFPS(nullptr, rotateSpeed, moveSpeed);
		device->getCursorControl()->setVisible(false);
	} else {
		float rotate = -100.0f;
		float translate = -100.0f;
		float zoom = 100.0f;
		float distance = 15.0f;
		camera_node = smgr->addCameraSceneNodeMaya(nullptr, rotate, translate, zoom, -1, distance);
		device->getCursorControl()->setVisible(true);
		camera_node->setTarget(irr::core::vector3df(10.0f, 0.0f, 10.0f));
	}
	// debug dynamic light
	irr::scene::ISceneNode* camera_light = smgr->addLightSceneNode(
		camera_node,
		irr::core::vector3df(0.0f, 0.0f, 0.0f),
		irr::video::SColorf(1.0f, 1.0f, 1.0f),
		10.0f);

	// debug static light
	irr::scene::ISceneNode* light_node = smgr->addLightSceneNode(
		0,
		irr::core::vector3df(10.0f, 10.0f, 10.0f),
		irr::video::SColorf(1.0f, 1.0f, 1.0f),
		5.0f);

	// // Create World
	// anax::World world;

	/////////////////////////////////////////////////
	// Main loop
	
	int lastFPS = -1;

	sf::Clock fps_timer;
	sf::Clock dt_timer;
	sf::Time logic_time;
	sf::Time render_time;
	bool running = true;
	int frame_counter = 0;

	sf::Int64 tick = clock.get_time().asMicroseconds() / sf::Int64(TIME);
	sf::Clock frame_clock = sf::Clock();
	frame_clock.restart();
	sf::Time min_frame_time = sf::microseconds(1000000 / 60);

	printf("Entering main loop\n");
	while(device->run()){
		float dt = dt_timer.getElapsedTime().asSeconds();
		dt_timer.restart();

		//add new clients
		new_entity_check(world, system_level);
		//Add new graphics stuff
		system_gc.addEntities(driver, smgr);
		
		world.refresh();
		system_player.update(entity_player.getId(), world);
		// render_health.HealthBarUpdate();
		system_health.CheckHealth();
		system_level.update(dt);
		system_update_transforms.update(system_level);

		if (clock.passed_beat()) {
			click.play();
			if (tick++ % 20 == 0)
				std::cout << "TICK (" << tick << ") " << clock.get_time().asMilliseconds() << "+++++++++++++++" << std::endl;
      

			system_grid_ai.update();
			system_combo.advanceBeat();
		}


		auto& combo = entity_player.getComponent<tempo::ComponentCombo>();
		char buffer [50];
		sprintf (buffer, "Combo: %d", combo.comboCounter);

		if (! device->isWindowActive())
		{
			device->yield();
			continue;
		}
		

		system_update_key_input.addKeys();

		// TODO: move into multiple systems, for now teleport the user
		// the two systems required are input->transform and transform->apply
		{
			std::vector<char> keys = entity_player.getComponent<client::ComponentKeyInput>().keysPressed;
			tempo::ComponentStagePosition& pos = entity_player.getComponent<tempo::ComponentStagePosition>();
			tempo::ComponentStageRotation& rot = entity_player.getComponent<tempo::ComponentStageRotation>();

			for (int i = 0; i < keys.size(); i++) {
				switch (keys[i]) {
				case 'w':
					pos.occupied[0] += tempo::NORTH;
					rot = tempo::NORTH;
					break;
				case 'a':
					pos.occupied[0] += tempo::WEST;
					rot = tempo::WEST;
					break;
				case 's':
					pos.occupied[0] += tempo::SOUTH;
					rot = tempo::SOUTH;
					break;
				case 'd':
					pos.occupied[0] += tempo::EAST;
					rot = tempo::EAST;
					break;
				}
			}
		}
		system_render_scene_node.update();

		driver->beginScene(true, true);
		smgr->drawAll();
		gui_env->drawAll();

		driver->endScene();


		++frame_counter;
		if (fps_timer.getElapsedTime().asSeconds() > 0.5f) {
			float seconds = fps_timer.getElapsedTime().asSeconds();
			/* printf("FPS: %i (%.1f% render)\n", (int)(frame_counter / seconds), */
			/* 	100 * (float)( */
			/* 		render_time.asMicroseconds() */
			/* 		) / ( */
			/* 			logic_time.asMicroseconds() + */
			/* 			render_time.asMicroseconds())); */
			/* printf("Logic time  (μs): %d\n",  logic_time.asMicroseconds()); */
			/* printf("Render time (μs): %d\n", render_time.asMicroseconds()); */
			fps_timer.restart();
			frame_counter = 0;
		}

	}
	printf("Left main loop\n");

	device->drop();

	return 0;
}
