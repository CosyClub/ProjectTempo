#ifndef CLIENT_ENTITY_RENDERHEALTH_HPP
#define CLIENT_ENTITY_RENDERHEALTH_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentHealth.hpp>
#include <client/component/ComponentHealthBar.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>

#include <ISceneManager.h>

namespace client {
	struct SystemRenderHealthBars : anax::System<anax::Requires<
			tempo::ComponentHealth,
	    client::ComponentHealthBar,
	    client::ComponentRenderSceneNode>> {

		void setup(irr::scene::ISceneManager* smgr);
		// HealthBarUpdate
		// Update all the health bars of entities based on their current health value
		//
		// Arguments:
		//          none
		// Returns:
		//          void
		void HealthBarUpdate();
	};
}

#endif
