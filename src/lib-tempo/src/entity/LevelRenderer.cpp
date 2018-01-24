////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file LevelRenderer.cpp
/// \author Jamie Terry
/// \date 2018/01/24
/// \brief Contains implementation of the LevelRenderer class
////////////////////////////////////////////////////////////////////////////

#include <tempo/entity/LevelManager.hpp>
#include <tempo/entity/LevelRenderer.hpp>

#include <Ogre.h>

namespace tempo{
	LevelRenderer::LevelRenderer(Ogre::SceneNode* root_floor_node, LevelManager* level)
		: root_floor_node(root_floor_node) {
		if(level != null){
			updateFloorNodes(*level);
		}
	}

	void LevelRenderer::updateFloorNodes(LevelManager& level){
		for(int x = 0; x < level.getWorldWidth(); ++x){
			for(int y = 0; y < level.getWorldHeight(); ++y){

				bool level_exists_tile  = level.existsTile(x,y);
				bool render_exists_tile = tile_nodes[x][y].node != nullptr;

				if(!level_exists_tile){
					if(render_exists_tile){
						tile_nodes[x][y].removeAndDestroyAllChildren();
						root_floor_node->getSceneManager().destroySceneNode(tile_nodes[x][y].node);
						tile_nodes[x][y].node       = nullptr;
						tile_nodes[x][y].floorpiece = nullptr;
					}
				} else { // then tile exists in level
					if(!render_exists_tile){
						tile_nodes[x][y].node = root_floor_node->getSceneManager().createEntity("meshes/tile.mesh");
						Ogre::SceneNode* node = root_floor_node->createChildSceneNode();
						node->attachObject(tile_nodes[x][y].floorpiece);
						tile_nodes[x][y].node = node;
					}
					tile_nodes[x][y].node->setPosition(x, -5 + level.getHeight(x,y), y);
				}
			}
		}
	}

	void LevelRenderer::setMaterial(int x, int y, std::string material_name) {
		if(tile_nodes[x][y].floorpiece) {
			tile_nodes[x][y].floorpiece->setMaterialName(material_name);
		}
	}
}
