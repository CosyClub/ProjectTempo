#include <client/system/SystemRenderSceneNode.hpp>
#include <client/misc/YAlignedBillboardSceneNode.hpp>

#include <tempo/component/ComponentStageRotation.hpp>

#include <IBillboardSceneNode.h>
#include <IVideoDriver.h>
#include <iostream>

#include <irrlicht.h>

namespace client
{
void SystemRenderSceneNode::setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver)
{
	// irr::core::dimension2d<irr::f32> sizePlayer(1.2f, 1.7f);
	// irr::core::dimension2d<irr::f32> sizeRogue(1.3f, 1.5f);
	// irr::core::vector3df             posPlayer(0.0f, 0.0f + sizePlayer.Height / 2, 0.0f);
	// irr::core::vector3df             posRogue(0.0f, 0.0f + sizeRogue.Height / 2, 0.0f);
	irr::core::dimension2d<irr::f32> size(1.2f, 1.6f);
	irr::core::vector3df             pos(0.0f, 0.0f + size.Height / 2, 0.0f);

	auto entities = getEntities();

	for (auto entity : entities) {
		client::ComponentRenderSceneNode &sn =
		  entity.getComponent<client::ComponentRenderSceneNode>();
		if (sn.node != nullptr)
			continue;
		tempo::ComponentModel &m = entity.getComponent<tempo::ComponentModel>();

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
			sn.billboard = new irr::scene::YAlignedBillboardSceneNode(sn.node, smgr, -1, pos, size, color, color);

			const std::string& path = m.path;
			std::cout << path << std::endl;
			irr::video::ITexture *spritesheet = driver->getTexture(path.c_str());

			driver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT,true);
			// sn.billboard->setColor(color);
			sn.billboard->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			sn.billboard->setMaterialType( irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL );
			sn.billboard->setMaterialTexture( 0, spritesheet);
			
			sn.spriteDim = m.spriteDim;
			sn.billboard->getMaterial(0).getTextureMatrix(0).setTextureScale(1.f / sn.spriteDim.y, 1.f / sn.spriteDim.x);
			sn.updateNeeded = true;
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
		tempo::ComponentStageRotation &   sr = entity.getComponent<tempo::ComponentStageRotation>();
		client::ComponentRenderSceneNode &sn =
		  entity.getComponent<client::ComponentRenderSceneNode>();

		glm::ivec2 pos = sp.getOrigin();

		if (sn.isMesh) continue;

		// Change Sprite based on Facing
		if (sn.updateNeeded) {
			int dirIndex = 0;
			for (int I = 0; I < 4; I++) {
				if( tempo::DIRECTIONS[I] == sr.facing ) dirIndex = I;
			}
			std::cout << dirIndex<< std::endl;
			sn.spritePos.y = (float) ((dirIndex + 3) % 4) / sn.spriteDim.y;
			printf("%d,%d\n", sr.facing.x, sr.facing.y);
			if (sr.previousFacing == sr.facing) {
				sn.spritePos.x = sn.spritePos.x + 1.f / sn.spriteDim.x;
				std::cout << sn.spritePos.x << std::endl;
			} else {
				sn.spritePos.x = 1.f / sn.spriteDim.x;
			}
			printf("%f,%f\n", sn.spritePos.x, sn.spritePos.y);
			sn.billboard->getMaterial(0).getTextureMatrix(0).setTextureTranslate(sn.spritePos.x, sn.spritePos.y);
			sn.updateNeeded = false;
		}

		sr.previousFacing = sr.facing;
		sn.node->setPosition(irr::core::vector3df(pos.x, s.getHeight(pos), pos.y));
	}
}

}  // namespace client
