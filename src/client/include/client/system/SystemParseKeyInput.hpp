#ifndef CLIENT_SYSTEM_PARSE_KEY_INPUT_HPP
#define CLIENT_SYSTEM_PARSE_KEY_INPUT_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <client/component/ComponentKeyInput.hpp>

#include <tempo/time.hpp>
#include <IrrlichtDevice.h>

#define TEMPO_DATA_CAPTURE 1

#ifdef TEMPO_DATA_CAPTURE
#include <fstream>
#endif

namespace client
{
struct SystemParseKeyInput : anax::System<anax::Requires<ComponentKeyInput>> {
	#ifdef TEMPO_DATA_CAPTURE
	std::ofstream data_output;
	#endif

	// Sets up the system, opens the data_output file
	SystemParseKeyInput();

	// Closes the data_output file
	~SystemParseKeyInput();

	void parseInput(tempo::Clock &clock, irr::IrrlichtDevice* device);
};

}  // namespace tempo

#endif
