////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file GridMotion.hpp
/// \author Jamie Terry
/// \date 2017/11/15
/// \brief Contains declartion of grid motion component and system
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_GRIDMOTION_HPP
#define TEMPO_ENTITY_GRIDMOTION_HPP

#include <OgreVector2.h>

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/Position.hpp>

namespace tempo{
	struct ComponentGridMotion : anax::Component {
		Ogre::Vector2    current;
		Ogre::Vector2    target;
		float            motion_progress;

		ComponentGridMotion();
		ComponentGridMotion(Ogre::Vector2 pos);
		ComponentGridMotion(Ogre::Real x, Ogre::Real y);

		/////////////////////////////////////////////////////////////////////
		/// \brief Causes the component to represent a motion by specified amounts
		/// Only takes effect if entity is not already in motion
		/////////////////////////////////////////////////////////////////////
		bool moveBy(Ogre::Real x, Ogre::Real y);
	};

	struct SystemGridMotion : anax::System<anax::Requires<ComponentPosition, ComponentGridMotion>> {
		// Dimensions of grid
		int min_x;
		int min_y;
		int max_x;
		int max_y;

		SystemGridMotion(int nmin_x, int nmin_y, int nmax_x, int nmax_y);

		void update(float dt);
	};
}

#endif
