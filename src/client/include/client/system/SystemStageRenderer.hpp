#ifndef CLIENT_SYSTEM_STAGE_RENDERER_HPP
#define CLIENT_SYSTEM_STAGE_RENDERER_HPP

#include <client/misc/CBatchingMesh.hpp>

#include <tempo/component/ComponentStage.hpp>


#include <anax/System.hpp>

#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <vector>
#include <unordered_map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/vec4.hpp>

namespace client
{
typedef std::vector<glm::ivec2>                    stage_nodes;

struct vec2less
{
	bool operator()(const glm::ivec2& a, const glm::ivec2& b)const
	{
		return (a.x < b.x) || ((a.x == b.x) && (a.y < b.y));
	}
};

struct vec2eq
{
    size_t operator()(const glm::ivec2& k)const
    {
        return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
    }

    bool operator()(const glm::ivec2& a, const glm::ivec2& b)const
    {
            return a.x == b.x && a.y == b.y;
    }
};

typedef struct {
	glm::ivec2 pos;
	float height;
	float height_target;
	irr::video::SColor color;
} tile_t;

// Initalises all rendering for the static stage
class SystemStageRenderer : public anax::System<anax::Requires<tempo::ComponentStage>>
{
   public:
	irr::video::ITexture *wall_diffuse_map;
	irr::video::ITexture *wall_normal_map;
	irr::video::ITexture *tile_texture;

	irr::scene::IMeshSceneNode *node;
	irr::scene::IMesh *         walls;
	irr::scene::CBatchingMesh * batchMesh;

	std::unordered_map<glm::ivec2, tile_t,
	         vec2eq, vec2eq, std::allocator<std::pair<const glm::ivec2, tile_t>>> tileMap;

	std::map<irr::video::SColor, irr::scene::IMesh*> meshMap;

	// Creates a static irrlitch scene node based on the component stage heights
	void setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver);


	void Update(irr::scene::ISceneManager *smgr,
	            irr::video::IVideoDriver  *driver,
	            glm::ivec2                 playerpos);

void Update(irr::scene::ISceneManager *smgr,
            irr::video::IVideoDriver * driver,
            glm::ivec2                 playerpos,
            irr::video::SColor         C1,
            irr::video::SColor         C2,
            int                        step,
            float                      dt);

	void colorStage(int                        j,
                        glm::ivec2                 playerpos,
	                irr::video::SColor         C1,
	                irr::video::SColor         C2);

	void AnimateTiles(float dt, glm::ivec2 playerpos);

	void setTileColor(glm::ivec2 pos, irr::video::SColor color);

   private:
	void initColorMesh(irr::video::SColor color, irr::scene::ISceneManager *smgr);

	inline void addFloorTilesToScene(irr::scene::ISceneManager *smgr,
	                                 irr::video::IVideoDriver * driver,
	                                 tempo::ComponentStage &    stage);


	inline bool checkerBoardPattern(glm::ivec2 pos, int step);
	inline bool linePattern(int orientation, int size, glm::ivec2 pos, int step);
	inline bool squarePattern(int orientation, int size, glm::ivec2 pos, int step);
};
}  // namespace client

#endif
