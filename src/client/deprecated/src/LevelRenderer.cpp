#include <client/LevelRenderer.hpp>

#include <glm/vec2.hpp>

namespace tempo
{
LevelRenderer::LevelRenderer(Ogre::SceneManager *scene,
                             Ogre::SceneNode *   root_floor_node,
                             SystemLevelManager *level)
    : scene(scene)
    , root_floor_node(root_floor_node)
{
	if (level != nullptr) {
		updateFloorNodes(*level);
	}
}

void LevelRenderer::updateFloorNodes(SystemLevelManager &level)
{
	printf("LevelRenderer: Begin update of floor nodes\n");

	glm::vec2 world_size = level.getWorldSize();

	tile_nodes.resize(world_size.x);
	for (int x = 0; x < world_size.x; ++x) {
		tile_nodes[x].resize(world_size.y);
		for (int y = 0; y < world_size.y; ++y) {
			bool level_exists_tile  = level.existsTile(x, y);
			bool render_exists_tile = tile_nodes[x][y].node != nullptr;

			if (!level_exists_tile) {
				if (tile_nodes[x][y].node) {
					if (tile_nodes[x][y].floorpiece) {
						tile_nodes[x][y].node->detachObject(tile_nodes[x][y].floorpiece);
						scene->destroyEntity(tile_nodes[x][y].floorpiece);
					}
					scene->destroySceneNode(tile_nodes[x][y].node);
				}
				tile_nodes[x][y].floorpiece = nullptr;
				tile_nodes[x][y].node       = nullptr;
			} else {  // then tile exists in level
				if (!render_exists_tile) {
					tile_nodes[x][y].floorpiece = scene->createEntity("meshes/tile.mesh");
					Ogre::SceneNode *node       = root_floor_node->createChildSceneNode();
					node->attachObject(tile_nodes[x][y].floorpiece);
					tile_nodes[x][y].node = node;
				}
				tile_nodes[x][y].node->setPosition(x, -5 + level.getHeight(x, y), y);
			}
		}
	}

	printf("LevelRenderer: Done update of floor nodes\n");
}

void LevelRenderer::setMaterial(int x, int y, std::string material_name)
{
	if (tile_nodes[x][y].floorpiece) {
		tile_nodes[x][y].floorpiece->setMaterialName(material_name);
	}
}
}
