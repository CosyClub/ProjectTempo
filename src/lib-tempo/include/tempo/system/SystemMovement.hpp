#ifndef TEMPO_SYSTEM_MOVEMENT_HPP
#define TEMPO_SYSTEM_MOVEMENT_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentStage.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentStagePosition.hpp>

#include <tempo/time.hpp>

namespace tempo
{

//
struct SystemMovement : anax::System<anax::Requires<ComponentStage,
																										ComponentStageTranslation,
																										ComponentStagePosition>>
{
	void processTranslation();
};

} // namespace tempo

#endif
