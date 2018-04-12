#ifndef CLIENT_SYSTEM_SYSTEMRENDERHEALING_HPP
#define CLIENT_SYSTEM_SYSTEMRENDERHEALING_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStage.hpp>
#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentHealth.hpp>

#include <ISceneManager.h>
#include <IParticleSystemSceneNode.h>

#include <unordered_map>
#include <vector>
#include <utility>

namespace client
{
class SystemRenderHealing
	: public anax::System<anax::Requires<tempo::ComponentStagePosition,
	                                     tempo::ComponentStage,
																			 tempo::ComponentHealth,
																			 tempo::ComponentCombo>> {

private:
	irr::scene::ISceneManager*                                smgr;
	irr::video::IVideoDriver*                                 driver;
	std::vector<std::pair<irr::scene::IParticleSystemSceneNode*,int>> particle_systems;
public:
	SystemRenderHealing(irr::video::IVideoDriver * driver,
	                   irr::scene::ISceneManager* smgr);

	// Healing
	// Updates the healing particle effect
	//
	// Arguments:
	//          colour_dark   - darkest   colour used for the particle system
	//          colour_bright - brightest colour used for the particle system
	// Returns:
	//          void
	void update(irr::video::SColor colour_dark   = irr::video::SColor(0, 000, 200, 40),
              irr::video::SColor colour_bright = irr::video::SColor(0, 000, 230, 70));

	void endBeat();
};
}

#endif
