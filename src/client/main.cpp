#include <cstdio>
#include <irrlicht.h>

#include <anax/World.hpp>
#include <anax/Entity.hpp>

#include <client/system/SystemStageRenderer.hpp>

void createEntityStage(anax::World& world){
	printf("Creating entity stage\n");
	anax::Entity entity_stage = world.createEntity();
	entity_stage.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	entity_stage.activate();
}

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

	// smgr->addCameraSceneNode(nullptr, // parent node
	//                          irr::core::vector3df(0,30,80), // position
	//                          irr::core::vector3df(0, 0, 0)  // look at
	//                         );

	irr::scene::ISceneNode* node = 0;

	if (node)
		node->setPosition(irr::core::vector3df(0,30,80));


	float 	rotateSpeed = 25.0f;
	float 	moveSpeed = 0.25f;

	smgr->addCameraSceneNodeFPS(node, rotateSpeed, moveSpeed);

	device->getCursorControl()->setVisible(false);

	/////////////////////////////////////////////////
	// Setup scene
	anax::World world;
	client::SystemStageRenderer system_stage_renderer(smgr);

	createEntityStage(world);

	world.addSystem(system_stage_renderer);
	world.refresh();

	system_stage_renderer.initialize2();

	printf("Entering main loop\n");
	while(device->run()){
		if (device->isWindowActive()) {
			driver->beginScene(true, true);
			smgr->drawAll();
			gui_env->drawAll();

			driver->endScene();
				}
		else
			device->yield();
	}
	printf("Left main loop\n");

	device->drop();

	return 0;
}
