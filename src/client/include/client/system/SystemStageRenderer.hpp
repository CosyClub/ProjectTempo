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

	};
} // namespace client

#endif
