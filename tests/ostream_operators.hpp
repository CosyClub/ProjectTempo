////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \brief Contains overloads for ostream operators for types we want
/// to comapre in test cases. This allows for pretty printing if result
/// is not what is expected
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_UNITTESTS_OSTREAMOPERATORS_HPP
#define TEMPO_UNITTESTS_OSTREAMOPERATORS_HPP

#include <iostream>
#include <sstream>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector3d.h>
#include <SColor.h>

namespace glm {
	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, glm::tvec2<T> v){
		return os << "(" << v.x << ", " << v.y << ")";
	}

	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, glm::tvec3<T> v){
		return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	}

	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, glm::tvec4<T> v){
		return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	}
}

namespace irr {
	namespace core {
		inline std::ostream& operator<<(std::ostream& os, irr::core::vector3df v){
		  return os << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";
		}
	}
	namespace video {
		inline std::ostream& operator<<(std::ostream& os, irr::video::SColor c){
			return os << "{ r: " << c.getRed()
			          << ", g: " << c.getGreen()
			          << ", b: " << c.getBlue()
			          << ", a: " << c.getAlpha()
			          << " }";
		}
	}
}

#endif
