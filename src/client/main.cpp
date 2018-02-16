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
#include <OgreFont.h>
#include <OgreFontManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayContainer.h>
#include <OgreTextAreaOverlayElement.h>

#include <tempo/Application.hpp>
#include <tempo/song.hpp>
#include <tempo/time.hpp>
#include <tempo/entity/EntityCreationClient.hpp>
#include <tempo/entity/LevelRenderer.hpp>
#include <tempo/network/client.hpp>
#include <tempo/system/SystemRenderHealth.hpp>
#include <tempo/system/SystemTransform.hpp>
#include <tempo/system/SystemCombo.hpp>
#include <tempo/system/SystemGridAi.hpp>
#include <tempo/system/SystemHealth.hpp>
#include <tempo/system/SystemLevelManager.hpp>
#include <tempo/system/SystemGameInput.hpp>
#include <tempo/system/SystemPlayer.hpp>
#include <tempo/system/SystemRender.hpp>

#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include <anax/World.hpp>
#include <anax/Entity.hpp>

#include <glm/gtc/constants.hpp>

#define BPM 174
#define DELTA 150
#define TIME 60000000 / BPM

void sync_time(tempo::Clock& clock, tempo::Song *song)
{
	sf::Time t = tempo::timeSyncClient(&clock);
	clock.set_time(t, song);
}

void new_entity_check(anax::World &world, Ogre::SceneManager* scene, tempo::SystemLevelManager system_level)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(tempo::QueueID::ENTITY_CREATION);
	while (!q->empty())
	{
		sf::Packet p = q->front();
		tempo::EntityCreationData data;
		p >> data;
		anax::Entity e = newEntity(data, world, scene, system_level);
		int iid = e.getComponent<tempo::ComponentID>().instance_id;
		q->pop();
	}
}

