#include <client/system/SystemRenderSceneNode.hpp>
#include <tempo\component\ComponentStageRotation.hpp>

#include <IBillboardSceneNode.h>
#include <IVideoDriver.h>
#include <iostream>

#include <irrlicht.h>

#include <client/misc/YAlignedBillboardSceneNode.hpp>

namespace client
{
void SystemRenderSceneNode::setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver)
{
	irr::core::dimension2d<irr::f32> sizePlayer(1.2f, 1.7f);
	irr::core::dimension2d<irr::f32> sizeRogue(1.3f, 1.5f);
	irr::core::vector3df             posPlayer(0.0f, 0.0f + sizePlayer.Height / 2, 0.0f);
	irr::core::vector3df             posRogue(0.0f, 0.0f + sizeRogue.Height / 2, 0.0f);

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
			if (sn.spritesheet == "rogue.png") {
				sn.billboard =
					new irr::scene::YAlignedBillboardSceneNode(sn.node, smgr, -1,
						posRogue,  // fix alignment
						sizeRogue, color, color);
			}
			else {
				sn.billboard =
					new irr::scene::YAlignedBillboardSceneNode(sn.node, smgr, -1,
						posPlayer,  // fix alignment
						sizePlayer, color, color);
			}
			const std::string& path = "resources/materials/textures/" + sn.spritesheet;
			irr::video::ITexture * spritesheet = driver->getTexture(path.c_str());

			driver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT,true);
			sn.billboard->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			sn.billboard->setMaterialType( irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL );
			sn.billboard->setMaterialTexture( 0, spritesheet);
			sn.billboard->getMaterial(0).getTextureMatrix(0).setTextureScale((1.0/sn.spriteCols),(1.0 / sn.spriteRows));
			sn.u = 0;
			sn.v = 0;
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

		// Change Sprite based on Facing
		if (sn.updateNeeded) {

			switch (sr.facing.y)
			{
			//Right
			case (1):
				switch (sr.previousFacing.y)
				{
				//Already facing right
				case (1):
					sn.u = sn.u + (1.0 / sn.spriteCols);
					break;
				//Facing another direction
				default:
					sn.u = (1.0 / sn.spriteCols);
					sn.v = (1.0 / sn.spriteRows) * 2.0;
					break;
				}
				break;
			//Left
			case (-1):
				switch (sr.previousFacing.y)
				{
				//Already facing right
				case (-1):
					sn.u = sn.u + (1.0 / sn.spriteCols);
					break;
				default:
					sn.u = (1.0 / sn.spriteCols);
					sn.v = (1.0 / sn.spriteRows);
					break;
				}
				break;

			case (0):
				switch (sr.facing.x)
				{
				//Down
				case (1):
					switch (sr.previousFacing.x)
					{
					//Already facing down
					case (1):
						sn.u = sn.u + (1.0 / sn.spriteCols);
						break;
					default:
						sn.u = (1.0 / sn.spriteCols);
						sn.v = 0.0;
						break;
					}
					break;
				//Up
				case (-1):
					switch (sr.previousFacing.x)
					{
					//Already facing up
					case (-1):
						sn.u = sn.u + (1.0 / sn.spriteCols);
						break;
					default:
						sn.u = (1.0 / sn.spriteCols);
						sn.v = (1.0 / sn.spriteRows) * 3.0;
						break;
					}
					break;
				case (0):
					printf("Jump?\n");
					break;
				}
				break;
			default:
				printf("Shouldn't happen!!\n");
				break;
			}

			sn.billboard->getMaterial(0).getTextureMatrix(0).setTextureTranslate(sn.u, sn.v);
			sn.updateNeeded = false;
		}

		sr.previousFacing = sr.facing;
		sn.node->setPosition(irr::core::vector3df(pos.x, s.getHeight(pos), pos.y));
	}
}
}  // namespace client
