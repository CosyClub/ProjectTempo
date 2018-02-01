#ifndef TEMPO_ENTITY_POSITION_HPP
#define TEMPO_ENTITY_POSITION_HPP

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

	/////////////////////////////////////////////////////////////////////
	/// \brief System which has knowledge of the world's geometry.
	/// Hence this system is responsible for rendering the world's tiles,
	/// and managing the movement of entities on the grid
	/////////////////////////////////////////////////////////////////////
	class SystemUpdateTransforms : public anax::System<anax::Requires<ComponentTransform,
	                                                                  ComponentGridPosition,
	                                                                  ComponentGridMotion
	                                                                  >>
	{
	public:
		/////////////////////////////////////////////////////////////////////
		/// \brief Updates transform components with data stored in grid position
		/// and grid motion
		/////////////////////////////////////////////////////////////////////
		void update(SystemLevelManager& level);
	};
}

#endif
