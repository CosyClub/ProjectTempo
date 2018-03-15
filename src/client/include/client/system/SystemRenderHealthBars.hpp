#ifndef CLIENT_ENTITY_RENDERHEALTH_HPP
#define CLIENT_ENTITY_RENDERHEALTH_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <client/component/ComponentHealthBar.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentCombo.hpp>

#include <ISceneManager.h>

namespace client
{
struct SystemRenderHealthBars
    : anax::System<anax::Requires<tempo::ComponentHealth,
                                  client::ComponentHealthBar,
                                  client::ComponentRenderSceneNode>> {
	void setup(irr::scene::ISceneManager *smgr);
	// HealthBarUpdate
	// Update all the health bars of entities based on their current health value
	//
	// Arguments:
	//          none
	// Returns:
	//          void
	void update();

	irr::core::dimension2d<irr::f32> original_size;
};
}

#endif
