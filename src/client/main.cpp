#include <cstdio>
#include <irrlicht.h>

int main(int argc, const char** argv){
	irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_OPENGL,
	                                                irr::core::dimension2d<irr::u32>(800, 600),
	                                                16,
	                                                false, false, false, 0);

	if(!device){
		printf("Failed to create Irrlicht Device\n");
		return 1;
	}

	device->setWindowCaption(L"RaveCave");

	irr::video::IVideoDriver*  driver  = device->getVideoDriver();
	irr::scene::ISceneManager* smgr    = device->getSceneManager();
	irr::gui::IGUIEnvironment* gui_env = device->getGUIEnvironment();

	smgr->setAmbientLight(irr::video::SColorf(0.5f, 0.5f, 0.5f, 0.5f));

	smgr->addCameraSceneNode(nullptr, // parent node
	                         irr::core::vector3df(0,30,80), // position
	                         irr::core::vector3df(0, 0, 0)  // look at
	                        );

	irr::scene::IAnimatedMesh* mesh_hills = smgr->addHillPlaneMesh(
	                                                               L"test_hill_mesh",
		irr::core::dimension2d<irr::f32>(  1,   1), // tile size
		irr::core::dimension2d<irr::u32>(100, 100), // tile count
		nullptr, // material
		1.0f // hill height
	);
	irr::scene::IAnimatedMeshSceneNode* node_hills = smgr->addAnimatedMeshSceneNode(mesh_hills);
	if (node_hills){
		node_hills->setPosition(irr::core::vector3df(0, 0, 0));
	}

	printf("Entering main loop\n");
	while(device->run()){
		smgr->drawAll();
		gui_env->drawAll();

		driver->endScene();
	}
	printf("Left main loop\n");

	device->drop();

	return 0;
}
