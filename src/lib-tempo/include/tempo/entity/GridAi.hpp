////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_GRIDAI_HPP
#define TEMPO_ENTITY_GRIDAI_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/LevelManager.hpp>

namespace tempo{
	struct ComponentGridAi : anax::Component {
		// no state needed for AI (yet)
	};

	struct SystemGridAi : anax::System<anax::Requires<ComponentGridMotion, ComponentGridAi>> {
		void update();
	};
}

#endif
