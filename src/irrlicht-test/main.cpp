////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \brief Entrypoint for irrlicht test application
////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <irrlicht.h>
#include <SFML/System/Clock.hpp>

int main(const int argc, const char** argv){
	irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_OPENGL,
	                                                irr::core::dimension2d<irr::u32>(800, 600),
	                                                16,
	                                                false, false, false, 0);

	if(!device){
		printf("Failed to create Irrlicht Device\n");
		return 1;
	}

	device->setWindowCaption(L"Irrlicht Test");

	irr::video::IVideoDriver*  driver = device->getVideoDriver();
	irr::scene::ISceneManager* smgr   = device->getSceneManager();
	irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	irr::gui::IGUISkin* gui_skin = guienv->getSkin();
	gui_skin->setColor(irr::gui::EGDC_BUTTON_TEXT, irr::video::SColor(255, 255, 255, 255));
	gui_skin->setFont(guienv->getFont("resources/fonts/liberation_sans.xml"));

	guienv->addStaticText(L"Hello World! This is Irrlicht",
	                      irr::core::rect<irr::s32>(10,10,500,50), false);


	irr::scene::IAnimatedMesh* mesh_sydney = smgr->getMesh("resources/meshes/sydney.md2");
	if (!mesh_sydney){
		printf("Failed to load mesh sydney\n");
		device->drop();
		return 1;
	}
	irr::scene::IAnimatedMeshSceneNode* node_sydney = smgr->addAnimatedMeshSceneNode(mesh_sydney);
	if (node_sydney){
		node_sydney->setPosition(irr::core::vector3df(20, 0, 0));
		node_sydney->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		node_sydney->setMD2Animation(irr::scene::EMAT_STAND);
		node_sydney->setMaterialTexture( 0, driver->getTexture("resources/materials/textures/sydney.bmp") );
	}


	irr::scene::IAnimatedMesh* mesh_bunny = smgr->getMesh("resources/meshes/bunny.obj");
	if (!mesh_bunny){
		printf("Failed to load mesh bunny\n");
		device->drop();
		return 1;
	}
	irr::scene::IMeshSceneNode* node_bunny = smgr->addMeshSceneNode(mesh_bunny);
	if (node_bunny){
		node_bunny->setScale(irr::core::vector3df(200, 200, 200));
		node_bunny->setPosition(irr::core::vector3df(-20, 0, 0));
		node_bunny->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	}

	smgr->addCameraSceneNode(0, irr::core::vector3df(0,30,-80), irr::core::vector3df(0,5,0));



	irr::scene::ILightSceneNode* light_0_node = smgr->addLightSceneNode(0,
	                                                                    irr::core::vector3df(0,0,0),
	                                                                    irr::video::SColorf(1.0f, 1.0f, 1.0f),
	                                                                    1000.0f);
	irr::video::SLight& light_0 = light_0_node->getLightData();
	light_0_node->enableCastShadow();
	light_0.Attenuation = irr::core::vector3df(0, 0.0001, 0.00001);

	{
		auto* node = smgr->addBillboardSceneNode(light_0_node, irr::core::dimension2d<irr::f32>(10, 10));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		node->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
		node->setMaterialTexture(0, driver->getTexture("resources/materials/textures/particlewhite.bmp"));
	}


	irr::core::matrix4 light_transform;

	sf::Clock game_timer;

	printf("Entering main loop\n");
	while(device->run()){
		float game_time = game_timer.getElapsedTime().asSeconds();
		driver->beginScene(true, true, irr::video::SColor(255, 30, 30, 30));

		node_bunny->setRotation(irr::core::vector3df(0, game_time * 60, 0));

		light_transform.setRotationDegrees(irr::core::vector3df(0, game_time * -70, 0));

		irr::core::vector3df light_0_position(30, 0, 0);
		light_transform.transformVect(light_0_position);
		light_0_node->setPosition(light_0_position);

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}
	printf("Left main loop, terminating program...\n");

	device->drop();
	return 0;
}
