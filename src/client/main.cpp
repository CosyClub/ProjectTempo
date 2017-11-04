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

#include <Ogre.h>
#include <OgreGL3PlusPlugin.h>
#include <OgreGLPlugin.h>
#include <OgreGLRenderSystem.h>

#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>

/////////////////////////////////////////////////////////////////////
/// \brief Struct containing top level objects related to the running of
/// the application
/////////////////////////////////////////////////////////////////////
struct Application{
	/// \brief Root Ogre object, access point to much of the Ogre API
	Ogre::Root* ogre;

	/// \brief Main application window to which we are rendering
	sf::Window* window;

	/// \brief Render target to which Ogre is directing its rendering commands
	/// Ogre names this badly... the RenderWindow is really wrapping the OpenGL
	/// context created by SFML for its window
	Ogre::RenderWindow* render_target;
};

/////////////////////////////////////////////////////////////////////
/// \brief Performs basic setup of the application, initialising main window
/// and Ogre such that it renders to the window
/////////////////////////////////////////////////////////////////////
Application initialize_application(const char* window_title,
                                   int window_width, int window_height,
                                   int window_style = sf::Style::Default){
	Application app = {0};

	/////////////////////////////////////////////////
	// Create Ogre Root and add plugins
  printf("Creating Ogre Root\n");
  app.ogre = new Ogre::Root();
	//root->installPlugin(new Ogre::GL3PlusPlugin());
	//root->installPlugin(new Ogre::GLPlugin());
	Ogre::GLRenderSystem* renderer_gl = new Ogre::GLRenderSystem();
	app.ogre->addRenderSystem(renderer_gl);
	app.ogre->setRenderSystem(renderer_gl);

	// Now plugins are registered and engine is configured, do initialisation
	app.ogre->initialise(false);

	/////////////////////////////////////////////////
	// Setup window
	sf::ContextSettings sf_settings;
	sf_settings.depthBits         = 24;
	sf_settings.antialiasingLevel =  4;
	sf_settings.stencilBits       =  8;
	app.window = new sf::Window(sf::VideoMode(window_width,window_height,32),
	                            window_title, window_style, sf_settings);

	Ogre::NameValuePairList window_options;
	//window_options["vsync"] = "true";
	window_options["externalWindowHandle"] = Ogre::StringConverter::toString(
		   app.window->getSystemHandle()
	);
	window_options["currentGlContext"] = "True";
	app.render_target = app.ogre->createRenderWindow(window_title,
	                                                 window_width, window_height,
	                                                 false, &window_options
	                                                 );
	app.render_target->setVisible(true);

	return app;
}

void destroy_application(Application& app){
	delete app.window;
	delete app.ogre;
	app = {0};
}

int main(int argc, const char** argv){
	Application app = initialize_application("RaveCave", 800, 600);

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

	Ogre::Viewport* vp   = app.render_target->addViewport(camera);

	/////////////////////////////////////////////////
	// Main loop
	int frame_counter = 0;
	sf::Clock frame_timer;
	sf::Clock fps_timer;
	while(app.window->isOpen()){
		sf::Event e;
		while(app.window->pollEvent(e)){
			switch(e.type){
			case sf::Event::Closed:
				app.window->close();
				break;
			case sf::Event::Resized:
				app.render_target->resize(e.size.width, e.size.height);
			default: break;
			}
		}

		float rot_factor = frame_timer.getElapsedTime().asSeconds() * 1.5f;
		float cam_dist   = 150 + (100 * sin(rot_factor / 3.0f));
		float cam_x = cam_dist * sin(rot_factor);
		float cam_z = cam_dist * cos(rot_factor);

		node_camera->setPosition(cam_x, 0, cam_z);
		camera->lookAt(0,0,0);

		app.ogre->renderOneFrame();

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
	destroy_application(app);
	printf("Exiting\n");
	return 0;
}
