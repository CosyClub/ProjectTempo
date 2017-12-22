////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file TileMask.hpp
/// \author Jamie Terry
/// \date 2017/12/22
/// \brief Contains the implementation of methods of tempo::TileMask
////////////////////////////////////////////////////////////////////////////

#include <cstdio>

#include <tempo/intrinsics.hpp>
#include <tempo/entity/TileMask.hpp>

namespace tempo {
	TileMask::_AccessHelper TileMask::operator[](int x){
		return { this, (u08)x };
	}
	const TileMask::_AccessHelper TileMask::operator[](int x) const{
		return { const_cast<TileMask*>(this), (u08)x };
	}

	TileMask::_AccessHelperBool TileMask::_AccessHelper::operator[](int y) {
		return { this->mask, (u08)(this->x_index + y*7) };
	}
	const TileMask::_AccessHelperBool TileMask::_AccessHelper::operator[](int y) const {
		return { const_cast<TileMask*>(this->mask), (u08)(this->x_index + y*7) };
	}

	TileMask::_AccessHelperBool::operator bool() const {
		return this->mask->mask & ((u64)1 << this->bit_index);
	}

	TileMask::_AccessHelperBool& TileMask::_AccessHelperBool::operator= (bool val) {
		if(val){
			// Then set the bit to 1
			this->mask->mask |=  ((u64)1 << this->bit_index);
		} else {
			// Then clear the bit to 0
			this->mask->mask &= ~((u64)1 << this->bit_index);
		}
		return *this;
	}

	/////////////////////////////////////////////////////////////////////
	/// \brief Determines if this TileMask is colliding with some other
	/// \param other The other TileMask in question
	/// \param center_delta The delta between the center of this entity and the center of the other
	/// \return TileMask representing the common tiles occupied by both Entities
	/////////////////////////////////////////////////////////////////////
	TileMask TileMask::isCollidingWith(const TileMask& other, Vec2s center_delta) const{
		TileMask other_relative = other.getTranslated(center_delta);
		return *this & other_relative;
	}

	/////////////////////////////////////////////////////////////////////
	/// \brief Computes a new TileMask that represents this translated
	/// by some amount along each axis
	/////////////////////////////////////////////////////////////////////]
	TileMask TileMask::getTranslated(int dx, int dy) const{
		TileMask result = *this;//*this;
		result.translate(dx, dy);
		return result;
	}

	/////////////////////////////////////////////////////////////////////
	/// \brief Translates this TileMask in place,
	/// is to getTranslated as += is to +
	/// \return Reference to this TileMask for operator chaining
	/////////////////////////////////////////////////////////////////////
	TileMask& TileMask::translate(int dx, int dy){
		//////////////////////////////////////
		// Perform shift along x
		if(dx > 0){
			this->mask <<=  dx;
		} else {
			this->mask >>= -dx;
		}

		//////////////////////////////////////
		// We now need to do some clean up, since values may have wrapped
		// (IE: shifting something off the end of the row will put it onto the next)
		//
		// . . . .     . . . .
		// . # # #  >> . . # #
		// . # . .     # . # .
		// . . . .     . . . .
		//
		// We can clean this up by anding with a mask of cells that could possibly
		// contain values, in for example, when shifting right by 1 the mask would be:
		// . # # #
		// . # # #
		// . # # #
		// . # # #
		//
		// If we shifted by (-2,0) the mask would be:
		// # # . .
		// # # . .
		// # # . .
		// # # . .
		//
		//
		// The bits are laid out in memory as:
		// 42	43	44	45	46	47	48
		// 35	36	37	38	39	40	41
		// 28	29	30	31	32	33	34
		// 21	22	23	24	25	26	27
		// 14	15	16	17	18	19	20
		//  7	 8	 9	10	11	12	13
		//  0	 1	 2	 3	 4	 5	 6
		//
		// First compute the pattern along the x axis:
		u64 x_pattern = 127;
		if(dx > 0){
			x_pattern <<=  dx;
		} else {
			x_pattern >>= -dx;
		}
		x_pattern &= 127;

		// Now replicate the x_pattern into all rows.
		//
		// Imagine the x_pattern = 9. In decimal we could broadcast this into certain
		// positions by multiplying by a number like 1011001, to get 9099009
		// Hence the "broadcast" is just a multiplication.
		//
		// In binary we want to multiply by the value with the low bit of each
		// groups of 7 bits set to 1. IE:
		// 2^0 + 2^7 + 2^14 + 2^21 + 2^28 + 2^35 + 2^42
		x_pattern *= 4432676798593;
		this->mask &= x_pattern;


		//////////////////////////////////////
		// Now perform shift along y
		if(dy > 0){
			this->mask <<=  dy*7;
		} else {
			this->mask >>= -dy*7;
		}
		// Clean up along y is simpler, since bits will have been shifted completely out of range
		// so just and with the full 7x7 mask
		this->mask &= TileMask::MASK_BITS;

		return *this;
	}

	void print(const TileMask& mask){
		const char chars[2] = {'.', '#'};

		for(int y = 0; y < 7; ++y){
			for(int x = 0; x < 7; ++x){
				printf("%c ", chars[mask[x][y] ? 1 : 0]);
			}
			printf("\n");
		}

	}
}
