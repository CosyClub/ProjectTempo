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
#include <tempo/entity/LevelManager.hpp>
#include <tempo/entity/GridAi.hpp>
#include <tempo/entity/PlayerInput.hpp>
#include <tempo/entity/Health.hpp>

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
	mainsong.set_volume(20.f);

	sf::SoundBuffer clickbuf;
	clickbuf.loadFromFile("resources/sound/tick.ogg");
	sf::Sound click;
	click.setBuffer(clickbuf);

	// Clock
	tempo::Clock clock = tempo::Clock(sf::microseconds(TIME), sf::milliseconds(DELTA));
	mainsong.set_volume(0.f);
	mainsong.start();
	clock.sync_time(&mainsong);
	mainsong.set_volume(20.f);
	long offset = 0;

	/////////////////////////////////////////////////
	// Setup scene

	anax::World world;
	tempo::SystemRender      system_render(app);
	Ogre::SceneManager* scene = system_render.scene;
	tempo::SystemGridAi      system_grid_ai;
	tempo::SystemPlayerInput system_player_input(clock);
	tempo::SystemHealth      system_health;
	tempo::RenderHealth      render_health;
	tempo::SystemLevelManager system_grid_motion(scene, "../bin/resources/levels/levelTest.bmp", "../bin/resources/levels/zonesTest.bmp");
	world.addSystem(system_grid_motion);
	world.addSystem(system_grid_ai);
	world.addSystem(system_render);
	world.addSystem(system_player_input);
	world.addSystem(system_health);
	world.addSystem(render_health);
	world.refresh();

	scene->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
	Ogre::SceneNode* node_light = scene->getRootSceneNode()->createChildSceneNode();
	Ogre::Light* light = scene->createLight("MainLight");
	node_light->attachObject(light);
	node_light->setPosition(20, 80, 50);

	//auto node_floor = system_grid_motion.getFloorNode();

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


	// Ogre::BillboardSet* Healthset = scene->createBillboardSet();
	// Healthset->setMaterialName("rectangleSprite");
	// Healthset->setDefaultDimensions(0.5, 0.5);
	// Healthset->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
	// Healthset->setCommonDirection(Ogre::Vector3(0, 1, 0));
	// Ogre::Billboard* health = Healthset->createBillboard(0.5, 2, 0);
	// health->setColour(Ogre::ColourValue::Green);

	entity_player.addComponent<tempo::ComponentTransform>();
	entity_player.addComponent<tempo::ComponentRender>(scene).node->attachObject(Pset);
	entity_player.getComponent<tempo::ComponentRender>().AddHealthBar(scene);
	// rend.node->attachObject(Healthset);
	entity_player.addComponent<tempo::ComponentGridPosition>(system_grid_motion.spawn());
	entity_player.addComponent<tempo::ComponentGridMotion>();
	entity_player.addComponent<tempo::ComponentPlayerInput>();
	entity_player.addComponent<tempo::ComponentHealth>(1000);
	entity_player.activate();

	//camera
	Ogre::Camera* camera = scene->createCamera("MainCamera");
	camera->setNearClipDistance(0.01f);
	camera->setAutoAspectRatio(true);
	Ogre::SceneNode *node_player;
	node_player = entity_player.getComponent<tempo::ComponentRender>().node;
	Ogre::SceneNode *node_camera = node_player->createChildSceneNode();
	node_camera->attachObject(camera);
	node_camera->setPosition(0, 20, 10);
	camera->lookAt(0,0,0);

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
	entity_ai.addComponent<tempo::ComponentRender>(scene).node->attachObject(Aset);
	entity_ai.addComponent<tempo::ComponentGridPosition>(3, 3);
	entity_ai.addComponent<tempo::ComponentGridMotion>();
	entity_ai.addComponent<tempo::ComponentGridAi>();
	entity_ai.addComponent<tempo::ComponentHealth>(700);
	entity_ai.activate();

	// Movement hack
	srand(time(NULL));

	// Viewport
	Ogre::Viewport* vp = app.render_target->addViewport(camera);

	/////////////////////////////////////////////////
	// Main loop
	sf::Clock fps_timer;
	sf::Clock dt_timer;
	sf::Time logic_time;
	sf::Time render_time;
	bool running = true;
	int frame_counter = 0;


	while(running) {
		float dt = dt_timer.getElapsedTime().asSeconds();
		dt_timer.restart();

		if (clock.passed_beat()) {
			click.play();
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

		//system_health.HealthUpdate(-5);
		render_health.HealthBarUpdate();
		//float cam_motion_delta = sin(beat_progress) * 0.3f;
		//node_camera->setPosition(sin(beat_progress-0.5)*0.1f, 8 + cam_motion_delta, 12 + cam_motion_delta);

		float light_intensity = 2 / (exp(beat_progress));
		light->setDiffuseColour(light_intensity, light_intensity, light_intensity);

		world.refresh();
		system_grid_motion.update(dt);
		logic_time = dt_timer.getElapsedTime();
		system_render.render(dt);
		render_time = dt_timer.getElapsedTime() - logic_time;
		SDL_GL_SwapWindow(app.window);

		++frame_counter;
		if(fps_timer.getElapsedTime().asSeconds() > 0.5f) {
			float seconds = fps_timer.getElapsedTime().asSeconds();
			printf("FPS: %i (%.1f% render)\n", (int)(frame_counter / seconds),
					                100 * (float)(
							render_time.asMicroseconds()
							) / (
					                logic_time.asMicroseconds() +
							render_time.asMicroseconds()));
			/* printf("Logic time  (μs): %d\n",  logic_time.asMicroseconds()); */
			/* printf("Render time (μs): %d\n", render_time.asMicroseconds()); */
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
