#ifndef TEMPO_ENTITY_GRIDAI_HPP
#define TEMPO_ENTITY_GRIDAI_HPP

#include <anax/Component.hpp>

#include <tempo/entity/LevelManager.hpp>
#include <tempo/entity/ComponentGridAi.hpp>

namespace tempo{
	struct SystemGridAi : anax::System<anax::Requires<ComponentGridMotion, ComponentGridAi>> {
		void update();
	};
}

#endif
