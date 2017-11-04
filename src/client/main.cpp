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

#include <Ogre.h>
#include <OgreGL3PlusPlugin.h>
#include <OgreGLPlugin.h>
#include <OgreGLRenderSystem.h>

#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

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

	// Now plugins are registered and engine is configured, do initialisation
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
	// Setup window
	int window_width  = 800;
	int window_height = 600;
	sf::ContextSettings sf_settings;
	sf_settings.depthBits         = 24;
	sf_settings.antialiasingLevel =  4;
	sf_settings.stencilBits       =  8;
	sf::RenderWindow sfapp(sf::VideoMode(window_width,window_height,32),
	                       "RaveCave", sf::Style::Default, sf_settings);
	Ogre::NameValuePairList window_options;
	//window_options["vsync"] = "true";
	window_options["externalWindowHandle"] = Ogre::StringConverter::toString(sfapp.getSystemHandle());
	window_options["currentGlContext"] = "True";
Ogre::RenderWindow* app   = root->createRenderWindow("RaveCave", window_width, window_height,
                                                     false, &window_options);
	app->setVisible(true);

	/////////////////////////////////////////////////
	// Setup scene
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

	Ogre::Entity* entity_ogre = scene->createEntity("meshes/ogrehead.mesh");
	Ogre::SceneNode* node_ogre = scene->getRootSceneNode()->createChildSceneNode();
	node_ogre->attachObject(entity_ogre);

	Ogre::Viewport* vp   = app->addViewport(camera);

	/////////////////////////////////////////////////
	// Startup main loop
	int frame_counter = 0;
	sf::Clock frame_timer;
	sf::Clock fps_timer;
	while(sfapp.isOpen()){
		sf::Event e;
		while(sfapp.pollEvent(e)){
			switch(e.type){
			case sf::Event::Closed:
				sfapp.close();
				break;
			default: break;
			}
		}

		float rot_factor = frame_timer.getElapsedTime().asSeconds() * 1.5f;
		float cam_dist   = 150 + (100 * sin(rot_factor / 3.0f));
		float cam_x = cam_dist * sin(rot_factor);
		float cam_z = cam_dist * cos(rot_factor);

		node_camera->setPosition(cam_x, 0, cam_z);
		camera->lookAt(0,0,0);

		root->renderOneFrame();

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
	delete root;
	printf("Exiting\n");
	return 0;
}
