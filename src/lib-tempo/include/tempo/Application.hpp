////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Application.hpp
/// \author Jamie Terry
/// \date 2017/11/06
/// \brief Contains definition of application type and related functions
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_APPLICATION_HPP
#define TEMPO_APPLICATION_HPP

#include <Ogre.h>
#include <SDL.h>
#undef main // SDL likes to define main

namespace tempo{
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

	bool operator==(const Application& a, const Application& b);
	bool operator!=(const Application& a, const Application& b);

	/////////////////////////////////////////////////////////////////////
	/// \brief Performs basic setup of the application, initialising main window
	/// and Ogre such that it renders to the window
	/////////////////////////////////////////////////////////////////////
	Application initialize_application(const char* window_title,
	                                   int window_width, int window_height);

	/////////////////////////////////////////////////////////////////////
	/// \brief Tears down application, destroying the main window and Ogre
	/// root instance
	/////////////////////////////////////////////////////////////////////
	void destroy_application(Application& app);
}

#endif
