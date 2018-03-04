#ifndef TEMPO_ENTITY_TILEMASK_HPP
#define TEMPO_ENTITY_TILEMASK_HPP

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

#include <tempo/intrinsics.hpp>

#include <cstdio>

/////////////////////////////////////////////////////////////////////
/// \brief Helper macro for creating a tile mask from a list of
/// 49 bits. Shifts each bit to the correct location and adds together
/// This can be computed at compile time
/////////////////////////////////////////////////////////////////////
#define TEMPO_MAKE_TILEMASK(b00, b01, b02, b03, b04, b05, b06, b10, b11, b12, b13, b14, b15, b16,  \
                            b20, b21, b22, b23, b24, b25, b26, b30, b31, b32, b33, b34, b35, b36,  \
                            b40, b41, b42, b43, b44, b45, b46, b50, b51, b52, b53, b54, b55, b56,  \
                            b60, b61, b62, b63, b64, b65, b66)                                     \
	{                                                                                              \
		(((u64)(b00 & 1)) << 0) + (((u64)(b01 & 1)) << 1) + (((u64)(b02 & 1)) << 2)                \
		  + (((u64)(b03 & 1)) << 3) + (((u64)(b04 & 1)) << 4) + (((u64)(b05 & 1)) << 5)            \
		  + (((u64)(b06 & 1)) << 6) + (((u64)(b10 & 1)) << 7) + (((u64)(b11 & 1)) << 8)            \
		  + (((u64)(b12 & 1)) << 9) + (((u64)(b13 & 1)) << 10) + (((u64)(b14 & 1)) << 11)          \
		  + (((u64)(b15 & 1)) << 12) + (((u64)(b16 & 1)) << 13) + (((u64)(b20 & 1)) << 14)         \
		  + (((u64)(b21 & 1)) << 15) + (((u64)(b22 & 1)) << 16) + (((u64)(b23 & 1)) << 17)         \
		  + (((u64)(b24 & 1)) << 18) + (((u64)(b25 & 1)) << 19) + (((u64)(b26 & 1)) << 20)         \
		  + (((u64)(b30 & 1)) << 21) + (((u64)(b31 & 1)) << 22) + (((u64)(b32 & 1)) << 23)         \
		  + (((u64)(b33 & 1)) << 24) + (((u64)(b34 & 1)) << 25) + (((u64)(b35 & 1)) << 26)         \
		  + (((u64)(b36 & 1)) << 27) + (((u64)(b40 & 1)) << 28) + (((u64)(b41 & 1)) << 29)         \
		  + (((u64)(b42 & 1)) << 30) + (((u64)(b43 & 1)) << 31) + (((u64)(b44 & 1)) << 32)         \
		  + (((u64)(b45 & 1)) << 33) + (((u64)(b46 & 1)) << 34) + (((u64)(b50 & 1)) << 35)         \
		  + (((u64)(b51 & 1)) << 36) + (((u64)(b52 & 1)) << 37) + (((u64)(b53 & 1)) << 38)         \
		  + (((u64)(b54 & 1)) << 39) + (((u64)(b55 & 1)) << 40) + (((u64)(b56 & 1)) << 41)         \
		  + (((u64)(b60 & 1)) << 42) + (((u64)(b61 & 1)) << 43) + (((u64)(b62 & 1)) << 44)         \
		  + (((u64)(b63 & 1)) << 45) + (((u64)(b64 & 1)) << 46) + (((u64)(b65 & 1)) << 47)         \
		  + (((u64)(b66 & 1)) << 48)                                                               \
	}

namespace tempo
{
/////////////////////////////////////////////////////////////////////
/// \brief Represents the set of tiles that an entity occupies
/// Entities may be up to a maximum size of 7x7 tiles, centred such
/// that their position represents that of their central tile
///
/// \note Internally the mask uses a single bit per tile representing
/// whether the entity occupies the tile in question. Individual bits
/// may be accessed via a proxy object using the syntax mask[x][y]
/// These proxy objects may be treated as a bool.
/////////////////////////////////////////////////////////////////////
struct TileMask {
	/// \brief The bits of a 64 bit integer that should be considered as the mask
	static const constexpr u64 MASK_BITS = (((u64) 1) << 50) - 1;

	/// \brief Container for the 49 bits making up the mask
	uint64_t mask;

	/////////////////////////////////////////////////////////////////////
	/// \brief Helper struct which behaves as a bool allowing access to a single
	/// bit in some TileMask
	/////////////////////////////////////////////////////////////////////
	struct _AccessHelperBool {
		TileMask *const mask;       /// \brief The mask to access
		u08             bit_index;  /// \brief Which bit we are accessing

		                   operator bool() const;  // Cast to bool for comparisons
		_AccessHelperBool &operator=(bool);        // Set to bool for assignments
	};

	/////////////////////////////////////////////////////////////////////
	/// \brief Helper struct for accessing a column of a mask. Use operator[]
	/// again to access a single bit
	/////////////////////////////////////////////////////////////////////
	struct _AccessHelper {
		TileMask *const         mask;     /// \brief The mask to access
		u08                     x_index;  /// \brief The index in the x direction being accessed
		_AccessHelperBool       operator[](int y);
		const _AccessHelperBool operator[](int y) const;
	};

