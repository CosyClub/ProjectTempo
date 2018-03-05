
// TODO License? Attribution? This doesn't look like ours

#ifndef XEN_CORE_INTRINSICS_HPP
#define XEN_CORE_INTRINSICS_HPP

#include <stdint.h>
#include <cmath>
#define M_PI 3.14159265358979323846 /* pi */

#define TempoArrayLength(array) (sizeof(array) / sizeof((array)[0]))

#define TempoMin(A, B) ((A < B) ? (A) : (B))
#define TempoMax(A, B) ((A < B) ? (B) : (A))

/// \brief Triggers break in debugger (or crash if no debugger)
/// \todo :TODO: something better?
#define TempoBreak(...) (*(char *) nullptr) = 'a';

#define TempoAssert(cond, ...)                                                                     \
	{                                                                                              \
		if (!(cond)) {                                                                             \
			TempoBreak(__VAR_ARGS__);                                                              \
		}                                                                                          \
	}
#define TempoInvalidCodePath() XenBreak();

typedef float        r32;
typedef double       r64;
typedef int8_t       s8;
typedef int8_t       s08;
typedef int16_t      s16;
typedef int32_t      s32;
typedef int64_t      s64;
typedef uint8_t      u8;
typedef uint8_t      u08;
typedef uint16_t     u16;
typedef uint32_t     u32;
typedef uint64_t     u64;
typedef int8_t       b8;
typedef int32_t      b32;
typedef unsigned int uint;
typedef uintptr_t    uptr;
typedef intptr_t     sptr;

#ifdef XEN_USE_DOUBLE_PRECISION
typedef double real;
#else
typedef float real;
#endif

namespace tempo
{
/// \brief Base class for types which should not be able to be copied
struct NonCopyable {
	inline NonCopyable() {}

   private:
	NonCopyable(const NonCopyable &other) = delete;
	NonCopyable &operator=(const NonCopyable &other) = delete;
};

static const constexpr real PI = (real) M_PI;

#ifdef XEN_USE_DOUBLE_PRECISION
inline double sqrt(double val)
{
	return sqrt(val);
}
#else
inline float  sqrt(float val)
{
	return sqrtf(val);
}
#endif

template <typename T>
T sign(T a)
{
	return (a < 0) ? -1 : 1;
}

/// \brief Clamps value to be between low and high
template <typename T>
T clamp(T val, T low, T high)
{
	if (val < low) {
		return low;
	}
	if (val > high) {
		return high;
	}
	return val;
}
}

inline constexpr real operator"" _r(long double val)
{
	return (real) val;
}
inline constexpr real operator"" _r(unsigned long long int val)
{
	return (real) val;
}

#endif
