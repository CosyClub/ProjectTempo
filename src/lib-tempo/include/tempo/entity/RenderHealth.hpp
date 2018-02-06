#ifndef TEMPO_ENTITY_RENDERHEALTH_HPP
#define TEMPO_ENTITY_RENDERHEALTH_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/Health.hpp>
#include <tempo/entity/Render.hpp>
#include <tempo/entity/GridAi.hpp>
#include <tempo/entity/PlayerRemote.hpp>
#include <tempo/entity/PlayerRemoteServer.hpp>

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
