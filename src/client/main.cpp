////////////////////////////////////////////////////////////////////////////
/// \file main.cpp
/// \author Jamie Terry
/// \date 2017/10/30
/// \brief Contains entry point for Ogre3d Demo
////////////////////////////////////////////////////////////////////////////

// Minimal application code adapted from:
// http://wiki.ogre3d.org/MinimalApplication

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>

#include <Ogre.h>
#include <OgreGL3PlusPlugin.h>
#include <OgreGLPlugin.h>
#include <OgreGLRenderSystem.h>

#include <SFML/Audio.hpp>

#define BPM 174
#define DELTA 150
#define NETOFFSET 30 * 1000
#define SIZE 1000
#define GRID 100
#define TIME 60000000 / BPM
#define LOWERBOUND (DELTA * 1000)
#define UPPERBOUND TIME - (DELTA * 1000)

int main(int argc, const char** argv){

	/////////////////////////////////////////////////
	// Create Ogre Root and add plugins
	printf("Creating Ogre Root\n");
	Ogre::Root* root = new Ogre::Root();
	//root->installPlugin(new Ogre::GL3PlusPlugin());
	//root->installPlugin(new Ogre::GLPlugin());
	Ogre::GLRenderSystem* renderer_gl = new Ogre::GLRenderSystem();
	root->addRenderSystem(renderer_gl);
	root->setRenderSystem(renderer_gl);

	// Now plugins are registered and engine is configured, do initialization
	root->initialise(false);

	printf("Initialised Ogre, starting application code...\n");

	/////////////////////////////////////////////////
	// Setup resources
	Ogre::ResourceGroupManager& resources = Ogre::ResourceGroupManager::getSingleton();
	resources.addResourceLocation("resources", "FileSystem",
	                              Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	                              true);
	resources.initialiseAllResourceGroups();

	/////////////////////////////////////////////////
	// Setup window and scene
	Ogre::NameValuePairList window_options;
	//window_options["vsync"] = "true";
	Ogre::RenderWindow* app   = root->createRenderWindow("OgreDemo", 800, 600, false, &window_options);
	Ogre::SceneManager* scene = root->createSceneManager(Ogre::ST_GENERIC);

	scene->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
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
	Ogre::Viewport* vp   = app->addViewport(camera);

	/////////////////////////////////////////////////
	// Setup input handling


	/////////////////////////////////////////////////
	// Start game, will block until frame listener returns false
	int frame_counter = 0;
	Ogre::Timer frame_timer;
	Ogre::Timer fps_timer;
	while(!app->isClosed()){
		float rot_factor = (Ogre::Real)frame_timer.getMicroseconds() / 6000000.0f;
		float cam_dist   = 10;
		float cam_x = cam_dist * sin(rot_factor);
		float cam_z = cam_dist * cos(rot_factor);

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
                }

		root->renderOneFrame();

		++frame_counter;
		if(fps_timer.getMicroseconds() > 500000){
			float seconds = fps_timer.getMicroseconds() / 1000000.0f;
			printf("FPS: %i\n", (int)(frame_counter / seconds));
			fps_timer.reset();
			frame_counter = 0;
		}
	}

	printf("Cleaning up...\n");
	delete root;
	printf("Exiting\n");
	return 0;
}
