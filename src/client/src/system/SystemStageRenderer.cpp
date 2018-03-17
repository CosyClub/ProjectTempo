#include <client/system/SystemStageRenderer.hpp>

#include <tempo/component/ComponentStage.hpp>

#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>
#include <irrlicht.h>  // :TODO: sort out includes


#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <vector>

namespace client
{
inline void SystemStageRenderer::addFloorTilesToScene(irr::scene::ISceneManager *smgr,
                                                      irr::video::IVideoDriver * driver,
                                                      tempo::ComponentStage &    stage,
                                                      glm::ivec4 colour1, glm::ivec4 colour2)
{
	auto tiles = stage.getHeights();

	if (tile_nodes.size() == tiles.size())
		return;


  this->wall_diffuse_map = driver->getTexture("resources/materials/walls/cobblestone.png");
  this->wall_normal_map  = driver->getTexture("resources/materials/walls/cobblestone_n.png");
  this->tile_texture     = driver->getTexture("resources/materials/TileLightMaskPixelOn.png");

	meshC1 = smgr->getMeshManipulator()->createMeshCopy(smgr->getMesh("resources/meshes/tile.obj"));
  meshC2 = smgr->getMeshManipulator()->createMeshCopy(smgr->getMesh("resources/meshes/tile.obj"));
  walls = smgr->getMeshManipulator()->createMeshCopy(smgr->getMesh("resources/meshes/tile.obj"));

	batchMesh = new irr::scene::CBatchingMesh();

	this->meshC1->getMeshBuffer(1)->getMaterial().setTexture(0, this->tile_texture);
	this->meshC1->getMeshBuffer(1)->getMaterial().EmissiveColor.set(colour1[0], colour1[1], colour1[2], colour1[3]);
	this->meshC1->getMeshBuffer(0)->getMaterial().setTexture(0, wall_diffuse_map);
	this->meshC1->getMeshBuffer(0)->getMaterial().setTexture(1, wall_normal_map);

  this->meshC2->getMeshBuffer(1)->getMaterial().setTexture(0, this->tile_texture);
	this->meshC2->getMeshBuffer(1)->getMaterial().EmissiveColor.set(colour2[0], colour2[1], colour2[2], colour2[3]);
	this->meshC2->getMeshBuffer(0)->getMaterial().setTexture(0, wall_diffuse_map);
	this->meshC2->getMeshBuffer(0)->getMaterial().setTexture(1, wall_normal_map);

  this->walls->getMeshBuffer(1)->getMaterial().setTexture(0, this->tile_texture);
	this->walls->getMeshBuffer(1)->getMaterial().DiffuseColor.set(255, 10, 10, 10);
	this->walls->getMeshBuffer(0)->getMaterial().setTexture(0, wall_diffuse_map);
	this->walls->getMeshBuffer(0)->getMaterial().setTexture(1, wall_normal_map);

	for (unsigned int i = tile_nodes.size(); i < tiles.size(); ++i) {
		float grid_x = tiles[i].position.x;
		float grid_y = tiles[i].position.y;
		float height = tiles[i].height;

		batchMesh->addMesh(walls, irr::core::vector3df(grid_x, height, grid_y));

		tile_nodes.push_back(glm::ivec2(grid_y, grid_x));
		old_positions.push_back(tempo::stage_tile(glm::ivec2(grid_y, grid_x), height));
		fractions.push_back(0.00001f);
	}

	batchMesh->update();
	this->node = smgr->addMeshSceneNode(batchMesh, 0);
	batchMesh->drop();
}


void SystemStageRenderer::setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver, glm::ivec4 colour1, glm::ivec4 colour2)
{
	printf("SystemStageRenderer initializing\n");

	auto  entities = getEntities();
	auto  entity   = std::begin(entities);
	auto &stage    = entity->getComponent<tempo::ComponentStage>();

	// tile_nodes =
	addFloorTilesToScene(smgr, driver, stage, colour1, colour2);
}