	/////////////////////////////////////////////////////////////////////
	/// \brief Accessor for particular tile
	/// mask[3][3] is the center, with limits being mask[0][0] and mask[7][7]
	/// Returned object acts as reference to the bit in question, and can be
	/// treated as if it were a bool
	/////////////////////////////////////////////////////////////////////
	_AccessHelper       operator[](int x);
	const _AccessHelper operator[](int x) const;

	/////////////////////////////////////////////////////////////////////
	/// \brief Determines if this TileMask is colliding with some other
	/// \param center_delta The delta between the center of this entity and the
	/// center of the other
	/// \return TileMask representing the common tiles occupied by both Entities
	/////////////////////////////////////////////////////////////////////
	TileMask isCollidingWith(const TileMask &other, glm::vec2 center_delta) const;

	/////////////////////////////////////////////////////////////////////
	/// \brief Computes a new TileMask that represents this translated
	/// by some amount along each axis
	/////////////////////////////////////////////////////////////////////]
	TileMask        getTranslated(int dx, int dy) const;
	inline TileMask getTranslated(glm::vec2 delta) const
	{
		return getTranslated(delta.x, delta.y);
	}

	/////////////////////////////////////////////////////////////////////
	/// \brief Translates this TileMask in place.
	/// This function is to getTranslated as += is to +
	/// \return Reference to this TileMask for operator chaining
	/////////////////////////////////////////////////////////////////////
	TileMask &translate(int dx, int dy);
	TileMask &translate(glm::vec2 delta)
	{
		return translate(delta.x, delta.y);
	}

	/////////////////////////////////////////////////////////////////////
	/// \brief Casts the TileMask to a bool. Evaluates to true
	/// if this entity takes up any tiles at all
	/////////////////////////////////////////////////////////////////////
	inline operator bool()
	{
		return this->mask & MASK_BITS;
	}
};

/////////////////////////////////////////////////////////////////////
/// \brief Debug function for visualising an TileMask
/// Prints it to stdout as 7 lines
/////////////////////////////////////////////////////////////////////
void print(const TileMask &mask);

const static constexpr TileMask tileMask0by0 = {0};
const static constexpr TileMask tileMask1by1 = TEMPO_MAKE_TILEMASK(0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   1,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0,
                                                                   0);

const static constexpr TileMask tileMask_3by3 = TEMPO_MAKE_TILEMASK(0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    1,
                                                                    1,
                                                                    1,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    1,
                                                                    1,
                                                                    1,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    1,
                                                                    1,
                                                                    1,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0,
                                                                    0);
}

/////////////////////////////////////////////////////////////////////
/// \brief Determines if two masks are equal
/// \note  Examines only the bits making up the mask, higher bits are ignored
/////////////////////////////////////////////////////////////////////
inline bool operator==(const tempo::TileMask &lhs, const tempo::TileMask &rhs)
{
	return ((lhs.mask & tempo::TileMask::MASK_BITS) == (rhs.mask & tempo::TileMask::MASK_BITS));
}

/////////////////////////////////////////////////////////////////////
/// \brief Determines if two masks are not equal
/// \note  Examines only the bits making up the mask, higher bits are ignored
/////////////////////////////////////////////////////////////////////
inline bool operator!=(const tempo::TileMask &lhs, const tempo::TileMask &rhs)
{
	return ((lhs.mask & tempo::TileMask::MASK_BITS) != (rhs.mask & tempo::TileMask::MASK_BITS));
}

/////////////////////////////////////////////////////////////////////
/// \brief Performs logical and of two masks, thus returning a mask of
/// tiles occupied by both the lhs and rhs mask
/////////////////////////////////////////////////////////////////////
inline tempo::TileMask operator&(const tempo::TileMask &lhs, const tempo::TileMask &rhs)
{
	return {(lhs.mask & rhs.mask) & tempo::TileMask::MASK_BITS};
}

/////////////////////////////////////////////////////////////////////
/// \brief Performs logical or of two masks, thus returning a mask of
/// tiles occupied by either the lhs or rhs mask
/////////////////////////////////////////////////////////////////////
inline tempo::TileMask operator|(const tempo::TileMask &lhs, const tempo::TileMask &rhs)
{
	return {(lhs.mask | rhs.mask) & tempo::TileMask::MASK_BITS};
}

/////////////////////////////////////////////////////////////////////
/// \brief Performs logical xor of two masks, thus returning a mask of
/// tiles occupied by either the lhs or rhs mask, but not both
/////////////////////////////////////////////////////////////////////
inline tempo::TileMask operator^(const tempo::TileMask &lhs, const tempo::TileMask &rhs)
{
	return {(lhs.mask ^ rhs.mask) & tempo::TileMask::MASK_BITS};
}

/////////////////////////////////////////////////////////////////////
/// \brief Performs logical negation of a mask, thus returning a mask
/// representing the tiles not occupied by the specified one
/////////////////////////////////////////////////////////////////////
inline tempo::TileMask operator~(const tempo::TileMask &lhs)
{
	return {~lhs.mask};
}

#endif
