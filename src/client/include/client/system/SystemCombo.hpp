#ifndef CLIENT_SYSTEM_COMBO_HPP
#define CLIENT_SYSTEM_COMBO_HPP

#include <anax/System.hpp>
#include <anax/World.hpp>

#include <tempo/component/ComponentCombo.hpp>
#include <tempo/time.hpp>

namespace client
{

struct SystemCombo : anax::System<anax::Requires<tempo::ComponentCombo>> {
	void advanceBeat();
	void checkForUpdates(anax::World &world);
};

}  // namespace client

#endif