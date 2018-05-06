#ifndef CLIENT_SYSTEM_SYSTEMRENDERATTACK_HPP
#define CLIENT_SYSTEM_SYSTEMRENDERATTACK_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentStage.hpp>

#include <client/system/SystemStageRenderer.hpp>
#include <glm/vec2.hpp>

namespace client
{

class SystemRenderAttack : public anax::System<anax::Requires<tempo::ComponentAttack,
	                                                      tempo::ComponentStagePosition,
	                                                      tempo::ComponentStageRotation,
	                                                      tempo::ComponentStage>>
{
	public:
	SystemRenderAttack();

	void update(SystemStageRenderer& sr, irr::video::SColor colour);
};

}

#endif
