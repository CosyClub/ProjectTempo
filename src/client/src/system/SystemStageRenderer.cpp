#include <client/system/SystemStageRenderer.hpp>
#include <client/misc/Color.hpp>

#include <tempo/component/ComponentStage.hpp>

#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>
#include <irrlicht.h>  // :TODO: sort out includes


#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <iostream>


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

	walls = smgr->getMeshManipulator()->createMeshCopy(smgr->getMesh("resources/meshes/tile.obj"));


	this->walls->getMeshBuffer(1)->getMaterial().setTexture(0, this->tile_texture);
	this->walls->getMeshBuffer(1)->getMaterial().DiffuseColor.set(255, 10, 10, 10);
	this->walls->getMeshBuffer(0)->getMaterial().setTexture(0, wall_diffuse_map);
	this->walls->getMeshBuffer(0)->getMaterial().setTexture(1, wall_normal_map);

	irr::video::SColor white(255, 255, 255, 255);
	initColorMesh(white, smgr);

	for (auto tile : tiles)
	{
		glm::ivec2 pos = tile.position;
		tile_t t;
		t.pos = pos;
		t.height = tile.height;
		t.height_target = tile.height;

		tileMap.emplace(pos, t);
	}

	batchMesh = new irr::scene::CBatchingMesh();
	this->node = smgr->addMeshSceneNode(batchMesh, 0);
}

void SystemStageRenderer::initColorMesh(irr::video::SColor color, irr::scene::ISceneManager *smgr)
{
	irr::scene::IMesh *mesh;

	mesh = smgr->getMeshManipulator()->createMeshCopy(smgr->getMesh("resources/meshes/tile.obj"));
	mesh->getMeshBuffer(1)->getMaterial().setTexture(0, this->tile_texture);
	mesh->getMeshBuffer(1)->getMaterial().EmissiveColor = color;
	mesh->getMeshBuffer(0)->getMaterial().setTexture(0, wall_diffuse_map);
	mesh->getMeshBuffer(0)->getMaterial().setTexture(1, wall_normal_map);

	meshMap.emplace(color, mesh);
}

void SystemStageRenderer::setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver)
{
	printf("SystemStageRenderer initializing\n");

	auto&  entities = getEntities();
	auto  entity   = std::begin(entities);
	auto &stage    = entity->getComponent<tempo::ComponentStage>();

	// tile_nodes =
	addFloorTilesToScene(smgr, driver, stage);
}

void SystemStageRenderer::setTileColor(glm::ivec2 pos, irr::video::SColor color)
{
	tileMap[pos].color = color;
}

void SystemStageRenderer::Update(irr::scene::ISceneManager *smgr,
                                 irr::video::IVideoDriver * driver,
                                 glm::ivec2                 playerpos,
                                 irr::video::SColor         C1,
                                 irr::video::SColor         C2,
                                 int                        step,
                                 float                      dt)
{
	batchMesh = new irr::scene::CBatchingMesh();
	irr::scene::ISceneNode *par = this->node->getParent();
	par->removeChild(this->node);

	auto &entities = getEntities();
	auto  entity   = std::begin(entities);
	auto &stage    = entity->getComponent<tempo::ComponentStage>();


	for (auto& it : tileMap) {
		tile_t tile = it.second;
		glm::ivec2 pos = tile.pos;

		if (pos.x < playerpos.x - 24 
		    || pos.x > playerpos.x + 7 
		    || pos.y < playerpos.y - 33
		    || pos.y > playerpos.y + 33) {
			continue;
		}

		if ((*stage.heightDelta).find(pos) != (*stage.heightDelta).end())
		{
			if ((*stage.heightDelta)[pos])
			{
				tileMap[pos].height_target = stage.getHeight(pos);
				(*stage.heightDelta)[pos] = false;
			}
		}

		if (tile.height != tile.height_target)
		{
			float gap = tile.height_target - tile.height;
			float delta = 5.f * (gap / fabs(gap)) * dt / 1.5f; //1.5 seconds to collapse
			if (fabs(delta) > fabs(gap))
			{
				delta = gap;
			}
			tileMap[pos].height += delta;
		}

		if (tile.height >= 5) {
			batchMesh->addMesh(walls, irr::core::vector3df(pos.x, tile.height, pos.y));
			continue;
		}

		int64_t local_step = step % client::palettes.size();
		bool render;

		if (local_step <=  3) {
			render = checkerBoardPattern(pos, local_step);
		} else if (local_step <=  8) {
			render = linePattern(0, 5, pos, local_step - 4);
		} else if (local_step <= 12) {
			render = checkerBoardPattern(pos, local_step);
		} else if (local_step <= 17) {
			render = linePattern(1, 5, pos, local_step - 13);
		} else {
			render = squarePattern(1, 12, pos, local_step);
		}

		if (render) {
			setTileColor(pos, C1);
		} else {
			setTileColor(pos, C2);
		}

		if (meshMap.find(tile.color) == meshMap.end())
		{
			initColorMesh(tile.color, smgr);
		}
		batchMesh->addMesh(meshMap[tile.color], irr::core::vector3df(pos.x, tile.height, pos.y));
	}

	batchMesh->update();
	batchMesh->finalize();
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

inline bool SystemStageRenderer::squarePattern(int orientation, int size, glm::ivec2 pos, int step)
{
	glm::ivec2 centrePoint;
	centrePoint.x = pos.x - pos.x % size + size / 2;
	centrePoint.y = pos.y - pos.y % size + size / 2;

	int dx = abs(pos.x - centrePoint.x);
	int dy = abs(pos.y - centrePoint.y);

	//What the fuck
	return ((((dx % 2 == 0 && dy == 0) || (dx > 0 && dx % 2 == 0 && dy < dx)) ||  // columns
	         ((dy % 2 == 0 && dx == 0) || (dy > 0 && dy % 2 == 0 && dx < dy)) ||  // rows
	         (dx == dy && dx % 2 == 0 && dy % 2 == 0))                            // corners
	        ^ (step % 2));
}

}  // namespace client
