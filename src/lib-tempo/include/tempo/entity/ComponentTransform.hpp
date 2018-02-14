#ifndef TEMPO_COMPONENT_POSITION_HPP
#define TEMPO_COMPONENT_POSITION_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/LevelManager.hpp>

#include <OgreMath.h>
#include <OgreVector3.h>

namespace tempo{
	struct ComponentTransform : anax::Component {
		/// \brief Offset from world origin to entity's local origin
		Ogre::Vector3    position;

		/// \brief Rotation of the entity about its local origin
		Ogre::Quaternion rotation;

		ComponentTransform();
		ComponentTransform(Ogre::Vector3 pos);
		ComponentTransform(Ogre::Real x, Ogre::Real y, Ogre::Real z);
	};
}

#endif
