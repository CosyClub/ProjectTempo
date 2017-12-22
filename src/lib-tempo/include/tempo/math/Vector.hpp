////////////////////////////////////////////////////////////////////////////////
//                        Part of Xeno Engine                                 //
////////////////////////////////////////////////////////////////////////////////
/// \file Vector.hpp
/// \author Jamie Terry
/// \date 2017/05/30
/// \brief Contains types for representing vectors (in the mathematical sense)
/// as well as the functions to manipulate them
///
/// \ingroup math
////////////////////////////////////////////////////////////////////////////////

#ifndef XEN_MATH_VECTOR_HPP
#define XEN_MATH_VECTOR_HPP

#include <tempo/intrinsics.hpp>
#include <tempo/math/Angle.hpp>

// gcc doesn't like the anonomous structures inside unions, disable the warning temporarily...
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

namespace tempo{

	template<u32 T_DIM, typename T>
	struct Vec{
		T elements[T_DIM];
	};

	template<typename T>
	struct Vec<2,T>{
		union{
		    T elements[2];
			struct{ T x, y; };
			struct{ T u, v; };
		};
		static const Vec<2, T> UnitX, UnitY, Origin;
		static const Vec<2, T> UnitAxes[2];
	};
	template<typename T> const Vec<2, T> Vec<2, T>::UnitX       = {1,0};
	template<typename T> const Vec<2, T> Vec<2, T>::UnitY       = {0,1};
	template<typename T> const Vec<2, T> Vec<2, T>::Origin      = {0,0};
	template<typename T> const Vec<2, T> Vec<2, T>::UnitAxes[2] = {{1,0}, {0,1}};

	template<typename T>
	struct Vec<3,T>{
		Vec<3,T>(){}
		Vec<3,T>(T nx, T ny, T nz) : x(nx), y(ny), z(nz) {}
		union{
			T elements[3];
			struct{ T x, y, z;                     };
			struct{ T u, v;                        };
			struct{ Vec<2, T> uv;                  };
			struct{ Vec<2, T> xy;                  };
			struct{ T _unused1; Vec<2, T> yz; };
		};
		static const Vec<3, T> UnitX, UnitY, UnitZ, Origin;
		static const Vec<3, T> UnitAxes[3];
	};
	template<typename T> const Vec<3, T> Vec<3, T>::UnitX       = {1,0,0};
	template<typename T> const Vec<3, T> Vec<3, T>::UnitY       = {0,1,0};
	template<typename T> const Vec<3, T> Vec<3, T>::UnitZ       = {0,0,1};
	template<typename T> const Vec<3, T> Vec<3, T>::Origin      = {0,0,0};
	template<typename T> const Vec<3, T> Vec<3, T>::UnitAxes[3] = {{1,0,0}
	                                                              ,{0,1,0}
	                                                              ,{0,0,1}
	                                                              };

	template<typename T>
	struct Vec<4,T>{
		Vec<4,T>(T nx, T ny, T nz, T nw) : x(nx), y(ny), z(nz), w(nw){}
		union{
		    T elements[4];
			struct{ T x, y, z, w;                   };
			struct{ T u, v;                         };
			struct{ Vec<2, T> xy, zw;               };
			struct{ Vec<2, T> uv;                   };
			struct{ Vec<3, T> xyz;                  };
			struct{ T _unused1; Vec<3, T> yzw; };
			struct{ T _unused2; Vec<2, T> yz;  };
		};
		static const Vec<4, T> UnitX, UnitY, UnitZ, UnitW, Origin;
		static const Vec<4, T> UnitAxes[4];
	};
	template<typename T> const Vec<4, T> Vec<4, T>::UnitX       = {1,0,0,0};
	template<typename T> const Vec<4, T> Vec<4, T>::UnitY       = {0,1,0,0};
	template<typename T> const Vec<4, T> Vec<4, T>::UnitZ       = {0,0,1,0};
	template<typename T> const Vec<4, T> Vec<4, T>::UnitW       = {0,0,0,1};
	template<typename T> const Vec<4, T> Vec<4, T>::Origin      = {0,0,0,0};
	template<typename T> const Vec<4, T> Vec<4, T>::UnitAxes[4] = {{1,0,0,0}
	                                                              ,{0,1,0,0}
	                                                              ,{0,0,1,0}
	                                                              ,{0,0,0,1}
	                                                              };
}

#pragma GCC diagnostic pop // re-enable -Wpedantic

