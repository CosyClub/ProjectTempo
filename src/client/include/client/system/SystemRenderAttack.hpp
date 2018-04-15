#ifndef CLIENT_SYSTEM_RENDER_ATTACK_HPP
#define CLIENT_SYSTEM_RENDER_ATTACK_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <client/component/ComponentRenderSceneNode.hpp>
#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentWeapon.hpp>

#include <ISceneManager.h>

namespace client
{
struct SystemRenderAttack
    : anax::System<anax::Requires<tempo::ComponentAttack,
                                  tempo::ComponentWeapon,
                                  client::ComponentRenderSceneNode>> {
	void setup(irr::scene::ISceneManager *smgr);

	void update();

	irr::core::dimension2d<irr::f32> original_size;
};
}

#endif
