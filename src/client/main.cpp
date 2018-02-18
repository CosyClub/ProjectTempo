#include <cstdio>
#include <irrlicht.h>

#include <anax/World.hpp>
#include <anax/Entity.hpp>

#include <client/system/SystemStageRenderer.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>

#include <tempo/component/ComponentStagePosition.hpp>

#include <glm/vec2.hpp>

void createEntityStage(anax::World& world){
	printf("Creating entity stage\n");
	anax::Entity entity_stage = world.createEntity();
	entity_stage.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	entity_stage.activate();
}

void createEntityPlayer(anax::World& world) {
	printf("Creating entity player\n");
	anax::Entity entity_player = world.createEntity();
	entity_player.addComponent<tempo::ComponentStagePosition>(glm::ivec2(0, 0));
	entity_player.addComponent<client::ComponentRenderSceneNode>(nullptr);
	entity_player.activate();
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

	smgr->setAmbientLight(irr::video::SColorf(0.1f, 0.1f, 0.1f));

	if (false) {
		float rotateSpeed = 25.0f;
		float moveSpeed = 0.1f;
		irr::scene::ICameraSceneNode* camera_node = smgr->addCameraSceneNodeFPS(nullptr, rotateSpeed, moveSpeed);
		device->getCursorControl()->setVisible(false);
	} else {
		float rotate = -100.0f;
		float translate = -100.0f;
		float zoom = 100.0f;
		float distance = 10.0f;
		irr::scene::ICameraSceneNode* camera_node = smgr->addCameraSceneNodeMaya(nullptr, rotate, translate, zoom, -1, distance);
		device->getCursorControl()->setVisible(true);
		camera_node->setPosition(irr::core::vector3df(10.0f, 10.0f, 10.0f));
		camera_node->setTarget(irr::core::vector3df(0.0f, 0.0f, 0.0f));
	}

	smgr->addLightSceneNode(nullptr, irr::core::vector3df(0.0f, 10.0f, 0.0f));

	irr::core::dimension2d<irr::f32> size(1.0f, 2.0f);
	irr::core::vector3df position(0.0f, 0.0f, 0.0f);
	irr::video::SColor red(255, 255, 0, 0);
	irr::scene::ISceneNode* billboard = smgr->addBillboardSceneNode(
		nullptr,
		size,
		position,
		-1,
		red,
		red);

	/////////////////////////////////////////////////
	// Setup scene
	anax::World world;
	client::SystemStageRenderer system_stage_renderer;
	world.addSystem(system_stage_renderer);
	createEntityStage(world);
	
	world.refresh();
	system_stage_renderer.setup(smgr);

	printf("Entering main loop\n");
	while(device->run()){
		if (device->isWindowActive()) {
			driver->beginScene(true, true);
			smgr->drawAll();
			gui_env->drawAll();

			driver->endScene();
		}
		else {
			device->yield();
		}
			
	}
	printf("Left main loop\n");

	device->drop();

	return 0;
}
