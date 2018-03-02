#ifndef TEMPO_MODEL_HPP
#define TEMPO_MODEL_HPP

#include <tempo/config.hpp>

#include <cstdint>
#include <limits>

#define MP_ID_TYPE int16_t
#define MP_MAX_PLAYERS std::numeric_limits<MP_ID_TYPE>::max()

namespace tempo
{
	// Temporary player struct - will be upgraded or replaced with class,
	// just for the purpose of writing server code
	typedef struct {
		MP_ID_TYPE mp_id; // Multiplayer ID
		int16_t x, y;  // Position, x and y
	} Player;

	// Game State - same temporary disclaimer as Player
	typedef struct {
		Player players[MP_MAX_PLAYERS];
	} GameState;
}

#endif
