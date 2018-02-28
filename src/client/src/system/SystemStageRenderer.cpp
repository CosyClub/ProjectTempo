#include <client/system/SystemStageRenderer.hpp>

#include <tempo/component/ComponentStage.hpp>

#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>
#include <irrlicht.h> // :TODO: sort out includes

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <iostream>

namespace {
	client::stage_nodes addFloorTilesToScene(irr::scene::ISceneManager* smgr,
	                          irr::video::IVideoDriver*  driver,
	                          irr::scene::IMesh* mesh,
	                          tempo::stage_tiles& tiles) {

		client::stage_nodes temp_nodes;

	  for(unsigned int i = 0; i < tiles.size(); ++i){

		  irr::scene::IMeshSceneNode* node = smgr->addMeshSceneNode(mesh, 0);

		  float grid_x = std::get<0>(tiles[i]).x;
		  float grid_y = std::get<0>(tiles[i]).y;
		  float height = std::get<1>(tiles[i]);

			temp_nodes.push_back(std::make_tuple(glm::ivec2(grid_y, grid_x), node));

		  node->setPosition(irr::core::vector3df(grid_x, height, grid_y));

		  printf("Floor mat count: %i\n", node->getMaterialCount());

		  irr::video::SMaterial& material_side = node->getMaterial(0);
		  irr::video::SMaterial& material_top  = node->getMaterial(1);

		  material_top.Shininess = 0.0f;
		  material_top.SpecularColor.set(255, 0, 0, 0);
		  material_top.DiffuseColor.set(255, 10,   10,   10);
	  }

		return temp_nodes;
	}
}

namespace client {
	void SystemStageRenderer::setup(irr::scene::ISceneManager* smgr,
	                                irr::video::IVideoDriver*  driver){
		printf("SystemStageRenderer initializing\n");

		auto entities = getEntities();
		auto entity = std::begin(entities);
		auto& stage   = entity->getComponent<tempo::ComponentStage>();

		tempo::stage_tiles tiles = stage.getHeights();

		irr::scene::IMesh* mesh_floor_tile = smgr->getMesh("resources/meshes/tile.obj");
		if (!mesh_floor_tile){
			std::cout << "Failed to load floor tile mesh\n" << std::endl;
			return;
		}

		tile_nodes = addFloorTilesToScene(smgr, driver, mesh_floor_tile, tiles);
	}

	void SystemStageRenderer::updateStage(glm::ivec4 colour1, glm::ivec4 colour2, irr::video::IVideoDriver*  driver, int j) {

		auto entities = getEntities();
		auto entity = std::begin(entities);
		auto& stage   = entity->getComponent<tempo::ComponentStage>();

		std::vector<std::tuple<glm::ivec2, float>> heights = stage.getHeights();

		for(unsigned int i = 0; i < this->tile_nodes.size(); ++i){
				irr::scene::IMeshSceneNode* node = std::get<1>(this->tile_nodes[i]);

				if(std::get<1>(heights[i]) >= 5){
					continue;
				}

				irr::video::SMaterial& material_side = node->getMaterial(0);
				irr::video::SMaterial& material_top  = node->getMaterial(1);

				material_top.Shininess = 0.0f;
			  material_top.SpecularColor.set(255, 0, 0, 0);
			  material_top.DiffuseColor.set(255, 200,   0,   0);

			switch (j) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 9:
			case 10:
			case 11:
			case 12: {
				checkerBoardPattern(driver, material_top, colour1, colour2, i, j);
				break;
			}
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			 {
				linePattern(driver, material_top, colour1, colour2, 0, 5, i, j-4);
				break;
			}
			case 13:
			case 14:
			case 15:
			case 16:
			case 17: {
				linePattern(driver, material_top, colour1, colour2, 1, 5, i, j-13);
				break;
			}
			case 18:
			case 19:
			case 20:
			case 21: {
				squarePattern(driver, material_top, colour1, colour2, 1, 12, i, j);
				break;
			}
		 }
		}
	}

	void SystemStageRenderer::checkerBoardPattern(irr::video::IVideoDriver*  driver,
											 												  irr::video::SMaterial& material_top,
																								glm::ivec4 colour1,
																								glm::ivec4 colour2,
																								int i,
																						    int j) {

		if(((int)(std::get<0>(this->tile_nodes[i]).x  % 2) == (int)(std::get<0>(this->tile_nodes[i]).y % 2)) ^ (j % 2)) {
			material_top.setTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
			material_top.EmissiveColor.set(colour1[0], colour1[1], colour1[2], colour1[3]);
		} else {
			material_top.setTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
			material_top.EmissiveColor.set(colour2[0], colour2[1], colour2[2], colour2[3]);
		}

	}


	void SystemStageRenderer::linePattern(irr::video::IVideoDriver*  driver,
																				irr::video::SMaterial& material_top,
																				glm::ivec4 colour1,
																				glm::ivec4 colour2,
																				int orientation,
																				int size,
																				int i,
																				int j) {

	if((int)!(std::get<0>(this->tile_nodes[i])[orientation] % size == j)){
		material_top.setTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
		material_top.EmissiveColor.set(colour1[0], colour1[1], colour1[2], colour1[3]);
	} else {
		material_top.setTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
		material_top.EmissiveColor.set(colour2[0], colour2[1], colour2[2], colour2[3]);
	}
}

	void SystemStageRenderer::squarePattern(irr::video::IVideoDriver*  driver,
																					irr::video::SMaterial& material_top,
																					glm::ivec4 colour1,
																					glm::ivec4 colour2,
																					int orientation,
																					int size,
																					int i,
																					int j) {

  glm::ivec2 centrePoint;
	centrePoint.x = std::get<0>(this->tile_nodes[i]).x - std::get<0>(this->tile_nodes[i]).x % size + size / 2;
	centrePoint.y = std::get<0>(this->tile_nodes[i]).y - std::get<0>(this->tile_nodes[i]).y % size + size / 2;

	int dx = abs(std::get<0>(this->tile_nodes[i]).x - centrePoint.x);
	int dy = abs(std::get<0>(this->tile_nodes[i]).y - centrePoint.y);

	if((((dx%2 ==0 && dy ==0) || (dx > 0 && dx %2 ==0 && dy<dx))|| //columns
		((dy%2 ==0 && dx ==0) || (dy > 0 && dy %2 ==0 && dx<dy))|| //rows
		(dx == dy && dx %2 ==0 && dy %2 ==0)) //corners
		^ (j%2)){ // alternating

		material_top.setTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
		material_top.EmissiveColor.set(colour1[0], colour1[1], colour1[2], colour1[3]);
	} else {
		material_top.setTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
		material_top.EmissiveColor.set(colour2[0], colour2[1], colour2[2], colour2[3]);
	}
}

} // namespace client
