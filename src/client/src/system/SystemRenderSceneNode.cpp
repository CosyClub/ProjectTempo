#include <client/system/SystemRenderSceneNode.hpp>

#include <IBillboardSceneNode.h>
#include <IVideoDriver.h>
#include <iostream>

#include <irrlicht.h>

#include <client/misc/YAlignedBillboardSceneNode.hpp>

namespace client
{
void SystemRenderSceneNode::setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver)
{
	irr::core::dimension2d<irr::f32> size(1.2f, 1.6f);
	irr::core::vector3df             pos(0.0f, 0.0f + size.Height / 2, 0.0f);

	auto entities = getEntities();

	for (auto entity : entities) {
		client::ComponentRenderSceneNode &sn =
		  entity.getComponent<client::ComponentRenderSceneNode>();
		if (sn.node != nullptr)
			continue;
		std::cout << "THIS SHOULD HAVE BEEN CALLED\n";
		tempo::ComponentModel &m = entity.getComponent<tempo::ComponentModel>();

		// Get color from componentmodel
		irr::video::SColor color(255, 255, 255, 255);

		// get path from componentmodel
		irr::io::path path(m.path.c_str());

		// a billboard has its origin in the centre, this node is used for alignement
		sn.node = smgr->addEmptySceneNode();
		if (m.isMesh) {
			std::cout << "Adding mesh" << std::endl;
			// irr::scene::IMesh *mesh = smgr->getMesh(path);
			std::cout << m.path << std::endl;
			std::cout << path.c_str() << std::endl;
			// irr::scene::IMeshSceneNode *meshNode = smgr->addMeshSceneNode(mesh,
			//                                                               sn.node);
		} else {
			std::cout << "Adding billboard" << std::endl;
			sn.node->setPosition(irr::core::vector3df(0.0f, 0.0f, 0.0f));
			irr::scene::IBillboardSceneNode *billboard =
				new irr::scene::YAlignedBillboardSceneNode(sn.node, smgr, -1,
														   pos,  // fix alignment
														   size, color, color);

			driver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT,true);
			billboard->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			billboard->setMaterialType( irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL );
			billboard->setMaterialTexture( 0, driver->getTexture("resources/materials/textures/zombiespritesheet.png"));
			billboard->getMaterial(0).getTextureMatrix(0).setTextureScale(.1,.5);
		}
	}

	update();
}

void SystemRenderSceneNode::update()
{
	auto entities = getEntities();

	for (auto entity : entities) {
		tempo::ComponentStage &           s  = entity.getComponent<tempo::ComponentStage>();
		tempo::ComponentStagePosition &   sp = entity.getComponent<tempo::ComponentStagePosition>();
		client::ComponentRenderSceneNode &sn =
		  entity.getComponent<client::ComponentRenderSceneNode>();

		glm::ivec2 pos = sp.getOrigin();

		// std::cout << "Moved billboard to: " << pos.x << ", " << s.getHeight(pos) << ", " << pos.y
		// << std::endl;
		sn.node->setPosition(irr::core::vector3df(pos.x, s.getHeight(pos), pos.y));
		// TODO
	}
}
}  // namespace client
