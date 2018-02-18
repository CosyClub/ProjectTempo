#include <client/system/SystemRenderSceneNode.hpp>

#include <IBillboardSceneNode.h>

#include <iostream>

namespace client {
	void SystemRenderSceneNode::setup(irr::scene::ISceneManager* smgr) {
		std::cout << "SystemRenderSceneNode initializing" << std::endl;

		irr::core::dimension2d<irr::f32> size(1.0f, 2.0f);
		irr::core::vector3df pos(0.0f, 0.0f+size.Height / 2, 0.0f);
		irr::video::SColor top_color(255, 255, 0, 0);
		irr::video::SColor bottom_color(255, 255, 0, 0);

		auto entities = getEntities();

		for (auto entity : entities)
		{
			std::cout << "Adding billboard" << std::endl;
			client::ComponentRenderSceneNode& sn = entity.getComponent<client::ComponentRenderSceneNode>();

			// a billboard has its origin in the centre, this node is used for alignement
			sn.node = smgr->addEmptySceneNode();
			sn.node->setPosition(irr::core::vector3df(0.0f, 0.0f, 0.0f));
			irr::scene::IBillboardSceneNode* billboard = smgr->addBillboardSceneNode(
				sn.node,
				size,
				pos, // fix alignment
				-1, 
				top_color, 
				bottom_color);
		}

		update();
	}

	void SystemRenderSceneNode::update() {
		auto entities = getEntities();

		for (auto entity : entities)
		{
			tempo::ComponentStage& s = entity.getComponent<tempo::ComponentStage>();
			tempo::ComponentStagePosition& sp = entity.getComponent<tempo::ComponentStagePosition>();
			client::ComponentRenderSceneNode& sn = entity.getComponent<client::ComponentRenderSceneNode>();

			glm::ivec2 pos = sp.getOrigin();
			
			std::cout << "Moved billboard to: " << pos.x << ", " << s.getHeight(pos) << ", " << pos.y << std::endl;
			sn.node->setPosition(irr::core::vector3df(pos.x, s.getHeight(pos), pos.y));
			// TODO
		}
	}
} // namespace client
