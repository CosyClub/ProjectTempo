////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file EntityTileMask.hpp
/// \author Jamie Terry
/// \date 2017/12/20
/// \brief Contains definition of EntityTileMask types and related functions
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_ENTITYTILEMASK_HPP
#define TEMPO_ENTITY_ENTITYTILEMASK_HPP

namespace tempo{
	/////////////////////////////////////////////////////////////////////
	/// \brief Represents the set of tiles that an entity occupies
	/// Entities may be up to a maximum size of 7x7 tiles, centred such
	/// that their position represents that of their central tile
	/////////////////////////////////////////////////////////////////////
	struct EntityTileMask {
		uint64_t mask;

		struct _AccessHelperBool {
			EntityTileMask* const mask;
			operator bool() const;  // Cast to bool for comparisons
			bool operator=(bool); // Set to bool for assignments
		};

		struct _AccessHelper {
			EntityTileMask* const mask;
			uint8_t row;
			_AccessHelperBool operator[](uint8_t col);
			const _AccessHelperBool operator[](uint8_t col) const;
		};

		/////////////////////////////////////////////////////////////////////
		/// \brief Accessor for particular tile
		/// mask[3][3] is the center, with limits being mask[0][0] and mask[7][7]
		/// Returned object acts as reference to the bit in question, and can be
		/// treated as if it were a bool
		/////////////////////////////////////////////////////////////////////
		_AccessHelper       operator[](uint8_t row);
		const _AccessHelper operator[](uint8_t row) const;
	};
}

#endif
