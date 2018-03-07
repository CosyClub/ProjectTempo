#include <client/system/SystemStageRenderer.hpp>

#include <tempo/component/ComponentStage.hpp>

#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>
#include <irrlicht.h>  // :TODO: sort out includes

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <iostream>

namespace client
{

inline void SystemStageRenderer::addFloorTilesToScene(irr::scene::ISceneManager *smgr,
                                         irr::video::IVideoDriver * driver,
                                         std::vector<tempo::stage_tile>&       tiles)
{

  if(tile_nodes.size() == tiles.size()) return;

  irr::scene::IMesh *mesh = smgr->getMesh("resources/meshes/tile.obj");
  if (!mesh) {
    std::cout << "Failed to load floor tile mesh\n" << std::endl;
    return;
  }

	irr::video::ITexture *wall_diffuse_map =
	  driver->getTexture("resources/materials/walls/cobblestone.png");
	irr::video::ITexture *wall_normal_map =
	  driver->getTexture("resources/materials/walls/cobblestone_n.png");
  irr::video::ITexture *tile_texture =
	  driver->getTexture("resources/materials/TileLightMaskPixelOn.png");


	for (unsigned int i = tile_nodes.size(); i < tiles.size(); ++i) {
		irr::scene::IMeshSceneNode *node = smgr->addMeshSceneNode(mesh, 0);

		float grid_x = tiles[i].position.x;
		float grid_y = tiles[i].position.y;
		float height = tiles[i].height;

		tile_nodes.push_back(std::make_tuple(glm::ivec2(grid_y, grid_x), node));
    old_positions.push_back(tempo::stage_tile(glm::ivec2(grid_y, grid_x), height));

		node->setPosition(irr::core::vector3df(grid_x, height, grid_y));
		node->setMaterialTexture(0, tile_texture);

		irr::video::SMaterial &material_side = node->getMaterial(0);
		irr::video::SMaterial &material_top  = node->getMaterial(1);

		material_side.setTexture(0, wall_diffuse_map);
		material_side.setTexture(1, wall_normal_map);

		material_top.DiffuseColor.set(255, 10, 10, 10);

	}

}


void SystemStageRenderer::setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver)
{
	printf("SystemStageRenderer initializing\n");

	auto  entities = getEntities();
	auto  entity   = std::begin(entities);
	auto &stage    = entity->getComponent<tempo::ComponentStage>();

	auto tiles = stage.getHeights();

	//tile_nodes =
  addFloorTilesToScene(smgr, driver, tiles);
}

void SystemStageRenderer::updateStage(glm::ivec4                colour1,
                                      glm::ivec4                colour2,
                                      irr::video::IVideoDriver *driver,
                                      int                       j)
{
	auto  entities = getEntities();
	auto  entity   = std::begin(entities);
	auto &stage    = entity->getComponent<tempo::ComponentStage>();

	auto heights = stage.getHeights();

	for (unsigned int i = 0; i < this->tile_nodes.size(); ++i) {
		irr::scene::IMeshSceneNode *node = std::get<1>(this->tile_nodes[i]);

    auto animation_pos = node->getPosition();
    float old_height = old_positions[i].height;

    float height = heights[i].height;

    if(old_height != height) {
      if(animation_pos.Y < height - 0.001f || height + 0.001f < animation_pos.Y) {
        animation_pos.Y += (height - old_height) / 60.f;
        node->setPosition(irr::core::vector3df(animation_pos.X, animation_pos.Y, animation_pos.Z));
        if(animation_pos.Y < height - 0.001f || height + 0.001f < animation_pos.Y) continue;
      } else {
        old_height = height;
      }
    }


		if (heights[i].height >= 5) {
			continue;
		}

		irr::video::SMaterial &material_side = node->getMaterial(0);
		irr::video::SMaterial &material_top  = node->getMaterial(1);

		switch (j) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 9:
		case 10:
		case 11:
		case 12: checkerBoardPattern(driver, node, material_top, colour1, colour2, i, j); break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8: linePattern(driver, node, material_top, colour1, colour2, 0, 5, i, j - 4); break;
		case 13:
		case 14:
		case 15:
		case 16:
		case 17: linePattern(driver, node, material_top, colour1, colour2, 1, 5, i, j - 13); break;
		case 18:
		case 19:
		case 20:
		case 21: squarePattern(driver, node, material_top, colour1, colour2, 1, 12, i, j); break;
		}
	}
}

inline void SystemStageRenderer::checkerBoardPattern(irr::video::IVideoDriver *  driver,
                                                     irr::scene::IMeshSceneNode *node,
                                                     irr::video::SMaterial &     material_top,
                                                     glm::ivec4                  colour1,
                                                     glm::ivec4                  colour2,
                                                     int                         i,
                                                     int                         j)
{
	if (((int) (std::get<0>(this->tile_nodes[i]).x % 2)
	     == (int) (std::get<0>(this->tile_nodes[i]).y % 2))
	    ^ (j % 2)) {
		material_top.EmissiveColor.set(colour1[0], colour1[1], colour1[2], colour1[3]);
	} else {
		material_top.EmissiveColor.set(colour2[0], colour2[1], colour2[2], colour2[3]);
	}
}


inline void SystemStageRenderer::linePattern(irr::video::IVideoDriver *  driver,
                                             irr::scene::IMeshSceneNode *node,
                                             irr::video::SMaterial &     material_top,
                                             glm::ivec4                  colour1,
                                             glm::ivec4                  colour2,
                                             int                         orientation,
                                             int                         size,
                                             int                         i,
                                             int                         j)
{
	if ((int) !(std::get<0>(this->tile_nodes[i])[orientation] % size == j)) {
		material_top.EmissiveColor.set(colour1[0], colour1[1], colour1[2], colour1[3]);
	} else {
		material_top.EmissiveColor.set(colour2[0], colour2[1], colour2[2], colour2[3]);
	}
}

inline void SystemStageRenderer::squarePattern(irr::video::IVideoDriver *  driver,
                                               irr::scene::IMeshSceneNode *node,
                                               irr::video::SMaterial &     material_top,
                                               glm::ivec4                  colour1,
                                               glm::ivec4                  colour2,
                                               int                         orientation,
                                               int                         size,
                                               int                         i,
                                               int                         j)
{
	glm::ivec2 centrePoint;
	centrePoint.x =
	  std::get<0>(this->tile_nodes[i]).x - std::get<0>(this->tile_nodes[i]).x % size + size / 2;
	centrePoint.y =
	  std::get<0>(this->tile_nodes[i]).y - std::get<0>(this->tile_nodes[i]).y % size + size / 2;

	int dx = abs(std::get<0>(this->tile_nodes[i]).x - centrePoint.x);
	int dy = abs(std::get<0>(this->tile_nodes[i]).y - centrePoint.y);

	if ((((dx % 2 == 0 && dy == 0) || (dx > 0 && dx % 2 == 0 && dy < dx)) ||  // columns
	     ((dy % 2 == 0 && dx == 0) || (dy > 0 && dy % 2 == 0 && dx < dy)) ||  // rows
	     (dx == dy && dx % 2 == 0 && dy % 2 == 0))                            // corners
	    ^ (j % 2)) {                                                          // alternating

		material_top.EmissiveColor.set(colour1[0], colour1[1], colour1[2], colour1[3]);
	} else {
		material_top.EmissiveColor.set(colour2[0], colour2[1], colour2[2], colour2[3]);
	}
}

}  // namespace client
