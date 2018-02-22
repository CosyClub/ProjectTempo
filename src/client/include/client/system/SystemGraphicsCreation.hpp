#ifndef TEMPO_SYSTEM_SYSTEMGRAPHICSCREATION_HPP
#define TEMPO_SYSTEM_SYSTEMGRAPHICSCREATION_HPP

#include <anax/System.hpp>

#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentModel.hpp>

namespace tempo 
{

struct SystemGraphicsCreation : anax::System<anax::Requires<ComponentStagePosition,
                                                            ComponentModel>>
{
	SystemGraphicsCreation();
	void addEntities();

};

} // namespace tempo

#endif
