#ifndef TEMPO_ENTITY_RENDERHEALTH_HPP
#define TEMPO_ENTITY_RENDERHEALTH_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentHealth.hpp>
#include <tempo/entity/ComponentRender.hpp>

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
