////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Application.cpp
/// \author Jamie Terry
/// \date 2017/11/06
/// \brief Contains definitions of functions declared in Application.hpp
////////////////////////////////////////////////////////////////////////////

#include <OgreGL3PlusPlugin.h>
#include <OgreGLPlugin.h>
#include <OgreGLRenderSystem.h>

#include <tempo/Config.hpp>
#include <tempo/Application.hpp>

#include <SDL_syswm.h> 

namespace tempo{
	bool operator==(const Application& a, const Application& b){
		return
			a.ogre          == b.ogre   &&
			a.window        == b.window &&
			a.render_target == b.render_target;
	}

	bool operator!=(const Application& a, const Application& b){ return !(a == b); }

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
		
#ifdef TEMPO_OS_WINDOWS
		//Windows always has to be the special one...

		app.render_target = app.ogre->createRenderWindow(window_title,
			window_width, window_height,
			false
		);

		void* window_handle;
		app.render_target->getCustomAttribute("WINDOW", &window_handle);

		app.window = SDL_CreateWindowFrom(window_handle);

#else

		printf("Creating window...\n");
		app.window = SDL_CreateWindow(window_title,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			window_width, window_height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
		);
		if (app.window == NULL) {
			printf("Failed initialise SDL window, error: %s\n", SDL_GetError());
			return { 0 };
		}

		app.gl_context = SDL_GL_CreateContext(app.window);
		SDL_GL_MakeCurrent(app.window, app.gl_context);

		Ogre::NameValuePairList window_options;

		window_options["currentGLContext"] = "true";

		//window_options["vsync"] = "true";
		app.render_target = app.ogre->createRenderWindow(window_title,
			window_width, window_height,
			false, &window_options
		);
#endif

		

		app.render_target->setVisible(true);

		return app;
	}

	void destroy_application(Application& app){
		SDL_DestroyWindow(app.window);
		delete app.ogre;
		app = {0};
	}
}