template<typename T> using Vec2 = tempo::Vec<2,T>;
template<typename T> using Vec3 = tempo::Vec<3,T>;
template<typename T> using Vec4 = tempo::Vec<4,T>;
typedef Vec2<u32>  Vec2u;
typedef Vec2<s32>  Vec2s;
typedef Vec2<r32>  Vec2f;
typedef Vec2<r64>  Vec2d;
typedef Vec2<real> Vec2r;
typedef Vec3<u32>  Vec3u;
typedef Vec3<s32>  Vec3s;
typedef Vec3<r32>  Vec3f;
typedef Vec3<r64>  Vec3d;
typedef Vec3<real> Vec3r;
typedef Vec4<u32>  Vec4u;
typedef Vec4<s32>  Vec4s;
typedef Vec4<r32>  Vec4f;
typedef Vec4<r64>  Vec4d;
typedef Vec4<real> Vec4r;

template<typename T>
tempo::Vec<2,T> operator+=(tempo::Vec<2,T>& lhs, const tempo::Vec<2, T>& rhs){
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}
template<typename T>
tempo::Vec<3,T> operator+=(tempo::Vec<3,T>& lhs, const tempo::Vec<3, T>& rhs){
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}
template<typename T>
tempo::Vec<4,T> operator+=(tempo::Vec<4,T>& lhs, const tempo::Vec<4, T>& rhs){
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	lhs.w += rhs.w;
	return lhs;
}
template<u32 T_DIM, typename T>
tempo::Vec<T_DIM, T> operator+(const tempo::Vec<T_DIM,T>& lhs, const tempo::Vec<T_DIM,T>& rhs){
	tempo::Vec<T_DIM, T> result = lhs;
	result += rhs;
	return result;
}



template<typename T>
tempo::Vec<2,T> operator-=(tempo::Vec<2,T>& lhs, const tempo::Vec<2, T>& rhs){
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}
template<typename T>
tempo::Vec<3,T> operator-=(tempo::Vec<3,T>& lhs, const tempo::Vec<3, T>& rhs){
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}
template<typename T>
tempo::Vec<4,T> operator-=(tempo::Vec<4,T>& lhs, const tempo::Vec<4, T>& rhs){
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	lhs.w -= rhs.w;
	return lhs;
}
template<u32 T_DIM, typename T>
tempo::Vec<T_DIM, T> operator-(const tempo::Vec<T_DIM,T>& lhs, const tempo::Vec<T_DIM,T>& rhs){
	tempo::Vec<T_DIM, T> result = lhs;
	result -= rhs;
	return result;
}



template<typename T>
tempo::Vec<2,T> operator*=(tempo::Vec<2,T>& lhs, const tempo::Vec<2, T>& rhs){
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}
template<typename T>
tempo::Vec<3,T> operator*=(tempo::Vec<3,T>& lhs, const tempo::Vec<3, T>& rhs){
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	lhs.z *= rhs.z;
	return lhs;
}
template<typename T>
tempo::Vec<4,T> operator*=(tempo::Vec<4,T>& lhs, const tempo::Vec<4, T>& rhs){
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	lhs.z *= rhs.z;
	lhs.w *= rhs.w;
	return lhs;
}
template<typename T>
tempo::Vec<2,T> operator*=(tempo::Vec<2,T>& lhs, T rhs){
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}
template<typename T>
tempo::Vec<3,T> operator*=(tempo::Vec<3,T>& lhs, T rhs){
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	return lhs;
}
template<typename T>
tempo::Vec<4,T> operator*=(tempo::Vec<4,T>& lhs, T rhs){
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	lhs.w *= rhs;
	return lhs;
}
template<u32 T_DIM, typename T>
tempo::Vec<T_DIM, T> operator*(const tempo::Vec<T_DIM,T>& lhs, const tempo::Vec<T_DIM,T>& rhs){
	tempo::Vec<T_DIM, T> result = lhs;
	result *= rhs;
	return result;
}
template<u32 T_DIM, typename T, typename T_SCALAR>
tempo::Vec<T_DIM, T> operator*(const tempo::Vec<T_DIM,T>& lhs, T_SCALAR rhs){
	tempo::Vec<T_DIM, T> result = lhs;
	result *= (T)rhs;
	return result;
}
template<u32 T_DIM, typename T, typename T_SCALAR>
tempo::Vec<T_DIM, T> operator*(T_SCALAR lhs, const tempo::Vec<T_DIM,T>& rhs){
	tempo::Vec<T_DIM, T> result = rhs;
	result *= (T)lhs;
	return result;
}


