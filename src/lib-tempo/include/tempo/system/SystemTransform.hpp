#ifndef TEMPO_SYSTEM_POSITION_HPP
#define TEMPO_SYSTEM_POSITION_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentTransform.hpp>
#include <tempo/system/SystemLevelManager.hpp>

namespace tempo
{
/////////////////////////////////////////////////////////////////////
/// \brief System which has knowledge of the world's geometry.
/// Hence this system is responsible for rendering the world's tiles,
/// and managing the movement of entities on the grid
/////////////////////////////////////////////////////////////////////
class SystemUpdateTransforms
    : public anax::System<
        anax::Requires<ComponentTransform, ComponentStagePosition, ComponentStageTranslation>>
{
   public:
	/////////////////////////////////////////////////////////////////////
	/// \brief Updates transform components with data stored in grid position
	/// and grid motion
	/////////////////////////////////////////////////////////////////////
	void update(SystemLevelManager &level);
};
}

#endif
