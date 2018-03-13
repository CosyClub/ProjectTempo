#ifndef CLIENT_SYSTEM_STAGE_RENDERER_HPP
#define CLIENT_SYSTEM_STAGE_RENDERER_HPP

#include <tempo/component/ComponentStage.hpp>

#include <anax/System.hpp>

#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <vector>

namespace client
{
typedef std::vector<std::tuple<glm::ivec2, irr::scene::IMeshSceneNode *>> stage_nodes;
typedef std::vector<std::tuple<glm::ivec2, float >> heights;

// Initalises all rendering for the static stage
class SystemStageRenderer : public anax::System<anax::Requires<tempo::ComponentStage>>
{
   public:
	stage_nodes tile_nodes;
  std::vector<tempo::stage_tile> old_positions;
  std::vector<double> fractions;

	// Creates a static irrlitch scene node based on the component stage heights
	void setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver);

	void updateStage(glm::ivec4                colour1,
	                 glm::ivec4                colour2,
	                 irr::video::IVideoDriver *driver,
	                 int                       j,
                   glm::ivec2                playerpos);

   private:

  inline void addFloorTilesToScene(irr::scene::ISceneManager *smgr,
                                          irr::video::IVideoDriver * driver,
                                          tempo::ComponentStage& stage);


	inline void checkerBoardPattern(irr::video::IVideoDriver *  driver,
	                                irr::scene::IMeshSceneNode *node,
	                                irr::video::SMaterial &     material_top,
	                                glm::ivec4                  colour1,
	                                glm::ivec4                  colour2,
	                                int                         i,
	                                int                         j);

	inline void linePattern(irr::video::IVideoDriver *  driver,
	                        irr::scene::IMeshSceneNode *node,
	                        irr::video::SMaterial &     material_top,
	                        glm::ivec4                  colour1,
	                        glm::ivec4                  colour2,
	                        int                         orientation,
	                        int                         size,
	                        int                         i,
	                        int                         j);

	inline void squarePattern(irr::video::IVideoDriver *  driver,
	                          irr::scene::IMeshSceneNode *node,
	                          irr::video::SMaterial &     material_top,
	                          glm::ivec4                  colour1,
	                          glm::ivec4                  colour2,
	                          int                         orientation,
	                          int                         size,
	                          int                         i,
	                          int                         j);
};
}  // namespace client

#endif
