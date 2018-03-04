////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \brief Entrypoint for irrlicht test application
////////////////////////////////////////////////////////////////////////////

#include <irrlicht.h>
#include <SFML/System/Clock.hpp>
#include <cstdio>

#include "FloorSceneNode.hpp"
#include "gui.hpp"

float floor_heights[] = {
  0.0f, 1.0f, 3.0f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 5.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 8.0f, 7.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 10.0f,
  9.0f, 8.0f, 7.0f, 6.0f, 0.0f, 0.0f, 30.0f, 8.0f, 7.0f, 6.0f, 5.0f,
};

int main(const int argc, const char **argv)
{
	irr::IrrlichtDevice *device =
	  irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(800, 600), 16,
	                    false, false, false, 0);

	if (!device) {
		printf("Failed to create Irrlicht Device\n");
		return 1;
	}

	device->setWindowCaption(L"Irrlicht Test");

	irr::video::IVideoDriver * driver  = device->getVideoDriver();
	irr::scene::ISceneManager *smgr    = device->getSceneManager();
	irr::gui::IGUIEnvironment *gui_env = device->getGUIEnvironment();

	setupGui(gui_env);

	smgr->setAmbientLight(irr::video::SColorf(0.5f, 0.5f, 0.5f, 0.5f));

	irr::scene::IAnimatedMesh *mesh_sydney = smgr->getMesh("resources/meshes/sydney.md2");
	if (!mesh_sydney) {
		printf("Failed to load mesh sydney\n");
		device->drop();
		return 1;
	}
	irr::scene::IAnimatedMeshSceneNode *node_sydney = smgr->addAnimatedMeshSceneNode(mesh_sydney);
	if (node_sydney) {
		node_sydney->setPosition(irr::core::vector3df(40, 0, 0));
		node_sydney->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		node_sydney->setMD2Animation(irr::scene::EMAT_RUN);
		node_sydney->setMaterialTexture(
		  0, driver->getTexture("resources/materials/textures/sydney.bmp"));
	}


	irr::scene::IAnimatedMesh *mesh_bunny = smgr->getMesh("resources/meshes/bunny.obj");
	if (!mesh_bunny) {
		printf("Failed to load mesh bunny\n");
		device->drop();
		return 1;
	}
	irr::scene::IMeshSceneNode *node_bunny = smgr->addMeshSceneNode(mesh_bunny);
	if (node_bunny) {
		node_bunny->setScale(irr::core::vector3df(200, 200, 200));
		node_bunny->setPosition(irr::core::vector3df(-40, 0, 0));
		node_bunny->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		node_bunny->getMaterial(0).DiffuseColor = irr::video::SColor(255, 255, 255, 255);
	}

	// smgr->addCameraSceneNode(0, irr::core::vector3df(0,30,-80), irr::core::vector3df(0,5,0));

	// add a camera scene node
	irr::scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeMaya();
	camera->setFarValue(20000.f);
	camera->setTarget(irr::core::vector3df(0, 30, 0));


	irr::scene::ILightSceneNode *light_0_node = smgr->addLightSceneNode(
	  0, irr::core::vector3df(0, 0, 0), irr::video::SColorf(0.7f, 0.7f, 0.7f), 1000.0f);
	irr::video::SLight &light_0 = light_0_node->getLightData();
	light_0_node->enableCastShadow();
	light_0.Attenuation = irr::core::vector3df(0, 0.0001, 0.00001);

	{
		auto *node =
		  smgr->addBillboardSceneNode(light_0_node, irr::core::dimension2d<irr::f32>(10, 10));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		node->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
		node->setMaterialTexture(
		  0, driver->getTexture("resources/materials/textures/particlewhite.bmp"));
	}

	irr::scene::ILightSceneNode *light_1_node = smgr->addLightSceneNode(
	  0, irr::core::vector3df(0, 0, 0), irr::video::SColorf(1.0f, 0.1f, 0.1f), 1000.0f);
	irr::video::SLight &light_1 = light_1_node->getLightData();
	light_1_node->enableCastShadow();
	light_1.Attenuation = irr::core::vector3df(0, 0.0001, 0.00001);

	{
		auto *node =
		  smgr->addBillboardSceneNode(light_1_node, irr::core::dimension2d<irr::f32>(5, 5));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		node->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
		node->setMaterialTexture(
		  0, driver->getTexture("resources/materials/textures/particlewhite.bmp"));
		node->setColor(irr::video::SColor(255, 255, 100, 100));
	}

	FloorSceneNode *floor_node = new FloorSceneNode(smgr->getRootSceneNode(), smgr, 7, 5);
	floor_node->setPosition(irr::core::vector3df(-125.0f, -30.0f, -75.0f));
	floor_node->setScale(irr::core::vector3df(30, 0.5f, 30));
	floor_node->updateTiles(floor_heights);

	irr::scene::ISceneNode *sky_box =
	  smgr->addSkyBoxSceneNode(driver->getTexture("resources/materials/textures/irrlicht2_up.jpg"),
	                           driver->getTexture("resources/materials/textures/irrlicht2_dn.jpg"),
	                           driver->getTexture("resources/materials/textures/irrlicht2_lf.jpg"),
	                           driver->getTexture("resources/materials/textures/irrlicht2_rt.jpg"),
	                           driver->getTexture("resources/materials/textures/irrlicht2_ft.jpg"),
	                           driver->getTexture("resources/materials/textures/irrlicht2_bk.jpg"));


	irr::scene::IParticleSystemSceneNode *ps = smgr->addParticleSystemSceneNode(false);

	irr::scene::IParticleEmitter *em =
	  ps->createBoxEmitter(irr::core::aabbox3d<irr::f32>(-1, -1, -1, 1, 1, 1),  // emitter size
	                       irr::core::vector3df(0.0f, 0.05f, 0.0f),             // initial direction
	                       80, 100,                                             // emit rate
	                       irr::video::SColor(0, 255, 255, 255),                // darkest color
	                       irr::video::SColor(0, 255, 255, 255),                // brightest color
	                       800, 1500, 50,                         // min and max age, angle
	                       irr::core::dimension2df(5.f, 5.f),     // min size
	                       irr::core::dimension2df(10.f, 10.f));  // max size

	ps->setEmitter(em);  // this grabs the emitter
	em->drop();          // so we can drop it here without deleting it

	irr::scene::IParticleAffector *paf = ps->createFadeOutParticleAffector();
	ps->addAffector(paf);  // same goes for the affector
	paf->drop();

	irr::scene::IParticleAffector *pafg = ps->createGravityAffector();
	ps->addAffector(pafg);  // same goes for the affector
	pafg->drop();

	ps->setPosition(irr::core::vector3df(0, 0, 0));
	ps->setScale(irr::core::vector3df(2, 2, 2));
	ps->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	ps->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("resources/materials/textures/particlewhite.bmp"));
	ps->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);

	irr::core::matrix4 light_0_transform;
	irr::core::matrix4 light_1_transform;

	sf::Clock game_timer;

	printf("Entering main loop\n");
	while (device->run()) {
		float game_time = game_timer.getElapsedTime().asSeconds();
		driver->beginScene(true, true, irr::video::SColor(255, 30, 30, 30));

		node_bunny->setRotation(irr::core::vector3df(0, game_time * 60, 0));

		light_0_transform.setRotationDegrees(irr::core::vector3df(0, game_time * -70, 0));
		irr::core::vector3df light_0_position(30, 0, 0);
		light_0_transform.transformVect(light_0_position);
		light_0_node->setPosition(light_0_position);

		light_1_transform.setRotationDegrees(irr::core::vector3df(0, game_time * 300.0f, 0));
		irr::core::vector3df light_1_position(15, 15, 0);
		light_1_transform.transformVect(light_1_position);
		light_1_position += node_bunny->getPosition();
		light_1_node->setPosition(light_1_position);

		smgr->drawAll();
		gui_env->drawAll();

		driver->endScene();
	}
	printf("Left main loop, terminating program...\n");

	device->drop();
	return 0;
}
