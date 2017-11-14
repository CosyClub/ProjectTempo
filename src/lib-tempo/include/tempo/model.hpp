////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Application.hpp
/// \author Raef Coles
/// \date 2017/11/13
/// \Functions and classes related to timing
//////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_SONG_HPP
#define TEMPO_SONG_HPP

#include <cstdint>
#include <limits>

#define MP_ID_TYPE int16_t
#define MP_MAX_PLAYERS std::numeric_limits<MP_ID_TYPE>::max()

#undef main // SDL likes to define main

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
