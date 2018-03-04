#ifndef TEMPO_ENTITY_GRIDAI_HPP
#define TEMPO_ENTITY_GRIDAI_HPP
#include <tempo/component/ComponentGridAi.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>

#include <anax/System.hpp>

namespace tempo
{
struct SystemGridAi : anax::System<anax::Requires<ComponentStageTranslation, ComponentGridAi>> {
	void update();
};
}

#endif
