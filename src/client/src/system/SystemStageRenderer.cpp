#include <client/system/SystemStageRenderer.hpp>

#include <tempo/component/ComponentStage.hpp>

#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>
#include <irrlicht.h> // :TODO: sort out includes

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

		  irr::video::SMaterial& material_side = node->getMaterial(0);
		  irr::video::SMaterial& material_top  = node->getMaterial(1);

		  material_top.Shininess = 0.0f;
		  material_top.SpecularColor.set(255, 0, 0, 0);
		  material_top.DiffuseColor.set(255, 200,   0,   0);
		  if((int)grid_x % 2 == (int)grid_y % 2){
			  node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
			  material_top.EmissiveColor.set(255, 200,   0,   0);
		  } else {
			  node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
		  }
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

	void SystemStageRenderer::setColour(glm::ivec2 pos, irr::video::IVideoDriver*  driver, int j){
		for(unsigned int i = 0; i < this->tile_nodes.size(); ++i){
				irr::scene::IMeshSceneNode* node = std::get<1>(this->tile_nodes[i]);

				irr::video::SMaterial& material_side = node->getMaterial(0);
				irr::video::SMaterial& material_top  = node->getMaterial(1);

				material_top.Shininess = 0.0f;
			  material_top.SpecularColor.set(255, 0, 0, 0);
			  material_top.DiffuseColor.set(255, 200,   0,   0);
			if(j==0){
			  if((int)(std::get<0>(this->tile_nodes[i]).x  % 2) == (int)(std::get<0>(this->tile_nodes[i]).y % 2)){
				  node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
				  material_top.EmissiveColor.set(255, 200,   0,   0);
			  } else {
				  node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
					material_top.EmissiveColor.set(255, 0,   200,   0);
			  }
			}
			else if(j==1) {
				if((int)(std::get<0>(this->tile_nodes[i]).x  % 2) != (int)(std::get<0>(this->tile_nodes[i]).y % 2)){
				  node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
				  material_top.EmissiveColor.set(255, 200,   0,   0);
			  } else {
				  node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
					material_top.EmissiveColor.set(255, 0,   200,   0);
			  }
			}
			else if(j==2) {
				if((int)!(std::get<0>(this->tile_nodes[i]).x % 5 == 0)){
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
					material_top.EmissiveColor.set(255, 200,   0,   0);
				} else {
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
					material_top.EmissiveColor.set(255, 0,   200,   0);
				}
			}
			else if(j==3) {
				if((int)!(std::get<0>(this->tile_nodes[i]).x % 5 == 1)){
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
					material_top.EmissiveColor.set(255, 200,   0,   0);
				} else {
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
					material_top.EmissiveColor.set(255, 0,   200,   0);
				}
			}
			else if(j==4) {
				if((int)!(std::get<0>(this->tile_nodes[i]).x % 5 == 2)){
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
					material_top.EmissiveColor.set(255, 200,   0,   0);
				} else {
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
					material_top.EmissiveColor.set(255, 0,   200,   0);
				}
			}
			else if(j==5) {
				if((int)!(std::get<0>(this->tile_nodes[i]).x % 5 == 3)){
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
					material_top.EmissiveColor.set(255, 200,   0,   0);
				} else {
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
					material_top.EmissiveColor.set(255, 0,   200,   0);
				}
			}
			else if(j==6) {
				if((int)!(std::get<0>(this->tile_nodes[i]).x % 5 == 4)){
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
					material_top.EmissiveColor.set(255, 200,   0,   0);
				} else {
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
					material_top.EmissiveColor.set(255, 0,   200,   0);
				}
			}
			if(j==7){
				if((int)(std::get<0>(this->tile_nodes[i]).x  % 2) == (int)(std::get<0>(this->tile_nodes[i]).y % 2)){
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
					material_top.EmissiveColor.set(255, 200,   0,   0);
				} else {
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
					material_top.EmissiveColor.set(255, 0,   200,   0);
				}
			}
			else if(j==8) {
				if((int)(std::get<0>(this->tile_nodes[i]).x  % 2) != (int)(std::get<0>(this->tile_nodes[i]).y % 2)){
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
					material_top.EmissiveColor.set(255, 200,   0,   0);
				} else {
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
					material_top.EmissiveColor.set(255, 0,   200,   0);
				}
			}
			else if(j==9) {
				if((int)!(std::get<0>(this->tile_nodes[i]).y % 5 == 0)){
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
					material_top.EmissiveColor.set(255, 200,   0,   0);
				} else {
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
					material_top.EmissiveColor.set(255, 0,   200,   0);
				}
			}
			else if(j==10) {
				if((int)!(std::get<0>(this->tile_nodes[i]).y % 5 == 1)){
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
					material_top.EmissiveColor.set(255, 200,   0,   0);
				} else {
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
					material_top.EmissiveColor.set(255, 0,   200,   0);
				}
			}
			else if(j==11) {
				if((int)!(std::get<0>(this->tile_nodes[i]).y % 5 == 2)){
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
					material_top.EmissiveColor.set(255, 200,   0,   0);
				} else {
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
					material_top.EmissiveColor.set(255, 0,   200,   0);
				}
			}
			else if(j==12) {
				if((int)!(std::get<0>(this->tile_nodes[i]).y % 5 == 3)){
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
					material_top.EmissiveColor.set(255, 200,   0,   0);
				} else {
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
					material_top.EmissiveColor.set(255, 0,   200,   0);
				}
			}
			else if(j==13) {
				if((int)!(std::get<0>(this->tile_nodes[i]).y % 5 == 4)){
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskPixelOn.png"));
					material_top.EmissiveColor.set(255, 200,   0,   0);
				} else {
					node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
					material_top.EmissiveColor.set(255, 0,   200,   0);
				}
			}

		}
	}
} // namespace client
