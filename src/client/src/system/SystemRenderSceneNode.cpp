#include <client/system/SystemRenderSceneNode.hpp>

#include <IBillboardSceneNode.h>

#include <iostream>

namespace client
{
void SystemRenderSceneNode::setup(irr::scene::ISceneManager *smgr)
{
	irr::core::dimension2d<irr::f32> size(0.7f, 1.4f);
	irr::core::vector3df             pos(0.0f, 0.0f + size.Height / 2, 0.0f);

	auto entities = getEntities();

	for (auto entity : entities) {
		client::ComponentRenderSceneNode &sn =
		  entity.getComponent<client::ComponentRenderSceneNode>();
		if (sn.node != nullptr)
			continue;
		tempo::ComponentModel &m = entity.getComponent<tempo::ComponentModel>();

		// Get color from componentmodel
		irr::video::SColor color(255, m.color.x, m.color.y, m.color.z);

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
			  smgr->addBillboardSceneNode(sn.node, size,
			                              pos,  // fix alignment
			                              -1, color, color);
			billboard->setMaterialFlag(irr::video::EMF_LIGHTING, false);
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
