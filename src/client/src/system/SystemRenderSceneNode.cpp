#include <client/misc/YAlignedBillboardSceneNode.hpp>
#include <client/system/SystemRenderSceneNode.hpp>

#include <tempo/component/ComponentStageRotation.hpp>

#include <IBillboardSceneNode.h>
#include <IVideoDriver.h>
#include <iostream>

#include <irrlicht.h>

namespace client
{
void SystemRenderSceneNode::setup(irr::scene::ISceneManager* smgr, irr::video::IVideoDriver* driver)
{
	auto entities = getEntities();

	for (auto entity : entities) {
		client::ComponentRenderSceneNode& sn =
		  entity.getComponent<client::ComponentRenderSceneNode>();
		if (sn.node != nullptr)
			continue;
		tempo::ComponentModel& m = entity.getComponent<tempo::ComponentModel>();

		// Get color from componentmodel
		irr::video::SColor color(m.color.x, m.color.y, m.color.y, 255);

		// get path from componentmodel
		irr::io::path path(m.path.c_str());

		// a billboard has its origin in the centre, this node is used for alignement
		sn.node = smgr->addEmptySceneNode();

		sn.isMesh = m.isMesh;

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

			const std::string& path = m.path;
			std::cout << path << std::endl;
			irr::video::ITexture* spritesheet = driver->getTexture(path.c_str());

			irr::core::dimension2d<irr::u32> usize = spritesheet->getOriginalSize();
			irr::core::dimension2d<irr::f32> size(usize.Width / (40.0f * m.spriteDim.x), usize.Height / (40.0f * m.spriteDim.y));
			irr::core::vector3df pos(0.0f, size.Height / 2.0f, 0.0f);
			sn.billboard = new irr::scene::YAlignedBillboardSceneNode(sn.node, smgr, -1, pos, size,
			                                                          color, color);

			driver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT, true);
			//sn.billboard->setColor(color);
			sn.billboard->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			sn.billboard->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			sn.billboard->setMaterialTexture(0, spritesheet);

			sn.spriteDim = m.spriteDim;
			sn.billboard->getMaterial(0).getTextureMatrix(0).setTextureScale(1.f / sn.spriteDim.x,
			                                                                 1.f / sn.spriteDim.y);
			sn.updateNeeded = true;
		}
	}

	update();
}

void SystemRenderSceneNode::update()
{
	auto entities = getEntities();

	for (auto entity : entities) {
		tempo::ComponentStage& s = entity.getComponent<tempo::ComponentStage>();
		tempo::ComponentStagePosition& sp = entity.getComponent<tempo::ComponentStagePosition>();
		tempo::ComponentStageRotation& sr = entity.getComponent<tempo::ComponentStageRotation>();
		client::ComponentRenderSceneNode& sn =
		  entity.getComponent<client::ComponentRenderSceneNode>();

		glm::ivec2 pos = sp.getOrigin();

		if (sn.isMesh)
			continue;

		// Change Sprite based on Facing
		if (sn.updateNeeded) {
			int dirIndex = 0;
			for (int I = 0; I < 4; I++) {
				if (tempo::DIRECTIONS[I] == sr.facing)
					dirIndex = I;
			}
			sn.spritePos.y = (float)((dirIndex + 3) % 4) / sn.spriteDim.y;
			if (sr.previousFacing == sr.facing) {
				sn.spritePos.x = sn.spritePos.x + 1.f / sn.spriteDim.x;
			} else {
				sn.spritePos.x = 1.f / sn.spriteDim.x;
			}
			sn.billboard->getMaterial(0).getTextureMatrix(0).setTextureTranslate(sn.spritePos.x,
			                                                                     sn.spritePos.y);
			sn.updateNeeded = false;
		}

		// TODO: remove this jank
		if (entity.hasComponent<tempo::ComponentCombo>()) {
			tempo::ComponentCombo& c = entity.getComponent<tempo::ComponentCombo>();
			float scale = c.comboCounter / 20.f;
			scale = fmin(scale, 0.5);
			irr::core::dimension2d<irr::f32> size(1.2f + 1.2f * scale, 1.2f + 1.2f * scale);
			irr::core::vector3df pos(0.0f + 0.2 * scale, 0.0f + size.Height / 2, 0.0f);
			sn.billboard->setSize(size);
			sn.billboard->setPosition(pos);
		}

		sr.previousFacing = sr.facing;
		sn.node->setPosition(irr::core::vector3df(pos.x, s.getHeight(pos), pos.y));
	}
}

}  // namespace client
