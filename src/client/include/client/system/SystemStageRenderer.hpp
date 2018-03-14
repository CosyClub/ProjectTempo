#ifndef CLIENT_SYSTEM_STAGE_RENDERER_HPP
#define CLIENT_SYSTEM_STAGE_RENDERER_HPP

#include <client/misc/CBatchingMesh.hpp>

#include <tempo/component/ComponentStage.hpp>


#include <anax/System.hpp>

#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <vector>

#include <glm/vec2.hpp>

namespace client
{
typedef std::vector<glm::ivec2>                    stage_nodes;
typedef std::vector<std::tuple<glm::ivec2, float>> heights;

// Initalises all rendering for the static stage
class SystemStageRenderer : public anax::System<anax::Requires<tempo::ComponentStage>>
{
   public:
	stage_nodes                    tile_nodes;
	std::vector<tempo::stage_tile> old_positions;
	std::vector<double>            fractions;

	std::vector<float> currentHeight = {-1.f};

	irr::video::ITexture *wall_diffuse_map;
	irr::video::ITexture *wall_normal_map;
	irr::video::ITexture *tile_texture;

	irr::scene::IMeshSceneNode *node;
	irr::scene::IMesh *         mesh;
	irr::scene::CBatchingMesh * batchMesh;

	// Creates a static irrlitch scene node based on the component stage heights
	void setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver);

	void updateStage(glm::ivec4                 colour1,
	                 glm::ivec4                 colour2,
	                 irr::scene::ISceneManager *smgr,
	                 irr::video::IVideoDriver * driver,
	                 int                        j,
	                 glm::ivec2                 playerpos);

   private:
	inline void addFloorTilesToScene(irr::scene::ISceneManager *smgr,
	                                 irr::video::IVideoDriver * driver,
	                                 tempo::ComponentStage &    stage);


	inline bool checkerBoardPattern(irr::video::IVideoDriver *driver,
	                                glm::ivec4                colour1,
	                                glm::ivec4                colour2,
	                                int                       i,
	                                int                       j);

	inline bool linePattern(irr::video::IVideoDriver *driver,
	                        glm::ivec4                colour1,
	                        glm::ivec4                colour2,
	                        int                       orientation,
	                        int                       size,
	                        int                       i,
	                        int                       j);

	inline bool squarePattern(irr::video::IVideoDriver *driver,
	                          glm::ivec4                colour1,
	                          glm::ivec4                colour2,
	                          int                       orientation,
	                          int                       size,
	                          int                       i,
	                          int                       j);
};
}  // namespace client

#endif
