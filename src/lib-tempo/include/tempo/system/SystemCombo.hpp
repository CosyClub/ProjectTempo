#ifndef TEMPO_SYSTEM_COMBO_HPP
#define TEMPO_SYSTEM_COMBO_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentCombo.hpp>
#include <tempo/time.hpp>

namespace tempo
{

// 
struct SystemCombo : anax::System<anax::Requires<ComponentCombo>> 
{
	void advanceBeat();
	void checkForUpdates();
};

} // namespace tempo

#endif

