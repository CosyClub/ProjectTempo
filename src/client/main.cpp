////////////////////////////////////////////////////////////////////////////
/// \file main.cpp
/// \author Jamie Terry
/// \date 2017/10/30
/// \brief Contains entry point for Ogre3d Demo
////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>

#include <Ogre.h>

#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Network.hpp>

#include <tempo/Application.hpp>
#include <tempo/time.hpp>
#include <tempo/song.hpp>
#include <tempo/entity/Transform.hpp>
#include <tempo/entity/Render.hpp>
#include <tempo/entity/GridMotion.hpp>
#include <tempo/entity/GridAi.hpp>
#include <tempo/entity/PlayerInput.hpp>
#include <tempo/LevelManager.hpp>

#include <anax/World.hpp>
#include <anax/Entity.hpp>

#define NET_PORT 1337 // Port the server is running on
#define BPM 174
#define DELTA 150
#define TIME 60000000 / BPM

int main(int argc, const char** argv)
{
	tempo::Application app = tempo::initialize_application("RaveCave", 800, 600);
	if (app.ogre == nullptr || app.window == nullptr || app.render_target == nullptr) {
		printf("Application initialisation failed, exiting\n");
		return 1;
	}

	/////////////////////////////////////////////////
	// Setup resources
	Ogre::ResourceGroupManager& resources = Ogre::ResourceGroupManager::getSingleton();
	resources.addResourceLocation("resources", "FileSystem",
	                              Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	                              true);
	resources.initialiseAllResourceGroups();

	// Sound
	tempo::Song mainsong("resources/sound/focus.ogg");
	mainsong.set_volume(30.f);

	// Clock
	tempo::Clock clock = tempo::Clock(sf::microseconds(TIME), sf::milliseconds(DELTA));
	clock.set_next_beat(sf::microseconds(TIME));
	mainsong.start();
	long offset = 0;

	/////////////////////////////////////////////////
	// Setup scene
	anax::World world;
	tempo::SystemGridMotion  system_grid_motion(-7, -7, 7, 7);
	tempo::SystemGridAi      system_grid_ai;
	tempo::SystemPlayerInput system_player_input(clock);
	tempo::SystemRender      system_render(app);
	world.addSystem(system_grid_motion);
	world.addSystem(system_grid_ai);
	world.addSystem(system_render);
	world.addSystem(system_player_input);
	world.refresh();

	Ogre::SceneManager* scene = system_render.scene;
	scene->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
	Ogre::SceneNode* node_light = scene->getRootSceneNode()->createChildSceneNode();
	Ogre::Light* light = scene->createLight("MainLight");
	node_light->attachObject(light);
	node_light->setPosition(20, 80, 50);

	Ogre::Camera* camera = scene->createCamera("MainCamera");
	camera->setNearClipDistance(0.01f);
	camera->setAutoAspectRatio(true);
	Ogre::SceneNode* node_camera = scene->getRootSceneNode()->createChildSceneNode();
	node_camera->attachObject(camera);
	node_camera->setPosition(0, 0, 30);


	LevelManager* new_floor = new LevelManager(scene, 7);
	new_floor->deleteTile(scene, {0,0});
	new_floor->deleteTile(scene, {3,3});
	new_floor->setHeight(5, {2,2});
	new_floor->setHeight(0.1, {2,2});
	new_floor->setMaterial("BrownWalls", {2,2});

	auto node_floor = new_floor->getFloorNode();

	// Dancefloor
	// anax::Entity entity_floor = world.createEntity();
	// Ogre::Entity* mesh_floor = scene->createEntity("meshes/floor.mesh");
	// entity_floor.addComponent<tempo::ComponentTransform>(0.0f, 0.0f, 0.0f);
	// Ogre::SceneNode* node_floor = entity_floor.addComponent<tempo::ComponentRender>(scene).node;
	// node_floor->setScale(1, 1, 1);
	// node_floor->attachObject(mesh_floor);
	// entity_floor.activate();


	// Dummy objects
	Ogre::Entity* x1 = scene->createEntity("x1", Ogre::SceneManager::PT_SPHERE);
	//x1->setPosition(1, 0, 0);
	//y1->setPosition(0, 1, 0);
	//z1->setPosition(0, 0, 1);
	Ogre::Entity* y1 = scene->createEntity("y1", Ogre::SceneManager::PT_SPHERE);
	Ogre::Entity* z1 = scene->createEntity("z1", Ogre::SceneManager::PT_SPHERE);
	Ogre::SceneNode* helpers = scene->getRootSceneNode()->createChildSceneNode();
	helpers->attachObject(x1);
	helpers->attachObject(y1);
	helpers->attachObject(z1);

	// Player
	anax::Entity entity_player = world.createEntity();
	Ogre::BillboardSet* Pset = scene->createBillboardSet();
	Pset->setMaterialName("rectangleSprite");
	Pset->setDefaultDimensions(0.5, 1.5);
	Pset->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
	Pset->setCommonDirection(Ogre::Vector3(0, 1, 0));
	Ogre::Billboard* player = Pset->createBillboard(0, 0.75, 0);
	player->setColour(Ogre::ColourValue::Red);
	entity_player.addComponent<tempo::ComponentTransform>();
	entity_player.addComponent<tempo::ComponentRender>(scene).node->attachObject(Pset);
	entity_player.addComponent<tempo::ComponentGridMotion>(0.0f, 0.0f);
	entity_player.addComponent<tempo::ComponentPlayerInput>();
	entity_player.activate();

	// Ai
	anax::Entity entity_ai = world.createEntity();
	Ogre::BillboardSet* Aset = scene->createBillboardSet();
	Aset->setMaterialName("rectangleSprite");
	Aset->setDefaultDimensions(0.4, 1.3);
	Aset->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
	Aset->setCommonDirection(Ogre::Vector3(0, 1, 0));
	Ogre::Billboard* ai = Aset->createBillboard(0, 0.75, 0);
	ai->setColour(Ogre::ColourValue::Blue);
	entity_ai.addComponent<tempo::ComponentTransform>();
	entity_ai.addComponent<tempo::ComponentRender>(scene).node->attachObject(Aset);
	entity_ai.addComponent<tempo::ComponentGridMotion>(3.0f, 3.0f);
	entity_ai.addComponent<tempo::ComponentGridAi>();
	entity_ai.activate();

	// Movement hack
	srand(time(NULL));

	// Viewport
	Ogre::Viewport* vp = app.render_target->addViewport(camera);

	/////////////////////////////////////////////////
	// Main loop
	sf::Clock fps_timer;
	sf::Clock dt_timer;
	bool running = true;
	int frame_counter = 0;

	clock.sync_time(&mainsong);

	while(running) {
		float dt = dt_timer.getElapsedTime().asSeconds();
		dt_timer.restart();

		if (clock.passed_beat()) {
			/*
			std::cout << clock.get_time().asMilliseconds() << std::endl;
			std::cout << clock.until_beat().asMilliseconds << std::endl;
			*/

			system_grid_ai.update();

			system_player_input.advanceBeat();
		}

		float seconds_until_beat = clock.until_beat().asSeconds();
		float seconds_since_beat = clock.since_beat().asSeconds();

		// Value between 0 and 1 indicating progress towards next beat,
		// where 0 means we've just had last beat and 1 means we've just
		// hit next beat
		float beat_progress = clock.beat_progress();

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if(!system_player_input.handleInput(e)){
				switch (e.type) {
				case SDL_WINDOWEVENT:
					switch (e.window.event) {
					case SDL_WINDOWEVENT_CLOSE:
						running = false;
						break;
					case SDL_WINDOWEVENT_RESIZED:
						app.render_target->resize(e.window.data1, e.window.data2);
						break;
					default: break;
					}
					break;
				case SDL_KEYDOWN:
					// Non movement keys first
					switch (e.key.keysym.sym) {
						// Volume Controls (left and right square brackets)
					case SDLK_LEFTBRACKET:  mainsong.dec_volume(10.f); break;
					case SDLK_RIGHTBRACKET: mainsong.inc_volume(10.f); break;
					default: break;
					}
				}
			}
		}

		float cam_motion_delta = sin(beat_progress) * 0.3f;
		node_camera->setPosition(sin(beat_progress-0.5)*0.1f, 8 + cam_motion_delta, 12 + cam_motion_delta);
		camera->lookAt(0,0,0);

		// This code moves the camera to a point where the entire floor is visible.
		camera->setPosition(node_floor->getParentSceneNode()->_getDerivedPosition());

		Ogre::Real nearPlane = camera->getNearClipDistance();
		Ogre::Radian theta = camera->getFOVy() * .5;
		// Get minimum dimension from aspect
		Ogre::Real aspectRatio = camera->getAspectRatio();
		if (aspectRatio < 1.0f)
			theta *= aspectRatio;

	//	Ogre::Real distance = (mesh_floor->getBoundingRadius() / Ogre::Math::Sin(theta)) + nearPlane;

		// Move the camera back along its negative direction (+Z)
		camera->moveRelative(Ogre::Vector3(0, 0, 0.2));

		// End of Camera move code

		float light_intensity = 2 / (exp(beat_progress));
		light->setDiffuseColour(light_intensity, light_intensity, light_intensity);

		world.refresh();
		system_grid_motion.update(dt);
		system_render.render(dt);
		SDL_GL_SwapWindow(app.window);

		++frame_counter;
		if(fps_timer.getElapsedTime().asSeconds() > 0.5f) {
			float seconds = fps_timer.getElapsedTime().asSeconds();
			printf("FPS: %i\n", (int)(frame_counter / seconds));
			fps_timer.restart();
			frame_counter = 0;
		}
	}

	/////////////////////////////////////////////////
	// Shutdown
	printf("Cleaning up...\n");
	tempo::destroy_application(app);
	printf("Exiting\n");
	return 0;
}
