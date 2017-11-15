////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Position.hpp
/// \author Jamie Terry
/// \date 2017/11/14
/// \brief Contains decleration of Position component and system
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_POSITION_HPP
#define TEMPO_ENTITY_POSITION_HPP

#include <OgreVector3.h>

#include <anax/System.hpp>
#include <anax/Component.hpp>

namespace tempo{
	struct ComponentPosition : anax::Component {
		/// \brief Offset from world origin to entity's local origin
		Ogre::Vector3    position;

		/// \brief Rotation of the entity about its local origin
		//Ogre::Quaternion rotation;

		/// \brief Delta position per second
		Ogre::Vector3    velocity;

		/// \brief Delta rotation per second
		//Ogre::Quaternion rotational_velocity;

		ComponentPosition();
		ComponentPosition(Ogre::Vector3 pos);
		ComponentPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);
	};

	struct SystemPosition : anax::System<anax::Requires<ComponentPosition>> {
		void update(double dt);
	};
}

#endif
