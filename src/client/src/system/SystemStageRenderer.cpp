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
                                                      tempo::ComponentStage &    stage)
{
	auto tiles = stage.getHeights();

	if (tileMap.size() == tiles.size())
		return;

	this->wall_diffuse_map = driver->getTexture("resources/materials/walls/cobblestone.png");
	this->wall_normal_map  = driver->getTexture("resources/materials/walls/cobblestone_n.png");
	this->tile_texture     = driver->getTexture("resources/materials/TileLightMaskPixelOn.png");

	mesh = smgr->getMeshManipulator()->createMeshCopy(smgr->getMesh("resources/meshes/tile.obj"));
	walls = smgr->getMeshManipulator()->createMeshCopy(smgr->getMesh("resources/meshes/tile.obj"));

	this->mesh->getMeshBuffer(1)->getMaterial().setTexture(0, this->tile_texture);
	// this->mesh->getMeshBuffer(1)->getMaterial().EmissiveColor = c1;
	this->mesh->getMeshBuffer(0)->getMaterial().setTexture(0, wall_diffuse_map);
	this->mesh->getMeshBuffer(0)->getMaterial().setTexture(1, wall_normal_map);

	this->walls->getMeshBuffer(1)->getMaterial().setTexture(0, this->tile_texture);
	this->walls->getMeshBuffer(1)->getMaterial().DiffuseColor.set(255, 10, 10, 10);
	this->walls->getMeshBuffer(0)->getMaterial().setTexture(0, wall_diffuse_map);
	this->walls->getMeshBuffer(0)->getMaterial().setTexture(1, wall_normal_map);

	for (auto tile : tiles)
	{
		glm::ivec2 pos = tile.position;
		tileMap[pos].pos = pos;
		tileMap[pos].height = tile.height;
		tileMap[pos].height_target = tile.height;
	}
}

void SystemStageRenderer::setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver)
{
	printf("SystemStageRenderer initializing\n");

	auto  entities = getEntities();
	auto  entity   = std::begin(entities);
	auto &stage    = entity->getComponent<tempo::ComponentStage>();

	// tile_nodes =
	addFloorTilesToScene(smgr, driver, stage);
}

void SystemStageRenderer::setTileColor(glm::ivec2 pos, irr::video::SColor color)
{
	tileMap[pos].color = color;
}

void SystemStageRenderer::colorStage(int                        step,
                                     glm::ivec2                 playerpos,
                                     irr::video::SColor         C1,
				     irr::video::SColor         C2)
{
	auto  entities = getEntities();
	auto  entity   = std::begin(entities);
	auto &stage    = entity->getComponent<tempo::ComponentStage>();

	auto heights = stage.getHeights();

	int index = 0;
	for (auto& it : tileMap)
	{
		tile_t tile = it.second;
	// for (unsigned int i = 0; i < this->tile_nodes.size(); ++i) {
		glm::ivec2 pos = tile.pos;

		// Stop logic on tiles that are not visible to camera
		if (pos.y < playerpos.x - 24 || pos.y > playerpos.x + 7 || pos.x < playerpos.y - 33
		    || pos.x > playerpos.y + 33) {
			continue;
		}

		bool render;

		switch (step) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 9:
		case 10:
		case 11:
		case 12: render = checkerBoardPattern(pos, step); break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8: render = linePattern(0, 5, pos, step - 4); break;
		case 13:
		case 14:
		case 15:
		case 16:
		case 17: render = linePattern(1, 5, i, step - 13); break;
		case 18:
		case 19:
		case 20:
		case 21: render = squarePattern(1, 12, pos, step); break;
		}

		if (render) {
			setTileColor(pos, C1);
			continue;
		}
		else{
			setTileColor(pos, C2);
			continue;
		}
	}

}

void SystemStageRenderer::Update(irr::scene::ISceneManager *smgr,
                                 irr::video::IVideoDriver * driver)
{
	batchMesh = new irr::scene::CBatchingMesh();
	irr::scene::ISceneNode *par = this->node->getParent();
	par->removeChild(this->node);

	bool firstRun = false;

	for (unsigned int i = 0; i < this->tile_nodes.size(); ++i) {
		glm::ivec2 pos = tile_nodes[i];

		float old_height = old_positions[i].height;

		if (heightMap.find(pos) == heightMap.end()) {
			heightMap[pos] = old_height;
		}

		float height = heightMap[pos];
		auto animation_pos = currentHeight[i];

		if (old_height != height) {
			if (animation_pos < height - 1.f || height + 1.f < animation_pos) {
				animation_pos += (height - old_height) * fractions[i];
				fractions[i]           = fractions[i] * 1.15f;
				heightMap[pos] = animation_pos;

			} else {
				heightMap[pos] = height;
				old_height             = height;
				fractions[i]           = 0.00001f;
			}
		}

		if (currentHeight[i] >= 5) {
			batchMesh->addMesh(walls, irr::core::vector3df(pos.y, currentHeight[i], pos.x));
			continue;
		}
	}

	for (auto& it : colorDeltaMap)
	{
		glm::ivec2 p = it.first;
		irr::video::SColor c = it.second;
		this->mesh->getMeshBuffer(1)->getMaterial().EmissiveColor = c;
		batchMesh->addMesh(mesh, irr::core::vector3df(p.y, currentHeight[i], p.x));
	}

	batchMesh->update();
	this->node = smgr->addMeshSceneNode(batchMesh, 0);
	batchMesh->drop();
}

inline bool SystemStageRenderer::checkerBoardPattern(glm::ivec2 pos, int step)
{
	return (pos.x % 2 == pos.y % 2) ^ (step % 2);
}


inline bool SystemStageRenderer::linePattern(int orientation, int size, glm::ivec2 pos, int step)
{
	return !(pos[orientation] % size == step);
}

inline bool SystemStageRenderer::squarePattern(int                       orientation,
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
