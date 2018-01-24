////////////////////////////////////////////////////////////////////////////
/// main.cpp
///
/// Contains entry point for Ogre3d Demo
////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>
#include <thread>

#include <Ogre.h>

#include <tempo/Application.hpp>
#include <tempo/network/client.hpp>
#include <tempo/song.hpp>
#include <tempo/time.hpp>
#include <tempo/song.hpp>
#include <tempo/entity/EntityCreation.hpp>
#include <tempo/entity/Transform.hpp>
#include <tempo/entity/Render.hpp>
#include <tempo/entity/LevelManager.hpp>
#include <tempo/entity/LevelRenderer.hpp>
#include <tempo/entity/GridAi.hpp>
#include <tempo/entity/PlayerLocal.hpp>
#include <tempo/entity/Health.hpp>


#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>

#include <anax/World.hpp>
#include <anax/Entity.hpp>

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

	// Sound
	tempo::Song mainsong("resources/sound/focus.ogg");
	mainsong.set_volume(20.f);

	sf::SoundBuffer clickbuf;
	clickbuf.loadFromFile("resources/sound/tick.ogg");
	sf::Sound click;
	click.setBuffer(clickbuf);

	/////////////////////////////////////////////////
	// Networking

	// Set up remote address, local ports and remote handshake port
	// Note, IF statement is to change ports for local development, bit
	// hacky and should be removed in due course!
	sf::IpAddress addr_r = DEFAULT_ADDR;
	if (addr_r == DEFAULT_ADDR) {
		tempo::port_ci = DEFAULT_PORT_IN+10;
		tempo::port_co = DEFAULT_PORT_OUT+10;
	} else {
		tempo::port_ci = DEFAULT_PORT_IN;
		tempo::port_co = DEFAULT_PORT_OUT;
	}
	tempo::port_sh = DEFAULT_PORT_HS;
	// Other server ports aquired dynamically on handshake

	// Bind sockets
	tempo::bindSocket('i', tempo::port_ci);
	tempo::bindSocket('o', tempo::port_co);

	// Start Listener Thread to catch server updates after connecting
	std::thread listener (tempo::listenForServerUpdates);

	// Establish role
	tempo::ClientRole role = tempo::ClientRole::PLAYER;
	tempo::ClientRoleData roleData = {"Bilbo Baggins"};

	// Connect to server and handshake information
	tempo::connectToAndSyncWithServer(role, roleData);

	// Clock & Time Sync Song
	tempo::Clock clock = tempo::Clock(sf::microseconds(TIME), sf::milliseconds(DELTA));
	mainsong.set_volume(0.f);
	mainsong.start();
	clock.sync_time(&mainsong);
	mainsong.set_volume(20.f);
	long offset = 0;

	/////////////////////////////////////////////////
	// Setup scene
	anax::World world;
	tempo::SystemRender      system_render(app);
	Ogre::SceneManager* scene = system_render.scene;
	tempo::SystemLevelManager system_level(world,
	                                       "../bin/resources/levels/levelTest.bmp",
	                                       "../bin/resources/levels/zonesTest.bmp"
	                                      );
	tempo::SystemGridAi       system_grid_ai;
	tempo::SystemPlayerLocal  system_player_local(clock);
	tempo::SystemHealth       system_health;
	tempo::RenderHealth       render_health;
	
	world.addSystem(system_level);
	world.addSystem(system_grid_ai);
	world.addSystem(system_render);
	world.addSystem(system_player_local);
	world.addSystem(system_health);
	world.addSystem(render_health);
	world.refresh();

	tempo::LevelRenderer level_renderer(scene, scene->getRootSceneNode(), &system_level);

	scene->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
	Ogre::SceneNode* node_light = scene->getRootSceneNode()->createChildSceneNode();
	Ogre::Light* light = scene->createLight("MainLight");
	node_light->attachObject(light);
	node_light->setPosition(20, 80, 50);

	//auto node_floor = system_grid_motion.getFloorNode();

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

	//testing
	/* tempo::EntityCreationData* entitytest = tempo::newEntity(1, {2,2}); */

	// Player
	anax::Entity entity_player = tempo::newPlayer(world, scene, 0, tempo::EID_PLAYER, system_level);

	// Ai
	anax::Entity entity_ai = tempo::newAI(world,scene, 1, tempo::EID_AI, 5, 5);

	//Destroyables
	anax::Entity entity_destroyable = tempo::newDestroyable(world,scene, 2, tempo::EID_DES, 2, 2, "Cube");

	//NonDestroyables
	anax::Entity entity_nondestroyable = tempo::newNonDestroyable(world,scene, 3, tempo::EID_NONDES, 5, 5, "Cube");

	//camera
	Ogre::Camera* camera = scene->createCamera("MainCamera");
	camera->setNearClipDistance(0.01f);
	camera->setAutoAspectRatio(true);
	Ogre::SceneNode *node_player;
	node_player = entity_player.getComponent<tempo::ComponentRender>().node;
	Ogre::SceneNode *node_camera = node_player->createChildSceneNode();
	node_camera->attachObject(camera);
	node_camera->setPosition(0, 20, 10);
	camera->lookAt(0, 0, 0);

	// Movement hack
	srand(time(NULL));

	// Viewport
	Ogre::Viewport* vp = app.render_target->addViewport(camera);

	/////////////////////////////////////////////////
	// Main loop
	sf::Clock fps_timer;
	sf::Clock dt_timer;
	sf::Time logic_time;
	sf::Time render_time;
	bool running = true;
	int frame_counter = 0;


	while (running) {
		float dt = dt_timer.getElapsedTime().asSeconds();
		dt_timer.restart();

		if (clock.passed_beat()) {
			click.play();
			/*
			std::cout << clock.get_time().asMilliseconds() << std::endl;
			std::cout << clock.until_beat().asMilliseconds << std::endl;
			*/

			system_grid_ai.update();

			system_player_local.advanceBeat();
		}

		float seconds_until_beat = clock.until_beat().asSeconds();
		float seconds_since_beat = clock.since_beat().asSeconds();

		// Value between 0 and 1 indicating progress towards next beat,
		// where 0 means we've just had last beat and 1 means we've just
		// hit next beat
		float beat_progress = clock.beat_progress();

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if(!system_player_local.handleInput(e)){
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
					default: break;
					}
				}
			}
		}


		render_health.HealthBarUpdate();
		system_health.CheckHealth();

		//float cam_motion_delta = sin(beat_progress) * 0.3f;
		//node_camera->setPosition(sin(beat_progress-0.5)*0.1f, 8 + cam_motion_delta, 12 + cam_motion_delta);

		float light_intensity = 2 / (exp(beat_progress));
		light->setDiffuseColour(light_intensity, light_intensity, light_intensity);

		world.refresh();
		system_level.update(dt);
		logic_time = dt_timer.getElapsedTime();
		system_render.render(dt);
		render_time = dt_timer.getElapsedTime() - logic_time;
		SDL_GL_SwapWindow(app.window);

		++frame_counter;
		if (fps_timer.getElapsedTime().asSeconds() > 0.5f) {
			float seconds = fps_timer.getElapsedTime().asSeconds();
			printf("FPS: %i (%.1f% render)\n", (int)(frame_counter / seconds),
				100 * (float)(
					render_time.asMicroseconds()
					) / (
						logic_time.asMicroseconds() +
						render_time.asMicroseconds()));
			/* printf("Logic time  (μs): %d\n",  logic_time.asMicroseconds()); */
			/* printf("Render time (μs): %d\n", render_time.asMicroseconds()); */
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
