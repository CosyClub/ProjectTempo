#ifndef CLIENT_SYSTEM_RENDER_SCENE_NODE_HPP
#define CLIENT_SYSTEM_RENDER_SCENE_NODE_HPP

#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStage.hpp>

#include <client/component/ComponentRenderSceneNode.hpp>

#include <anax/System.hpp>

#include <ISceneManager.h>

namespace client {
	class SystemRenderSceneNode : public anax::System<anax::Requires<
			tempo::ComponentStage,
			tempo::ComponentStagePosition,
			client::ComponentRenderSceneNode>> {
	public:
		// Creates a static red square for all components with positions and render nodes
		void setup(irr::scene::ISceneManager* smgr);
		// Updates the positions of all render scene node according to the stage position
		void update();
	};
} // namespace client

#endif
