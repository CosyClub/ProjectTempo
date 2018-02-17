#ifndef TEMPO_SYSTEM_RENDERHEALTH_HPP
#define TEMPO_SYSTEM_RENDERHEALTH_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentHealth.hpp>

#include <client/component/ComponentRender.hpp>

#include <Ogre.h>

namespace tempo {
	struct RenderHealth : anax::System<anax::Requires<ComponentHealth, ComponentRender>> {
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