template<typename T>
tempo::Vec<2,T> operator/=(tempo::Vec<2,T>& lhs, const tempo::Vec<2, T>& rhs){
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	return lhs;
}
template<typename T>
tempo::Vec<3,T> operator/=(tempo::Vec<3,T>& lhs, const tempo::Vec<3, T>& rhs){
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	lhs.z /= rhs.z;
	return lhs;
}
template<typename T>
tempo::Vec<4,T> operator/=(tempo::Vec<4,T>& lhs, const tempo::Vec<4, T>& rhs){
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	lhs.z /= rhs.z;
	lhs.w /= rhs.w;
	return lhs;
}
template<typename T>
tempo::Vec<2,T> operator/=(tempo::Vec<2,T>& lhs, T rhs){
	lhs.x /= rhs;
	lhs.y /= rhs;
	return lhs;
}
template<typename T>
tempo::Vec<3,T> operator/=(tempo::Vec<3,T>& lhs, T rhs){
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;
	return lhs;
}
template<typename T>
tempo::Vec<4,T> operator/=(tempo::Vec<4,T>& lhs, T rhs){
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;
	lhs.w /= rhs;
	return lhs;
}
template<u32 T_DIM, typename T>
tempo::Vec<T_DIM, T> operator/(const tempo::Vec<T_DIM,T>& lhs, const tempo::Vec<T_DIM,T>& rhs){
	tempo::Vec<T_DIM, T> result = lhs;
	result /= rhs;
	return result;
}
template<u32 T_DIM, typename T>
tempo::Vec<T_DIM, T> operator/(const tempo::Vec<T_DIM,T>& lhs, T rhs){
	tempo::Vec<T_DIM, T> result = lhs;
	result /= rhs;
	return result;
}

template<typename T>
tempo::Vec<3, T> operator-(const tempo::Vec<3,T>& vec){
	return {-vec.x, -vec.y, -vec.z};
}

template<typename T>
tempo::Vec<4, T> operator-(const tempo::Vec<4,T>& vec){
	return {-vec.x, -vec.y, -vec.z, -vec.w};
}

namespace tempo{
	/// \brief Computes dot product of two vectors
	template<typename T>
	T dot(const Vec2<T>& a, const Vec2<T>& b){
		return a.x*b.x + a.y*b.y;
	}
	template<typename T>
	T dot(const Vec3<T>& a, const Vec3<T>& b){
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}
	template<typename T>
	T dot(const Vec4<T>& a, const Vec4<T>& b){
		return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
	}

	/// \brief Computes cross product of two vectors
	template<typename T>
	Vec3<T> cross(const Vec3<T>& a, const Vec3<T>& b){
		return { a.y*b.z - a.z*b.y
			   , a.z*b.x - a.x*b.z
			   , a.x*b.y - a.y*b.x };
	}

	template<u32 T_DIM, typename T>
	real distanceSq(const Vec<T_DIM, T>& a, const Vec<T_DIM, T>& b){
		Vec<T_DIM, T> d = a - b;
		return dot(d, d);
	}

	template<u32 T_DIM, typename T>
	real distance(const Vec<T_DIM, T>& a, const Vec<T_DIM, T>& b){
		return tempo::sqrt(distanceSq<T_DIM, T>(a,b));
	}

	template<u32 T_Dims, typename T>
	real lengthSq(const Vec<T_Dims, T>& v){
		return distanceSq<T_Dims, T>(v, Vec<T_Dims, T>::Origin);
	}

	template<u32 T_Dims, typename T>
	real length(const Vec<T_Dims, T>& v){
		return distance(v, Vec<T_Dims, T>::Origin);
	}

	/// \brief Computes magnitude of vector
	template<u32 T_Dims, typename T>
	real mag(const Vec<T_Dims, T>& v){ return length(v); }

	/// \brief Computes magnitude of vector squared
	template<u32 T_Dims, typename T>
	real magSq(const Vec<T_Dims, T>& v){ return lengthSq(v); }

	template<u32 T_Dims, typename T>
	Vec<T_Dims, T> normalized(const Vec<T_Dims, T>& v){ return v / length(v); }

	/// \brief Computes minimum angle between two vectors - direction of angle could be
	/// either clockwise or anti-clockwise
	template<u32 T_Dims, typename T>
	inline Angle angleBetween(const Vec<T_Dims, T>& a, const Vec<T_Dims, T>& b){
		return tempo::acos(dot(a,b) / (mag(a) * mag(b)));
	}

	/// \brief Projects the direction vector vec onto the plane defined by the normal vector norm
	template<typename T>
	inline Vec3<T> projectOntoPlane(Vec3<T> vec, Vec3<T> norm){
		return vec - ((dot(vec,norm) / magSq(norm)) * norm);
	}

	template<u32 T_DIM, typename T>
	bool operator==(const tempo::Vec<T_DIM, T>& lhs, const tempo::Vec<T_DIM, T>& rhs){
		// float comparisions are dodgy... check if magnitude of delta is < some value
		return tempo::magSq(lhs - rhs) <= 0.000000001_r;
	}
	template<u32 T_DIM, typename T>
	bool operator!=(const tempo::Vec<T_DIM, T>& lhs, const tempo::Vec<T_DIM, T>& rhs){
		return !(lhs == rhs);
	}
}

#endif
