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
	                                                irr::core::dimension2d<irr::u32>(800, 600),
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
	irr::scene::ISceneNode* light_node = smgr->addLightSceneNode(
		camera_node,
		irr::core::vector3df(0.0f, 10.0f, 0.0f),
		irr::video::SColorf(1.0f, 1.0f, 1.0f),
		1000.0f);

	irr::core::dimension2d<irr::f32> size(1.0f, 2.0f);
	irr::core::vector3df position(0.0f, 0.0f, 0.0f);
	irr::video::SColor red(255, 255, 0, 0);
	irr::scene::ISceneNode* billboard = smgr->addBillboardSceneNode(
		nullptr,
		size,
		position,
		-1,
		red,
		red);
		//  if (anim)
		//  {
		// 		 billboard->addAnimator(anim);
		// 		 anim->drop();
		//  }
	scene::ISceneNodeAnimator* anim =
						 smgr->createFlyCircleAnimator(core::vector3df(0,0,30), 20.0f);

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
	system_stage_renderer.setup(smgr);
	system_render_scene_node.setup(smgr);
	system_update_key_input.setup(device);

	int lastFPS = -1;
	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

	// This is the movemen speed in units per second.
	const f32 MOVEMENT_SPEED = 5.f;


	mainsong.start();
	//sync_time(clock, &mainsong);
	mainsong.set_volume(20.f);
	long offset = 0;

	printf("Entering main loop\n");
	while(device->run()){
		if (device->isWindowActive()) {
			// Work out a frame delta time.
			const u32 now = device->getTimer()->getTime();
			const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
			then = now;

			{
				core::vector3df nodePosition = billboard->getPosition();
				if (receiver.IsKeyDown(irr::KEY_KEY_W)) {
					nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
				}
				else if (receiver.IsKeyDown(irr::KEY_KEY_S))
					nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;
				if (receiver.IsKeyDown(irr::KEY_KEY_A))
					nodePosition.Z -= MOVEMENT_SPEED * frameDeltaTime;
				else if (receiver.IsKeyDown(irr::KEY_KEY_D))
					nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime;
				billboard->setPosition(nodePosition);
			}

			system_update_key_input.clear();
			system_update_key_input.addkey();

			// TODO: move into multiple systems, for now teleport the user
			// the two systems required are input->transform and transform->apply
			{
				std::vector<char> keys = entity_player.getComponent<client::ComponentKeyInput>().keysPressed;
				tempo::ComponentStagePosition& pos = entity_player.getComponent<tempo::ComponentStagePosition>();

				if (keys.size() > 0) {
					char key = keys[0];
					glm::ivec2 new_pos = pos.occupied[0];

					switch (key) {
					case 'w':
						new_pos.x -= 1;
						break;
					case 'a':
						new_pos.y -= 1;
						break;
					case 's':
						new_pos.x += 1;
						break;
					case 'd':
						new_pos.y += 1;
						break;
					}

					pos.occupied.pop_back();
					pos.occupied.push_back(new_pos);

					std::cout << "Key pressed: " << key << ":" << new_pos.x << ", " << new_pos.y << std::endl;
				}
			}

			system_render_scene_node.update();

			driver->beginScene(true, true);
			smgr->drawAll();
			gui_env->drawAll();

			driver->endScene();

			std::this_thread::sleep_for(std::chrono::milliseconds(33));
		}
		else {
			device->yield();
		}

	}
	printf("Left main loop\n");

	device->drop();

	return 0;
}
