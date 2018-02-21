#include <client/system/SystemStageRenderer.hpp>

#include <tempo/component/ComponentStage.hpp>

#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>
#include <irrlicht.h> // :TODO: sort out includes

#include <glm/vec2.hpp>

#include <iostream>

namespace {
	void addFloorTilesToScene(irr::scene::ISceneManager* smgr,
	                          irr::scene::IMesh* mesh,
	                          tempo::stage_tiles& tiles) {
	  for(unsigned int i = 0; i < tiles.size(); ++i){

		  irr::scene::IMeshSceneNode* node = smgr->addMeshSceneNode(mesh, 0);
		  float grid_x = std::get<0>(tiles[i]).x;
		  float grid_y = std::get<0>(tiles[i]).y;
		  float height = std::get<1>(tiles[i]);

		  node->setPosition(irr::core::vector3df(grid_x, height, grid_y));
	  }
	}
}

namespace client {
	void SystemStageRenderer::setup(irr::scene::ISceneManager* smgr){
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

		addFloorTilesToScene(smgr, mesh_floor_tile, tiles);
	}
} // namespace client
