#include <client/system/SystemStageRenderer.hpp>

#include <tempo/component/ComponentStage.hpp>

#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>
#include <irrlicht.h>  // :TODO: sort out includes


#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <iostream>

namespace client
{

inline void SystemStageRenderer::addFloorTilesToScene(irr::scene::ISceneManager *smgr,
                                         irr::video::IVideoDriver * driver,
                                         tempo::ComponentStage& stage)
{

  auto tiles = stage.getHeights();

  if(tile_nodes.size() == tiles.size()) return;

  mesh = smgr->getMesh("resources/meshes/tile.obj");
  if (!mesh) {
    std::cout << "Failed to load floor tile mesh\n" << std::endl;
    return;
  }

	this->wall_diffuse_map =
	  driver->getTexture("resources/materials/walls/cobblestone.png");
	this->wall_normal_map =
	  driver->getTexture("resources/materials/walls/cobblestone_n.png");
	this->tile_texture =
	  driver->getTexture("resources/materials/TileLightMaskPixelOn.png");

		batchMesh = new irr::scene::CBatchingMesh();

		mesh->getMeshBuffer(1)->getMaterial().setTexture(0, this->tile_texture);
		mesh->getMeshBuffer(1)->getMaterial().DiffuseColor.set(255, 10, 10, 10);
		mesh->getMeshBuffer(0)->getMaterial().setTexture(0, wall_diffuse_map);
		mesh->getMeshBuffer(0)->getMaterial().setTexture(1, wall_normal_map);

	for (unsigned int i = tile_nodes.size(); i < tiles.size(); ++i) {

		float grid_x = tiles[i].position.x;
		float grid_y = tiles[i].position.y;
		float height = tiles[i].height;

		if (height >= 5) {
			continue;
		}

		batchMesh->addMesh(mesh, irr::core::vector3df(grid_x, height, grid_y));

		tile_nodes.push_back(std::make_tuple(glm::ivec2(grid_y, grid_x), nullptr));
		old_positions.push_back(tempo::stage_tile(glm::ivec2(grid_y, grid_x), height));
		fractions.push_back(0.00001f);

  //  //TODO: Implement a getNeighbours function
  //  if(stage.getHeight(tiles[i].position - glm::ivec2(0,1)) < height ||
  //      stage.getHeight(tiles[i].position - glm::ivec2(0,-1)) < height ||
  //      stage.getHeight(tiles[i].position - glm::ivec2(-1,0)) < height ||
  //      stage.getHeight(tiles[i].position - glm::ivec2(1,0)) < height ) {
		//}
	}

	mesh->getMeshBuffer(1)->getMaterial().setTexture(0, nullptr);
	mesh->getMeshBuffer(1)->getMaterial().DiffuseColor.set(255, 10, 10, 10);
	mesh->getMeshBuffer(0)->getMaterial().setTexture(0, this->wall_diffuse_map);
	mesh->getMeshBuffer(0)->getMaterial().setTexture(1, this->wall_normal_map);

	for (unsigned int i = tile_nodes.size(); i < tiles.size(); ++i) {

		float grid_x = tiles[i].position.x;
		float grid_y = tiles[i].position.y;
		float height = tiles[i].height;

		if (height < 5) {
			continue;
		}



		batchMesh->addMesh(mesh, irr::core::vector3df(grid_x, height, grid_y));

		tile_nodes.push_back(std::make_tuple(glm::ivec2(grid_y, grid_x), nullptr));
		old_positions.push_back(tempo::stage_tile(glm::ivec2(grid_y, grid_x), height));
		fractions.push_back(0.00001f);
	}


	batchMesh->update();
	this->node = smgr->addMeshSceneNode(batchMesh, 0);
	batchMesh->drop();
}


void SystemStageRenderer::setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver)
{
	printf("SystemStageRenderer initializing\n");

	auto  entities = getEntities();
	auto  entity   = std::begin(entities);
	auto &stage    = entity->getComponent<tempo::ComponentStage>();

	//tile_nodes =
	addFloorTilesToScene(smgr, driver, stage); 
}

