#ifndef CLIENT_SYSTEM_PARSE_KEY_INPUT_HPP
#define CLIENT_SYSTEM_PARSE_KEY_INPUT_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <client/component/ComponentKeyInput.hpp>

#include <tempo/time.hpp>
#include <IrrlichtDevice.h> 

namespace client
{
struct SystemParseKeyInput : anax::System<anax::Requires<ComponentKeyInput>> {
	void parseInput(tempo::Clock &clock, irr::IrrlichtDevice* device);
};

}  // namespace tempo

#endif
