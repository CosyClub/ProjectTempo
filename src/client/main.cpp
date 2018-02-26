#include <cstdio>
#include <iostream>
#include <irrlicht.h>

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

#include <glm/vec2.hpp>

#define BPM 174
#define DELTA 150
#define TIME 60000000 / BPM

#include <thread>
#include <chrono>

using namespace irr;


anax::Entity createEntityStage(anax::World& world){
	printf("Creating entity stage\n");
	anax::Entity entity_stage = world.createEntity();
	entity_stage.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	entity_stage.activate();

	return entity_stage;
}

anax::Entity createEntityPlayer(anax::World& world) {
	printf("Creating entity player\n");
	anax::Entity entity_player = world.createEntity();
	entity_player.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	entity_player.addComponent<tempo::ComponentStagePosition>(glm::ivec2(5, 5));
	entity_player.addComponent<client::ComponentRenderSceneNode>(nullptr);
	entity_player.addComponent<client::ComponentKeyInput>();
	entity_player.activate();

	return entity_player;
}

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
	smgr->setAmbientLight(irr::video::SColorf(0.3f, 0.3f, 0.3f));

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
	//irr::scene::ISceneNode* camera_light = smgr->addLightSceneNode(
	//                                                               camera_node,
	//                                                               irr::core::vector3df(0.0f, 0.0f, 0.0f),
	//                                                               irr::video::SColorf(0.8f, 0.8f, 0.8f),
	//                                                               10.0f);
	// debug static light
	irr::scene::ILightSceneNode* light_node = smgr->addLightSceneNode(
	                                                             0,
	                                                             irr::core::vector3df(10.0f, 10.0f, 10.0f),
	                                                             irr::video::SColorf(0.8f, 0.8f, 0.8f),
	                                                             5.0f);
	irr::video::SLight& light_data = light_node->getLightData();

	// Create World
	anax::World world;
	client::SystemStageRenderer system_stage_renderer;
	client::SystemRenderSceneNode system_render_scene_node;
	client::SystemUpdateKeyInput system_update_key_input;
	world.addSystem(system_stage_renderer);
	world.addSystem(system_render_scene_node);
	world.addSystem(system_update_key_input);
	createEntityStage(world);
	anax::Entity entity_player = createEntityPlayer(world);

	// Setup Systems
	world.refresh();
	system_stage_renderer.setup(smgr, driver);
	system_render_scene_node.setup(smgr);
	system_update_key_input.setup(device);

	int lastFPS = -1;
	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

	// This is the movemen speed in units per second.
	const f32 MOVEMENT_SPEED = 1.0f;

 sf::Clock fps_timer;
 sf::Clock dt_timer;
 sf::Time logic_time;
 sf::Time render_time;
 int frame_counter = 0;

 sf::Int64 tick = clock.get_time().asMicroseconds() / sf::Int64(TIME);
 sf::Clock frame_clock = sf::Clock();
 frame_clock.restart();

	mainsong.start();
	//sync_time(clock, &mainsong);
	//mainsong.set_volume(20.f);
	long offset = 0;
	int j = 0;
	printf("Entering main loop\n");
	while(device->run()){
		if (device->isWindowActive()) {

			float dt = dt_timer.getElapsedTime().asSeconds();
			dt_timer.restart();

			// Work out a frame delta time.
			const u32 now = device->getTimer()->getTime();
			const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
			then = now;

			// float light_intensity = (sin((float)now * 0.01f) + 1.0f) / 2.0f;
			//
			// light_data.DiffuseColor.set(1, light_intensity * 0.3f, light_intensity, light_intensity * 0.3f);

			system_update_key_input.addKeys();

			// TODO: move into multiple systems, for now teleport the user
			// the two systems required are input->transform and transform->apply
			{
				std::vector<char> keys = entity_player.getComponent<client::ComponentKeyInput>().keysPressed;
				tempo::ComponentStagePosition& pos = entity_player.getComponent<tempo::ComponentStagePosition>();

				for (int i = 0; i < keys.size(); i++) {
					switch (keys[i]) {
					case 'w':
						pos.occupied[0].x -= 1;
						break;
					case 'a':
						pos.occupied[0].y -= 1;
						break;
					case 's':
						pos.occupied[0].x += 1;
						break;
					case 'd':
						pos.occupied[0].y += 1;
						break;
					}
				}
			}
			if (clock.passed_beat()) {
			 std::cout << "TICK (" << tick << ") " << clock.get_time().asMilliseconds() << "+++++++++++++++" << std::endl;
			 j++;
			 if(j > 22)
				 j = 18;

			 system_stage_renderer.updateStage({8,8}, driver, j);
			 click.play();
			}

			system_render_scene_node.update();

			driver->beginScene(true, true);
			smgr->drawAll();
			gui_env->drawAll();

			driver->endScene();

		// 	++frame_counter;
		// if (fps_timer.getElapsedTime().asSeconds() > 0.5f) {
		// 	float seconds = fps_timer.getElapsedTime().asSeconds();
		// 	printf("FPS: %i (%.1f% render)\n", (int)(frame_counter / seconds),
		// 		100 * (float)(
		// 			render_time.asMicroseconds()
		// 			) / (
		// 				logic_time.asMicroseconds() +
		// 				render_time.asMicroseconds()));
		// 	printf("Logic time  (μs): %d\n",  logic_time.asMicroseconds());
		// 	printf("Render time (μs): %d\n", render_time.asMicroseconds());
		// 	fps_timer.restart();
		// 	frame_counter = 0;
		// }
		}
		else {
			device->yield();
		}

	}
	printf("Left main loop\n");

	device->drop();

	return 0;
}
