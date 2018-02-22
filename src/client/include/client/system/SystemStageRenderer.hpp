#ifndef CLIENT_SYSTEM_STAGE_RENDERER_HPP
#define CLIENT_SYSTEM_STAGE_RENDERER_HPP

#include <tempo/component/ComponentStage.hpp>

#include <anax/System.hpp>

#include <ISceneManager.h>
#include <IVideoDriver.h>

namespace client {

	// Initalises all rendering for the static stage
	class SystemStageRenderer : public anax::System<anax::Requires<tempo::ComponentStage>> {
	public:
		// Creates a static irrlitch scene node based on the component stage heights
		void setup(irr::scene::ISceneManager* smgr, irr::video::IVideoDriver* driver);

	};
} // namespace client

#endif
