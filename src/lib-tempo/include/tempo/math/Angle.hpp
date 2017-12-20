////////////////////////////////////////////////////////////////////////////////
//                        Part of Xeno Engine                                 //
////////////////////////////////////////////////////////////////////////////////
/// \file Angle.hpp
/// \author Jamie Terry
/// \date 2017/05/31
/// \brief Contains type for representing angles, helps in ensuring units are correct
/// and in converting between different units for angles
///
/// \ingroup math
////////////////////////////////////////////////////////////////////////////////

#ifndef XEN_MATH_ANGLE_HPP
#define XEN_MATH_ANGLE_HPP

#include <tempo/intrinsics.hpp>

#include <cmath>

namespace tempo{
	struct Angle{
		real radians;
	};

	inline constexpr Angle Degrees    (real value){ return { (value * tempo::PI) / 180.0_r }; }
	inline constexpr Angle Radians    (real value){ return { value                           }; }
	inline constexpr Angle Revolutions(real value){ return { value * 2.0_r * tempo::PI     }; }

	inline real asDegrees   (Angle a){ return (a.radians * 180.0_r) / tempo::PI; }
	inline real asRadians   (Angle a){ return a.radians;                           }
	inline real asRevolution(Angle a){ return a.radians / 2.0_r * tempo::PI;     }

	#if XEN_USE_DOUBLE_PRECISION
	inline double sin (Angle a ){ return sin(a.radians); }
	inline double cos (Angle a ){ return cos(a.radians); }
	inline double tan (Angle a ){ return tan(a.radians); }

	inline Angle  asin(double a){ return { asin(a) };    }
	inline Angle  acos(double a){ return { acos(a) };    }
	inline Angle  atan(double a){ return { atan(a) };    }

	inline Angle  atan2(double a, double b){ return { atan2(a,b) }; }
	#else
	inline float  sin (Angle a){ return sinf(a.radians); }
	inline float  cos (Angle a){ return cosf(a.radians); }
	inline float  tan (Angle a){ return tanf(a.radians); }

	inline Angle  asin(float a){ return { asinf(a) };    }
	inline Angle  acos(float a){ return { acosf(a) };    }
	inline Angle  atan(float a){ return { atanf(a) };    }

	inline Angle  atan2(float a, float b){ return { atan2f(a,b) }; }
	#endif


	/// \brief Clamps an angle to being between 0 and 1 full revolution
	inline Angle clamp(Angle a){
		// clamp to between -1 and 1 revolution
		real val = (real)fmod(a.radians, 2.0_r * tempo::PI );
		return { val < 0 ? val + 2.0_r * tempo::PI : val  };
	}
}

inline tempo::Angle operator+=(tempo::Angle& lhs,  const tempo::Angle& rhs){ lhs.radians += rhs.radians; return lhs; }
inline tempo::Angle operator-=(tempo::Angle& lhs,  const tempo::Angle& rhs){ lhs.radians -= rhs.radians; return lhs; }
inline tempo::Angle operator*=(tempo::Angle& lhs,  real              rhs){ lhs.radians *= rhs;         return lhs; }
inline tempo::Angle operator/=(tempo::Angle& lhs,  real              rhs){ lhs.radians /= rhs;         return lhs; }

inline tempo::Angle operator*(const tempo::Angle& lhs, real              rhs){ return {lhs.radians * rhs}; }
inline tempo::Angle operator*(real              lhs, const tempo::Angle& rhs){ return {lhs * rhs.radians}; }
inline tempo::Angle operator/(const tempo::Angle& lhs, real              rhs){ return {lhs.radians / rhs}; }

inline tempo::Angle operator+(const tempo::Angle& lhs, const tempo::Angle& rhs){ return {lhs.radians + rhs.radians}; }
inline tempo::Angle operator-(const tempo::Angle& lhs, const tempo::Angle& rhs){ return {lhs.radians - rhs.radians}; }

inline tempo::Angle operator-(const tempo::Angle& a) { return { -a.radians }; }

inline bool operator==(const tempo::Angle& a, const tempo::Angle& b){
	real delta = a.radians - b.radians;
	return delta * delta <= 0.0000001;
}
inline bool operator!=(const tempo::Angle& a, const tempo::Angle& b){ return !(a == b); }
inline bool operator<=(const tempo::Angle& a, const tempo::Angle& b){ return a.radians <= b.radians; }
inline bool operator>=(const tempo::Angle& a, const tempo::Angle& b){ return a.radians >= b.radians; }
inline bool operator< (const tempo::Angle& a, const tempo::Angle& b){ return a.radians <  b.radians; }
inline bool operator> (const tempo::Angle& a, const tempo::Angle& b){ return a.radians >  b.radians; }

inline constexpr tempo::Angle operator"" _deg(long double            val){ return tempo::Degrees    ((real)val); }
inline constexpr tempo::Angle operator"" _deg(unsigned long long int val){ return tempo::Degrees    ((real)val); }

inline constexpr tempo::Angle operator"" _rad(long double            val){ return tempo::Radians    ((real)val); }
inline constexpr tempo::Angle operator"" _rad(unsigned long long int val){ return tempo::Radians    ((real)val); }

inline constexpr tempo::Angle operator"" _rev(long double            val){ return tempo::Revolutions((real)val); }
inline constexpr tempo::Angle operator"" _rev(unsigned long long int val){ return tempo::Revolutions((real)val); }


#endif
