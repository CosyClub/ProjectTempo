#ifndef CLIENT_SYSTEM_PARSE_KEY_INPUT_HPP
#define CLIENT_SYSTEM_PARSE_KEY_INPUT_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <client/component/ComponentKeyInput.hpp>

#include <tempo/time.hpp>

namespace client
{

	struct SystemParseKeyInput : anax::System<anax::Requires<ComponentKeyInput>> 
	{
		void parseInput(tempo::Clock &clock);
	};

} // namespace tempo

#endif