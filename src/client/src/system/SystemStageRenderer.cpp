#include <client/system/SystemStageRenderer.hpp>

#include <tempo/component/ComponentStage.hpp>

#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>
#include <irrlicht.h> // :TODO: sort out includes

#include <glm/vec2.hpp>

#include <iostream>

namespace {
	void addFloorTilesToScene(irr::scene::ISceneManager* smgr,
	                          irr::video::IVideoDriver*  driver,
	                          irr::scene::IMesh* mesh,
	                          tempo::stage_tiles& tiles) {
	  for(unsigned int i = 0; i < tiles.size(); ++i){

		  irr::scene::IMeshSceneNode* node = smgr->addMeshSceneNode(mesh, 0);
		  float grid_x = std::get<0>(tiles[i]).x;
		  float grid_y = std::get<0>(tiles[i]).y;
		  float height = std::get<1>(tiles[i]);

		  node->setPosition(irr::core::vector3df(grid_x, height, grid_y));

		  irr::video::SMaterial& material_side = node->getMaterial(0);
		  irr::video::SMaterial& material_top  = node->getMaterial(1);



		  material_top.Shininess = 0.0f;
		  material_top.SpecularColor.set(255, 0, 0, 0);
		  material_top.DiffuseColor.set(255, 200,   0,   0);
		  if((int)grid_x % 2 == (int)grid_y % 2){
			  node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOn.png"));
			  material_top.EmissiveColor.set(255, 200,   0,   0);
		  } else {
			  node->setMaterialTexture(0, driver->getTexture("resources/materials/TileLightMaskOff.png"));
		  }
	  }
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

		addFloorTilesToScene(smgr, driver, mesh_floor_tile, tiles);
	}
} // namespace client
