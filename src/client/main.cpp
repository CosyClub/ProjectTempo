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

	/////////////////////////////////////////////////
	// Setup scene
	Ogre::SceneManager* scene = app.ogre->createSceneManager(Ogre::ST_GENERIC);
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

	// Dancefloor
	Ogre::Entity* entity_floor = scene->createEntity("meshes/floor.mesh");
	Ogre::SceneNode* node_floor = scene->getRootSceneNode()->createChildSceneNode();
	node_floor->setScale(1, 1, 1);
	node_floor->attachObject(entity_floor);


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
	Ogre::BillboardSet* Pset = scene->createBillboardSet();
	Pset->setMaterialName("rectangleSprite");
	Pset->setDefaultDimensions(0.5, 1.5);
	Pset->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
	Pset->setCommonDirection(Ogre::Vector3(0, 1, 0));

	Ogre::Billboard* player = Pset->createBillboard(0, 0.75, 0);
	player->setColour(Ogre::ColourValue::Red);
	Ogre::SceneNode* node_player = scene->getRootSceneNode()->createChildSceneNode();
	node_player->setPosition(0, 0, 0);
	node_player->attachObject(Pset);

	// Ai
	Ogre::BillboardSet* Aset = scene->createBillboardSet();
	Aset->setMaterialName("rectangleSprite");
	Aset->setDefaultDimensions(0.4, 1.3);
	Aset->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
	Aset->setCommonDirection(Ogre::Vector3(0, 1, 0));

	Ogre::Billboard* ai = Aset->createBillboard(0, 0.75, 0);
	ai->setColour(Ogre::ColourValue::Blue);
	Ogre::SceneNode* node_ai = scene->getRootSceneNode()->createChildSceneNode();
	node_ai->setPosition(3, 0, 3);
	node_ai->attachObject(Aset);

	// Sound
	tempo::Song mainsong("resources/sound/focus.ogg");
	mainsong.set_volume(30.f);

	// Clock
	tempo::Clock clock = tempo::Clock(sf::microseconds(TIME), sf::milliseconds(DELTA));
	clock.set_next_beat(sf::microseconds(TIME));
	mainsong.start();
	long offset = 0;

	// Movement hack
	srand(time(NULL));

	// Viewport
	Ogre::Viewport* vp = app.render_target->addViewport(camera);

	/////////////////////////////////////////////////
	// Main loop
	sf::Clock game_time;
	sf::Clock fps_timer;
	bool running = true;
	int frame_counter = 0;
	bool moved_this_beat = false;

	clock.sync_time(&mainsong);

	int combo = 0;
	while(running) {
		if (clock.passed_beat()) {
			/* 
			std::cout << clock.get_time().asMilliseconds() << std::endl; 
			std::cout << clock.until_beat().asMilliseconds << std::endl;
			*/

			if (moved_this_beat) 
				++combo;
			

			int dir = rand() % 2; // between 0 and 1 
			int amount = (rand() % 2) * 2 - 1; //-1 or 1 
			if (dir) { 
				node_ai->translate(amount, 0, 0); 
			} else { 
				node_ai->translate(0, 0, amount); 
			}

			moved_this_beat = false;
		}

		float seconds_until_beat = clock.until_beat().asSeconds();
		float seconds_since_beat = clock.since_beat().asSeconds();

		// Value between 0 and 1 indicating progress towards next beat, 
		// where 0 means we've just had last beat and 1 means we've just
		// hit next beat
		float beat_progress = clock.beat_progress();

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
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

				// Movement Keys/Fallthrough
				default: 
					if (!moved_this_beat && clock.within_delta()) {
						switch (e.key.keysym.sym) {

						// Arrows
						case SDLK_LEFT:  node_player->translate(-1, 0, 0); moved_this_beat = true; break;
						case SDLK_RIGHT: node_player->translate(1, 0, 0); moved_this_beat = true; break;
						case SDLK_UP:    node_player->translate(0, 0, -1); moved_this_beat = true; break;
						case SDLK_DOWN:  node_player->translate(0, 0, 1); moved_this_beat = true; break;

						// WASD
						case SDLK_a: node_player->translate(-1, 0, 0); moved_this_beat = true; break;
						case SDLK_d: node_player->translate(1, 0, 0); moved_this_beat = true; break;
						case SDLK_w: node_player->translate(0, 0, -1); moved_this_beat = true; break;
						case SDLK_s: node_player->translate(0, 0, 1); moved_this_beat = true; break;

						// HJKL
						case SDLK_h: node_player->translate(-1, 0, 0); moved_this_beat = true; break;
						case SDLK_l: node_player->translate(1, 0, 0); moved_this_beat = true; break;
						case SDLK_k: node_player->translate(0, 0, -1); moved_this_beat = true; break;
						case SDLK_j: node_player->translate(0, 0, 1); moved_this_beat = true; break;
						default: break;
						}
						//break;
					} else {
						std::cout << "Missed beat by " << std::min(clock.since_beat().asMilliseconds(), clock.until_beat().asMilliseconds()) << std::endl;
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

		Ogre::Real distance = (entity_floor->getBoundingRadius() / Ogre::Math::Sin(theta)) + nearPlane;

		// Move the camera back along its negative direction (+Z)
		camera->moveRelative(Ogre::Vector3(0, 0, 0.2*distance));

		// End of Camera move code

		float light_intensity = 2 / (exp(beat_progress));
		light->setDiffuseColour(light_intensity, light_intensity, light_intensity);

		// Ensure player is within world bounds
		Ogre::Vector3 pos = node_player->getPosition();
		if (pos.x < -7) pos.x = -7;
		if (pos.x >  7) pos.x = 7;
		if (pos.z < -7) pos.z = -7;
		if (pos.z >  7) pos.z = 7;
		node_player->setPosition(pos);

		 // AI as well
		pos = node_ai->getPosition();
		if (pos.x < -7) pos.x = -7;
		if (pos.x >  7) pos.x = 7;
		if (pos.z < -7) pos.z = -7;
		if (pos.z >  7) pos.z = 7;
		node_ai->setPosition(pos);

		app.ogre->renderOneFrame();
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
