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

	Ogre::Entity* entity_ogre = scene->createEntity("meshes/floor.mesh");
	Ogre::SceneNode* node_ogre = scene->getRootSceneNode()->createChildSceneNode();
    node_ogre->setScale(10, 0.05, 10);
	node_ogre->attachObject(entity_ogre);

    Ogre::BillboardSet* Bset = scene->createBillboardSet();
    Bset->setMaterialName("rectangleSprite");
    Ogre::Billboard* playerBoard = Bset->createBillboard(0, 0, 0);
    playerBoard->setColour(Ogre::ColourValue::Red);
	Ogre::SceneNode* node_player = scene->getRootSceneNode()->createChildSceneNode();
	node_player->attachObject(Bset);
    

	Ogre::Viewport* vp   = app->addViewport(camera);

	/////////////////////////////////////////////////
	// Setup input handling


	/////////////////////////////////////////////////
	// Start game, will block until frame listener returns false
	int frame_counter = 0;
	Ogre::Timer frame_timer;
	Ogre::Timer fps_timer;
	while(!app->isClosed()){
		float rot_factor = (Ogre::Real)frame_timer.getMicroseconds() / 600000.0f;
		float cam_dist   = 150 + (100 * sin(rot_factor / 3.0f));
		float cam_x = cam_dist * sin(rot_factor);
		float cam_z = cam_dist * cos(rot_factor);

		node_camera->setPosition(10, 5, 10);
		camera->lookAt(0,0,0);

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
