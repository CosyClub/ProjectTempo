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

#include <SFML/System/Clock.hpp>

#include <tempo/config.hpp>
#include <tempo/Application.hpp>

int main(int argc, const char** argv){
	tempo::Application app = tempo::initialize_application("RaveCave", 800, 600);
	if(app == (tempo::Application){0}){
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
	tempo::destroy_application(app);
	printf("Exiting\n");
	return 0;
}
