////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Transform.cpp
/// \author Jamie Terry
/// \date 2017/11/14
/// \brief Contains definition of Transform system functions
////////////////////////////////////////////////////////////////////////////

#include <tempo/entity/Transform.hpp>

namespace tempo{
	ComponentTransform::ComponentTransform() : ComponentTransform(0, 0, 0){
		// empty body
	}

	ComponentTransform::ComponentTransform(Ogre::Vector3 pos) : ComponentTransform(pos.x, pos.y, pos.z){
		// empty body
	}

	ComponentTransform::ComponentTransform(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
		this->position.x = x;
		this->position.y = y;
		this->position.z = z;
		this->rotation= Ogre::Quaternion();
	}
}
