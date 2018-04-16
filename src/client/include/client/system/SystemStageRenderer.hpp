#ifndef CLIENT_SYSTEM_STAGE_RENDERER_HPP
#define CLIENT_SYSTEM_STAGE_RENDERER_HPP

#include <client/misc/CBatchingMesh.hpp>

#include <tempo/component/ComponentStage.hpp>


#include <anax/System.hpp>

#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/vec4.hpp>

namespace client
{
typedef std::vector<glm::ivec2>                    stage_nodes;
typedef std::vector<std::tuple<glm::ivec2, float>> heights;

struct Vec3Funcs
{
    size_t operator()(const glm::ivec3& k)const
    {
        return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
    }

    bool operator()(const glm::vec3& a, const glm::vec3& b)const
    {
            return a.x == b.x && a.y == b.y && a.z == b.z;
    }
};

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
	irr::scene::IMesh *         walls;
	irr::scene::CBatchingMesh * batchMesh;

	std::map<glm::ivec3, std::vector<glm::ivec2>, 
	         Vec3Funcs, std::allocator<std::pair<const glm::ivec2, int>>> colorDeltaMap;

	// Creates a static irrlitch scene node based on the component stage heights
	void setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver,
						 glm::ivec4 colour1, glm::ivec4 colour2);

	void Update(irr::scene::ISceneManager *smgr,
                    irr::video::IVideoDriver * driver);

	void colorStage(int                        j,
                        glm::ivec2                 playerpos,
                        glm::vec3                  C1,
			glm::vec3                  C2);

	void setTileColor(glm::ivec2 pos, glm::ivec3 color);

   private:
	

	inline void addFloorTilesToScene(irr::scene::ISceneManager *smgr,
	                                 irr::video::IVideoDriver * driver,
	                                 tempo::ComponentStage &    stage,
																 glm::ivec4 colour1, glm::ivec4 colour2);


	inline bool checkerBoardPattern(int                       i,
	                                int                       j);

	inline bool linePattern(int                       orientation,
	                        int                       size,
	                        int                       i,
	                        int                       j);

	inline bool squarePattern(int                       orientation,
	                          int                       size,
	                          int                       i,
	                          int                       j);
};
}  // namespace client

#endif
