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

#undef main // SDL likes to define main

namespace tempo
{
	// Temporary player struct - will be upgraded or replaced with class,
	// just for the purpose of writing server code
	typedef struct {
		int32_t mp_id;
		int16_t x, y;
	} Player;
}

#endif
