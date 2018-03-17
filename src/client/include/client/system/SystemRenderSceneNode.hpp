#ifndef CLIENT_SYSTEM_RENDER_SCENE_NODE_HPP
#define CLIENT_SYSTEM_RENDER_SCENE_NODE_HPP

#include <tempo/component/ComponentModel.hpp>
#include <tempo/component/ComponentStage.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentCombo.hpp>

#include <client/component/ComponentRenderSceneNode.hpp>

#include <anax/System.hpp>

#include <ISceneManager.h>

namespace client
{
class SystemRenderSceneNode
    : public anax::System<anax::Requires<tempo::ComponentStage,
                                         tempo::ComponentStagePosition,
                                         client::ComponentRenderSceneNode,
                                         tempo::ComponentModel>>
{
   public:
	// Creates a static red square for all components with positions and render nodes
	void setup(irr::scene::ISceneManager *smgr,irr::video::IVideoDriver * driver);
	// Updates the positions of all render scene node according to the stage position
	void update();
};
}  // namespace client

#endif
