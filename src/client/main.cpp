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
	sf::Clock frame_timer;
	sf::Clock fps_timer;
	sf::Clock dt_timer;
	bool running = true;
	int frame_counter = 0;
	while(running){
		//float dt = dt_timer.getElapsedTime().asSeconds();
		//dt_timer.restart();

		float rot_factor = (Ogre::Real)frame_timer.getElapsedTime().asSeconds() * 0.5f;
		float cam_dist   = 10;
		float cam_x = cam_dist * sin(rot_factor);
		float cam_z = cam_dist * cos(rot_factor);

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
			case SDL_KEYUP:
				switch(e.key.keysym.sym){
				case SDLK_r:
					light->setDiffuseColour(1,0,0);
					break;
				case SDLK_g:
					light->setDiffuseColour(0,1,0);
					break;
				case SDLK_b:
					light->setDiffuseColour(0,0,1);
					break;
				case SDLK_w:
					light->setDiffuseColour(1,1,1);
					break;
				}
				break;
			default: break;
			}
		}

		node_camera->setPosition(cam_x, 5, cam_z);
		//node_camera->setPosition(10, 5, 10);
		camera->lookAt(0,0,0);

		long t = clock.getElapsedTime().asMicroseconds();
		if (t > TIME - offset){
			//std::cout << offset << std::endl;
			offset = t - (TIME - offset);
			tick.play();
			clock.restart();
			int dir = rand() % 2; // between 0 and 1
			int amount = (rand() % 2) * 2 - 1; //-1 or 1
			if (dir){
				node_player->translate(amount, 0, 0);
			}
			else{
				node_player->translate(0, 0, amount);
			}
			Ogre::Vector3 pos = node_player->getPosition();
			if (pos.x < -5) pos.x = -4.5;
			if (pos.x >  5) pos.x =  4.5;
			if (pos.z < -5) pos.z = -4.5;
			if (pos.z >  5) pos.z =  4.5;
			node_player->setPosition(pos);
		}

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
