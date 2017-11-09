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

#include <tempo/Application.hpp>

#define BPM 174
#define DELTA 150
#define FDELTA DELTA / 1000.0f
#define NETOFFSET 30 * 1000
#define SIZE 1000
#define GRID 100
#define TIME 60000000 / BPM
#define LOWERBOUND (DELTA * 1000)
#define UPPERBOUND TIME - (DELTA * 1000)

int main(int argc, const char** argv){
	tempo::Application app = tempo::initialize_application("RaveCave", 800, 600);
	if(app.ogre == nullptr || app.window == nullptr || app.render_target == nullptr){
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
	node_camera->setPosition(0, 0, 140);

	//Dancefloor
	Ogre::Entity* entity_floor = scene->createEntity("meshes/floor.mesh");
	Ogre::SceneNode* node_floor = scene->getRootSceneNode()->createChildSceneNode();
	node_floor->setScale(10, 0.05, 10);
	node_floor->attachObject(entity_floor);


	//Dummy objects
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

	//Player
	Ogre::BillboardSet* Bset = scene->createBillboardSet();
	Bset->setMaterialName("rectangleSprite");
	Bset->setDefaultDimensions(0.5, 1.5);
	Bset->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
	Bset->setCommonDirection(Ogre::Vector3(0, 1, 0));
	Ogre::Billboard* player = Bset->createBillboard(0, 0.75, 0);
	//player->setRotation(Ogre::Radian(M_PI / 4));
	player->setColour(Ogre::ColourValue::Red);

	Ogre::SceneNode* node_player = scene->getRootSceneNode()->createChildSceneNode();
	node_player->setPosition(0.5, 0, 0.5);
	node_player->attachObject(Bset);

	//Sound
	sf::SoundBuffer tickbuffer;
	sf::SoundBuffer songbuffer;
	tickbuffer.loadFromFile("resources/sound/tick.ogg");
	songbuffer.loadFromFile("resources/sound/focus.ogg");
	sf::Sound tick;
	sf::Sound song;
	tick.setBuffer(tickbuffer);
	song.setBuffer(songbuffer);

	//Clock
	sf::Clock clock;
	song.play();
	long offset = 0;

	//Movement hack
	srand(time(NULL));

	//Viewport
	Ogre::Viewport* vp = app.render_target->addViewport(camera);

	/////////////////////////////////////////////////
	// Main loop
	sf::Clock game_time;
	sf::Clock fps_timer;
	bool running = true;
	int frame_counter = 0;
	bool moved_this_beat = false;

	int combo = 0;
	while(running){
		long t = clock.getElapsedTime().asMicroseconds();
		if (t > TIME - offset){
			offset = t - (TIME - offset);
			tick.play();
			clock.restart();

			if(moved_this_beat){
				++combo;
			}

			moved_this_beat = false;
		}

		float seconds_until_beat = (TIME - t) / 1000000.0f;
		float seconds_since_beat = (TIME / 1000000.0f) - seconds_until_beat;

		// Value between 0 and 1 indicating progress towards next beat, where 0 means we've
		// just had last beat and 1 means we've just hit next beat
		float beat_progress = (seconds_since_beat / (TIME / 1000000.0f));

		//printf("Time till next tick: %8f, since last: %8f, beat progress: %8f\n",
		//       seconds_until_beat, seconds_since_beat, beat_progress);

		SDL_Event e;
		while(SDL_PollEvent(&e)){
			switch(e.type){
			case SDL_WINDOWEVENT:
				switch(e.window.event){
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
				if(!moved_this_beat && (seconds_until_beat < FDELTA || seconds_since_beat < FDELTA)){
					switch(e.key.keysym.sym){
					case SDLK_LEFT:  node_player->translate(-1, 0,  0); moved_this_beat = true; break;
					case SDLK_RIGHT: node_player->translate( 1, 0,  0); moved_this_beat = true; break;
					case SDLK_UP:    node_player->translate( 0, 0, -1); moved_this_beat = true; break;
					case SDLK_DOWN:  node_player->translate( 0, 0,  1); moved_this_beat = true; break;
					default: break;
					}
					break;
				}
                                else{
                                        float miss = std::min(seconds_until_beat - FDELTA, seconds_since_beat + FDELTA);
                                        std::cout << "Missed beat by " << miss << " Seconds" << std::endl;
                                }
			}
		}

		float cam_motion_delta = sin(beat_progress) * 0.3f;
		node_camera->setPosition(sin(beat_progress-0.5)*0.1f, 8 + cam_motion_delta, 12 + cam_motion_delta);
		camera->lookAt(0,0,0);

		float light_intensity = 2 / (exp(beat_progress));
		light->setDiffuseColour(light_intensity, light_intensity, light_intensity);

		// Ensure player is within world bounds
		Ogre::Vector3 pos = node_player->getPosition();
		if (pos.x < -5) pos.x = -4.5;
		if (pos.x >  5) pos.x =  4.5;
		if (pos.z < -5) pos.z = -4.5;
		if (pos.z >  5) pos.z =  4.5;
		node_player->setPosition(pos);

		app.ogre->renderOneFrame();
		SDL_GL_SwapWindow(app.window);

		++frame_counter;
		if(fps_timer.getElapsedTime().asSeconds() > 0.5f){
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
