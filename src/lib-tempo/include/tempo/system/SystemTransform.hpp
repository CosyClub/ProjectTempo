#ifndef TEMPO_SYSTEM_POSITION_HPP
#define TEMPO_SYSTEM_POSITION_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/LevelManager.hpp>
#include <tempo/component/ComponentTransform.hpp>

#include <OgreMath.h>
#include <OgreVector3.h>

namespace tempo{

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
