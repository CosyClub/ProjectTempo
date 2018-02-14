#ifndef TEMPO_COMPONENT_GRIDAI_HPP
#define TEMPO_COMPONENT_GRIDAI_HPP

#include <anax/Component.hpp>

#include <tempo/entity/ComponentGridMotion.hpp>
#include <tempo/entity/ComponentGridPosition.hpp>

namespace tempo{
	struct ComponentGridAi : anax::Component {
		// no state needed for AI (yet)
	};
}

#endif