void SystemStageRenderer::updateStage(irr::scene::ISceneManager *smgr,
                                      irr::video::IVideoDriver * driver,
                                      int                        j,
                                      glm::ivec2                 playerpos)
{
	irr::scene::ISceneNode *par = this->node->getParent();
	par->removeChild(this->node);

	bool firstRun = false;

	if (this->currentHeight.size() <= 1) {
		this->currentHeight = std::vector<float>(this->tile_nodes.size());
		firstRun            = true;
	}

	auto  entities = getEntities();
	auto  entity   = std::begin(entities);
	auto &stage    = entity->getComponent<tempo::ComponentStage>();

	auto heights = stage.getHeights();

	batchMesh = new irr::scene::CBatchingMesh();

	for (unsigned int i = 0; i < this->tile_nodes.size(); ++i) {
		glm::ivec2 pos = tile_nodes[i];

		float old_height = old_positions[i].height;

		if (firstRun) {
			this->currentHeight[i] = old_height;
		}

		float height = heights[i].height;

		// qqii:: uncommented this block with no problems, is this legacy???
		if (pos.y < playerpos.x - 30 || pos.y > playerpos.x + 13 || pos.x < playerpos.y - 30
		    || pos.x > playerpos.y + 30) {
			continue;
		}

		auto animation_pos = currentHeight[i];

		if (old_height != height) {
			if (animation_pos < height - 1.f || height + 1.f < animation_pos) {
				animation_pos += (height - old_height) * fractions[i];
				fractions[i]           = fractions[i] * 1.15f;
				this->currentHeight[i] = animation_pos;

			} else {
				this->currentHeight[i] = height;
				old_height             = height;
				fractions[i]           = 0.00001f;
			}
		}

    	if (currentHeight[i] >= 5) {
        batchMesh->addMesh(walls, irr::core::vector3df(pos.y, currentHeight[i], pos.x));
    		continue;
    	}

		bool render;

		switch (j) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 9:
		case 10:
		case 11:
		case 12: render = checkerBoardPattern(driver, i, j); break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8: render = linePattern(driver, 0, 5, i, j - 4); break;
		case 13:
		case 14:
		case 15:
		case 16:
		case 17: render = linePattern(driver, 1, 5, i, j - 13); break;
		case 18:
		case 19:
		case 20:
		case 21: render = squarePattern(driver, 1, 12, i, j); break;
		}

		if (render) {
			batchMesh->addMesh(meshC1, irr::core::vector3df(pos.y, currentHeight[i], pos.x));
      continue;
		}
    else{
      batchMesh->addMesh(meshC2, irr::core::vector3df(pos.y, currentHeight[i], pos.x));
      continue;
    }
	}

	firstRun = false;


	batchMesh->update();
	this->node = smgr->addMeshSceneNode(batchMesh, 0);
	batchMesh->drop();
}

inline bool SystemStageRenderer::checkerBoardPattern(irr::video::IVideoDriver *driver,
                                                     int                       i,
                                                     int                       j)
{
	return (((int) ((this->tile_nodes[i]).x % 2) == (int) ((this->tile_nodes[i]).y % 2)) ^ (j % 2));
}


inline bool SystemStageRenderer::linePattern(irr::video::IVideoDriver *driver,
                                             int                       orientation,
                                             int                       size,
                                             int                       i,
                                             int                       j)
{
	return ((int) !((this->tile_nodes[i])[orientation] % size == j));
}

inline bool SystemStageRenderer::squarePattern(irr::video::IVideoDriver *driver,
                                               int                       orientation,
                                               int                       size,
                                               int                       i,
                                               int                       j)
{
	glm::ivec2 centrePoint;
	centrePoint.x = (this->tile_nodes[i]).x - (this->tile_nodes[i]).x % size + size / 2;
	centrePoint.y = (this->tile_nodes[i]).y - (this->tile_nodes[i]).y % size + size / 2;

	int dx = abs((this->tile_nodes[i]).x - centrePoint.x);
	int dy = abs((this->tile_nodes[i]).y - centrePoint.y);

	return ((((dx % 2 == 0 && dy == 0) || (dx > 0 && dx % 2 == 0 && dy < dx)) ||  // columns
	         ((dy % 2 == 0 && dx == 0) || (dy > 0 && dy % 2 == 0 && dx < dy)) ||  // rows
	         (dx == dy && dx % 2 == 0 && dy % 2 == 0))                            // corners
	        ^ (j % 2));
}

}  // namespace client