void SystemStageRenderer::updateStage(glm::ivec4                colour1,
                                      glm::ivec4                colour2,
									  irr::scene::ISceneManager *smgr,
                                      irr::video::IVideoDriver *driver,
                                      int                       j,
                                      glm::ivec2                playerpos)
{
	
	irr::scene::ISceneNode* par = this->node->getParent();
	par->removeChild(this->node);
	auto  entities = getEntities();
	auto  entity   = std::begin(entities);
	auto &stage    = entity->getComponent<tempo::ComponentStage>();

	auto heights = stage.getHeights();

	batchMesh = new irr::scene::CBatchingMesh();

	mesh->getMeshBuffer(1)->getMaterial().setTexture(0, this->tile_texture);
	mesh->getMeshBuffer(1)->getMaterial().EmissiveColor.set(colour1[0], colour1[1], colour1[2], colour1[3]);

	for (unsigned int i = 0; i < this->tile_nodes.size(); ++i) {

		glm::ivec2 pos = std::get<0>(tile_nodes[i]);

		//if (pos.x == 5 && pos.y == 5) {
		//	printf("Before C1!\n");
		//}

		if(pos.y < playerpos.x - 30 ||
			pos.y > playerpos.x + 30 ||
			pos.x < playerpos.y - 30 ||
			pos.x > playerpos.y + 30) {
			//printf("(%d,%d)\n", pos.y, pos.x);
			continue;
		}

		//if (pos.x == 5 && pos.y == 5) {
		//	printf("After C1!\n");
		//}


   // auto animation_pos = node->getPosition();
    float old_height = old_positions[i].height;

    float height = heights[i].height;

	//if (old_height != height) {

	//	if (animation_pos.Y < height - 1.f || height + 1.f < animation_pos.Y) {
	//		animation_pos.Y += (height - old_height) * fractions[i];
	//		fractions[i] = fractions[i] * 1.2f;
	//		//node->setPosition(irr::core::vector3df(animation_pos.X, animation_pos.Y, animation_pos.Z));

	//		if (animation_pos.Y < height - 1.f || height + 1.f < animation_pos.Y) {
	//			continue;
	//}
	//}
	//else {
	//		//node->setPosition(irr::core::vector3df(animation_pos.X, height, animation_pos.Z));
	//old_height = height;
	//		fractions[i] = 0.00001f;
	//}
	//}

	//irr::video::SMaterial &material_side = node->getMaterial(0);
	//irr::video::SMaterial &material_top  = node->getMaterial(1);

		//irr::video::SMaterial &material_side = node->getMaterial(0);
		//irr::video::SMaterial &material_top  = node->getMaterial(1);

	bool render;

		switch (j) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 9:
		case 10:
		case 11:
		case 12:render = checkerBoardPattern(driver, colour1, colour2, i, j); break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:render = linePattern(driver, colour1, colour2, 0, 5, i, j - 4); break;
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:render = linePattern(driver, colour1, colour2, 1, 5, i, j - 13); break;
		case 18:
		case 19:
		case 20:
		case 21:render = squarePattern(driver, colour1, colour2, 1, 12, i, j); break;
		}

		if (render) {
			
			batchMesh->addMesh(mesh, irr::core::vector3df(pos.y, height, pos.x));

		}
	}

	mesh->getMeshBuffer(1)->getMaterial().setTexture(0, this->tile_texture);
	mesh->getMeshBuffer(1)->getMaterial().EmissiveColor.set(colour2[0], colour2[1], colour2[2], colour2[3]);

	for (unsigned int i = 0; i < this->tile_nodes.size(); ++i) {

		glm::ivec2 pos = std::get<0>(tile_nodes[i]);

		//if (pos.x == 5 && pos.y == 5) {
		//	printf("Before C2!\n");
		//}

		if (pos.y < playerpos.x - 30 ||
			pos.y > playerpos.x + 30 ||
			pos.x < playerpos.y - 30 ||
			pos.x > playerpos.y + 30) {
			//printf("(%d,%d)\n", pos.y, pos.x);
			continue;
		}

		//if (pos.x == 5 && pos.y == 5) {
		//	printf("After C2!\n");
		//}

		//auto animation_pos = node->getPosition();
		float old_height = old_positions[i].height;

		float height = heights[i].height;

		//if (old_height != height) {

		//	if (animation_pos.Y < height - 1.f || height + 1.f < animation_pos.Y) {
		//		animation_pos.Y += (height - old_height) * fractions[i];
		//		fractions[i] = fractions[i] * 1.2f;
		//		//node->setPosition(irr::core::vector3df(animation_pos.X, animation_pos.Y, animation_pos.Z));

		//		if (animation_pos.Y < height - 1.f || height + 1.f < animation_pos.Y) {
		//			continue;
				//}
			//}
			//else {
		//		//node->setPosition(irr::core::vector3df(animation_pos.X, height, animation_pos.Z));
				//old_height = height;
		//		fractions[i] = 0.00001f;
			//}
		//}

		//irr::video::SMaterial &material_side = node->getMaterial(0);
		//irr::video::SMaterial &material_top  = node->getMaterial(1);

		bool render;

		switch (j) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 9:
		case 10:
		case 11:
		case 12:render = checkerBoardPattern(driver, colour1, colour2, i, j); break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:render = linePattern(driver, colour1, colour2, 0, 5, i, j - 4); break;
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:render = linePattern(driver, colour1, colour2, 1, 5, i, j - 13); break;
		case 18:
		case 19:
		case 20:
		case 21:render = squarePattern(driver, colour1, colour2, 1, 12, i, j); break;
		}

		if (!render) {
			
			batchMesh->addMesh(mesh, irr::core::vector3df(pos.y, height, pos.x));

		}
	}

	batchMesh->update();
	this->node = smgr->addMeshSceneNode(batchMesh, 0);
	batchMesh->drop();

}

inline bool SystemStageRenderer::checkerBoardPattern(irr::video::IVideoDriver *  driver,
                                                     glm::ivec4                  colour1,
                                                     glm::ivec4                  colour2,
                                                     int                         i,
                                                     int                         j)
{
	return (((int)(std::get<0>(this->tile_nodes[i]).x % 2)
		== (int)(std::get<0>(this->tile_nodes[i]).y % 2))
		^ (j % 2));

}


inline bool SystemStageRenderer::linePattern(irr::video::IVideoDriver *  driver,
                                             glm::ivec4                  colour1,
                                             glm::ivec4                  colour2,
                                             int                         orientation,
                                             int                         size,
                                             int                         i,
                                             int                         j)
{
	return ((int)!(std::get<0>(this->tile_nodes[i])[orientation] % size == j));
}

inline bool SystemStageRenderer::squarePattern(irr::video::IVideoDriver *  driver,
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

	return ((((dx % 2 == 0 && dy == 0) || (dx > 0 && dx % 2 == 0 && dy < dx)) ||  // columns
		((dy % 2 == 0 && dx == 0) || (dy > 0 && dy % 2 == 0 && dx < dy)) ||  // rows
		(dx == dy && dx % 2 == 0 && dy % 2 == 0))                            // corners
		^ (j % 2));
}

}  // namespace client
