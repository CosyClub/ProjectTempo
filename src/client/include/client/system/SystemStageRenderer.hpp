#ifndef CLIENT_SYSTEM_STAGE_RENDERER_HPP
#define CLIENT_SYSTEM_STAGE_RENDERER_HPP

#include <tempo/component/ComponentStage.hpp>

#include <anax/System.hpp>

#include <ISceneManager.h>
#include <IVideoDriver.h>

namespace client {

	typedef std::vector<std::tuple<glm::ivec2, irr::scene::IMeshSceneNode*>> stage_nodes;

	// Initalises all rendering for the static stage
	class SystemStageRenderer : public anax::System<anax::Requires<tempo::ComponentStage>> {
	public:

		stage_nodes tile_nodes;

		// Creates a static irrlitch scene node based on the component stage heights
		void setup(irr::scene::ISceneManager* smgr, irr::video::IVideoDriver* driver);

		void updateStage(glm::ivec4 colour1, glm::ivec4 colour2, irr::video::IVideoDriver*  driver, int j);

	private:
		void checkerBoardPattern(irr::video::IVideoDriver*  driver,
	 												   irr::video::SMaterial& material_side,
														 glm::ivec4 colour1,
														 glm::ivec4 colour2,
														 int i,
												     int j);

	 void linePattern(irr::video::IVideoDriver*  driver,
										irr::video::SMaterial& material_side,
										glm::ivec4 colour1,
										glm::ivec4 colour2,
										int orientation,
										int size,
										int i,
										int j);

	void squarePattern(irr::video::IVideoDriver*  driver,
										 irr::video::SMaterial& material_side,
										 glm::ivec4 colour1,
										 glm::ivec4 colour2,
										 int orientation,
										 int size,
										 int i,
										 int j);

	};
} // namespace client

#endif