int main(int argc, const char** argv)
{
	std::cout << glm::pi<float>() << std::endl;
	
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
	mainsong.set_volume(0.f);

	sf::SoundBuffer clickbuf;
	clickbuf.loadFromFile("resources/sound/tick.ogg");
	sf::Sound click;
	click.setBuffer(clickbuf);

	// Clock
	tempo::Clock clock = tempo::Clock(sf::microseconds(TIME), sf::milliseconds(DELTA));

	/////////////////////////////////////////////////
	// Setup scene
	anax::World world;
	tempo::SystemRender           system_render(app);
	tempo::SystemLevelManager     system_level(world,
	                                           "../bin/resources/levels/levelTest.bmp",
	                                           "../bin/resources/levels/zonesTest.bmp"
	                                           );
	tempo::SystemUpdateTransforms system_update_transforms;
	tempo::SystemGridAi           system_grid_ai;
	tempo::SystemGameInput        system_input(clock);
	tempo::SystemPlayer           system_player(clock);
	tempo::SystemCombo            system_combo;
	tempo::SystemHealth           system_health;
	tempo::RenderHealth           render_health;
	tempo::SystemID               system_id;

	world.addSystem(system_level);
	world.addSystem(system_update_transforms);
	world.addSystem(system_grid_ai);
	world.addSystem(system_render);
	world.addSystem(system_input);
	world.addSystem(system_player);
	world.addSystem(system_combo);
	world.addSystem(system_health);
	world.addSystem(render_health);
	world.addSystem(system_id);
	world.refresh();

	Ogre::SceneManager* scene = system_render.scene;

	/////////////////////////////////////////////////
	// Networking

	// Set up remote address, local ports and remote handshake port
	// Note, IF statement is to change ports for local development, bit
	// hacky and should be removed in due course!
	tempo::addr_r = "127.0.0.1";
	if (tempo::addr_r == "127.0.0.1") {
		std::srand (time(NULL));
		int d = std::rand() % 10;
		tempo::port_ci = DEFAULT_PORT_IN+10+d;
		tempo::port_co = DEFAULT_PORT_OUT+10+d;
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
	tempo::connectToAndSyncWithServer(role, roleData, world, scene, system_level);

	// Start and Sync Song
	mainsong.start();
	sync_time(clock, &mainsong);
	mainsong.set_volume(20.f);
	long offset = 0;


	///////////////////////
	// Who even knows

	tempo::LevelRenderer level_renderer(scene, scene->getRootSceneNode(), &system_level);

	scene->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
	Ogre::SceneNode* node_light = scene->getRootSceneNode()->createChildSceneNode();
	Ogre::Light* light = scene->createLight("MainLight");
	node_light->attachObject(light);
	node_light->setPosition(20, 80, 50);

	//auto node_floor = system_grid_motion.getFloorNode();

	// Player
	/* anax::Entity entity_player = tempo::newPlayer(world, scene, 0, tempo::EID_PLAYER, system_level, 2, 2); */
	// TODO: use better way to find out player, for now this is a search
	anax::Entity entity_player;
	for (auto& entity : world.getEntities()) {
		if (entity.hasComponent<tempo::ComponentPlayerLocal>()) {
			entity_player = entity;
			break;
		}
	}

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




	Ogre::OverlaySystem* OverlaySystem = new Ogre::OverlaySystem();
	scene->addRenderQueueListener(OverlaySystem);

	Ogre::FontManager &fman = Ogre::FontManager::getSingleton();
	// create a font resource
	Ogre::ResourcePtr resource = fman.create("Roboto",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	// set as truetype
	resource->setParameter("type","truetype");
	// set the .ttf file name
	resource->setParameter("source","fonts/Roboto-Black.ttf");
	// set the size
	resource->setParameter("size","16");
	// set the dpi
	resource->setParameter("resolution","96");
	// load the ttf
	resource->load();

	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

	// Create a panel
	Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(
	    overlayManager.createOverlayElement("Panel", "PanelName"));
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setPosition(10.0, 550.0);
	panel->setDimensions(100, 100);
	//panel->setMaterialName("MaterialName"); // Optional background material

	// Create a text area
	Ogre::TextAreaOverlayElement* textArea = static_cast<Ogre::TextAreaOverlayElement*>(
	    overlayManager.createOverlayElement("TextArea", "TextAreaName"));
	textArea->setMetricsMode(Ogre::GMM_PIXELS);
	textArea->setPosition(0, 0);
	textArea->setDimensions(100, 100);
	textArea->setCharHeight(40);
	textArea->setFontName("Roboto");
	textArea->setColour(Ogre::ColourValue::Red);

	// Create an overlay, and add the panel
	Ogre::Overlay* overlay = overlayManager.create("OverlayName");
	overlay->add2D(panel);

	// Add the text area to the panel
	panel->addChild(textArea);

	// Show the overlay
	overlay->show();

	//
	// Ogre::FontManager &fontMgr = Ogre::FontManager::getSingleton();
	//  // create a font resource
	//  ResourcePtr font = fontMgr.create("MyFont","General");
	//  // set as truetype
	//  font->setParameter("type","truetype");
	//  // set the .ttf file name
	//  font->setParameter("source",/usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf);
	//  // set the size
	//  font->setParameter("size","26");
	//  // set the dpi
	//  font->setParameter("resolution","96");
	//  // load the ttf
	//  font->load();

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
		new_entity_check(world, scene, system_level);

		float dt = dt_timer.getElapsedTime().asSeconds();
		dt_timer.restart();

		if (clock.passed_beat()) {
			click.play();

			system_grid_ai.update();
			system_combo.advanceBeat();
		}


		auto& combo = entity_player.getComponent<tempo::ComponentCombo>();
		char buffer [50];
		sprintf (buffer, "Combo: %d", combo.comboCounter);

		textArea->setCaption(buffer);

		float seconds_until_beat = clock.until_beat().asSeconds();
		float seconds_since_beat = clock.since_beat().asSeconds();

		// Value between 0 and 1 indicating progress towards next beat,
		// where 0 means we've just had last beat and 1 means we've just
		// hit next beat
		float beat_progress = clock.beat_progress();

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (!system_input.handleInput(e)) {
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

		//float cam_motion_delta = sin(beat_progress) * 0.3f;
		//node_camera->setPosition(sin(beat_progress-0.5)*0.1f, 8 + cam_motion_delta, 12 + cam_motion_delta);

		float light_intensity = 2 / (exp(beat_progress));
		light->setDiffuseColour(light_intensity, light_intensity, light_intensity);

		world.refresh();
		system_player.update(entity_player.getComponent<tempo::ComponentID>().instance_id, &system_id);
		render_health.HealthBarUpdate();
		system_health.CheckHealth();
		system_level.update(dt);
		system_update_transforms.update(system_level);
		logic_time = dt_timer.getElapsedTime();
		system_render.render(dt);
		render_time = dt_timer.getElapsedTime() - logic_time;
		SDL_GL_SwapWindow(app.window);

		++frame_counter;
		if (fps_timer.getElapsedTime().asSeconds() > 0.5f) {
			float seconds = fps_timer.getElapsedTime().asSeconds();
			/* printf("FPS: %i (%.1f% render)\n", (int)(frame_counter / seconds), */
			/* 	100 * (float)( */
			/* 		render_time.asMicroseconds() */
			/* 		) / ( */
			/* 			logic_time.asMicroseconds() + */
			/* 			render_time.asMicroseconds())); */
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
