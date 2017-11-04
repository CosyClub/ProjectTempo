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

#include <SDL.h>

#include <tempo/config.hpp>

/////////////////////////////////////////////////////////////////////
/// \brief Struct containing top level objects related to the running of
/// the application
/////////////////////////////////////////////////////////////////////
struct Application{
	/// \brief Root Ogre object, access point to much of the Ogre API
	Ogre::Root* ogre;

	/// \brief Main application window to which we are rendering
	SDL_Window* window;

	/// \brief The OpenGL context for the main application window
	SDL_GLContext gl_context;

	/// \brief Render target to which Ogre is directing its rendering commands
	/// Ogre names this badly... the RenderWindow is really wrapping the OpenGL
	/// context created by SFML for its window
	Ogre::RenderWindow* render_target;
};

bool operator==(const Application& a, const Application& b){
	return
		a.ogre          == b.ogre   &&
		a.window        == b.window &&
		a.render_target == b.render_target;
}

/////////////////////////////////////////////////////////////////////
/// \brief Performs basic setup of the application, initialising main window
/// and Ogre such that it renders to the window
/////////////////////////////////////////////////////////////////////
Application initialize_application(const char* window_title,
                                   int window_width, int window_height){
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
	printf("Initialising SDL...\n");
	if(!SDL_WasInit(SDL_INIT_VIDEO) && SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("Failed to initialize SDL, error: %s\n", SDL_GetError());
		return {0};
	}
	printf("Creating window...\n");
	app.window = SDL_CreateWindow(window_title,
	                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                              window_width, window_height,
	                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	                             );
	if(app.window == NULL){
		printf("Failed initialise SDL window, error: %s\n", SDL_GetError());
		return {0};
	}

	app.gl_context = SDL_GL_CreateContext(app.window);
	SDL_GL_MakeCurrent(app.window, app.gl_context);

	Ogre::NameValuePairList window_options;
	#ifdef TEMPO_OS_WINDOWS
	//Windows always has to be the special one...
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWMInfo(&wmInfo);

	size_t winHandle = reinterpret_cast<size_t>(wmInfo.window);
	size_t winGlContext = reinterpret_cast<size_t>(wmInfo.hglrc);

	window_options["externalWindowHandle"] = StringConverter::toString(winHandle);
	window_options["externalGLContext"   ] = StringConverter::toString(winGlContext);
	#else
	window_options["currentGLContext"] = "true";
	#endif

	//window_options["vsync"] = "true";
	app.render_target = app.ogre->createRenderWindow(window_title,
	                                                 window_width, window_height,
	                                                 false, &window_options
	                                                );
	app.render_target->setVisible(true);

	return app;
}

void destroy_application(Application& app){
	SDL_DestroyWindow(app.window);
	delete app.ogre;
	app = {0};
}

int main(int argc, const char** argv){
	Application app = initialize_application("RaveCave", 800, 600);
	if(app == (Application){0}){
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

	Ogre::Entity* entity_ogre = scene->createEntity("meshes/ogrehead.mesh");
	Ogre::SceneNode* node_ogre = scene->getRootSceneNode()->createChildSceneNode();
	node_ogre->attachObject(entity_ogre);

	Ogre::Viewport* vp   = app.render_target->addViewport(camera);

	/////////////////////////////////////////////////
	// Main loop
	int frame_counter = 0;
	sf::Clock frame_timer;
	sf::Clock fps_timer;
	sf::Clock dt_timer;
	bool running = true;
	while(running){
		float dt = dt_timer.getElapsedTime().asSeconds();
		dt_timer.restart();
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

		float rot_factor = frame_timer.getElapsedTime().asSeconds() * 2.0f;
		float light_x = 200 * sin(rot_factor);
		float light_z = 200 * cos(rot_factor);

		node_light->setPosition(light_x, 0, light_z);

		node_ogre->rotate(Ogre::Vector3(0,1,0), Ogre::Radian(dt * 1.0f));

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
	destroy_application(app);
	printf("Exiting\n");
	return 0;
}
