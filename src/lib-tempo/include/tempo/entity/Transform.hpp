////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Transform.hpp
/// \author Jamie Terry
/// \date 2017/11/14
/// \brief Contains decleration of Transform component
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_POSITION_HPP
#define TEMPO_ENTITY_POSITION_HPP

#include <OgreVector3.h>

#include <anax/System.hpp>
#include <anax/Component.hpp>

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