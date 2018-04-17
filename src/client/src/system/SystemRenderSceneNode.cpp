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

			driver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT,true);
			// sn.billboard->setColor(color);
			sn.billboard->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			sn.billboard->setMaterialType( irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL );

			sn.spriteDim = m.spriteDim;

			sn.updateNeeded = true;
		}
	}

	update(driver);
}

void SystemRenderSceneNode::update(irr::video::IVideoDriver *driver)
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
			sn.spritePos.y = (float) ((dirIndex + 3) % 4) / sn.spriteDim.y;
			if (sr.previousFacing == sr.facing) {
				++sn.spritePos.x;
			} else {
				sn.spritePos.x = 0;
			}

			tempo::ComponentModel &m = entity.getComponent<tempo::ComponentModel>();

			char path_buffer     [512];
			char extension_buffer[ 32];

			strcpy(path_buffer, m.path.c_str());

			char* last_dot = nullptr;
			char* cur      = path_buffer;
			while(*cur){
				if(*cur == '.'){ last_dot = cur; }
				++cur;
			}

			strcpy(extension_buffer, last_dot);
			last_dot[0] = '_';
			last_dot[1] = '0' + (dirIndex + 3) % 4;
			last_dot[2] = '_';
			last_dot[3] = '0' + ((int)sn.spritePos.x) % 4;
			strcpy(&last_dot[4], extension_buffer);

			irr::video::ITexture* sprite = driver->getTexture(path_buffer);
			sn.billboard->setMaterialTexture(0, sprite);
			sn.updateNeeded = false;
		}

		if (entity.hasComponent<tempo::ComponentCombo>())
		{
			tempo::ComponentCombo& c = entity.getComponent<tempo::ComponentCombo>();
			float scale = c.comboCounter / 20.f;
			scale = fmin(scale, 0.5);
			irr::core::dimension2d<irr::f32> size(1.2f + 1.2f * scale, 1.6f +  1.6f * scale);
			irr::core::vector3df             pos(0.0f + 0.2 * scale, 0.0f + size.Height / 2, 0.0f);
			sn.billboard->setSize(size);
			sn.billboard->setPosition(pos);

		}

		sr.previousFacing = sr.facing;
		sn.node->setPosition(irr::core::vector3df(pos.x, s.getHeight(pos), pos.y));
	}
}

}  // namespace client
