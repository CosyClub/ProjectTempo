#ifndef CLIENT_SYSTEM_RENDER_ATTACK_HPP
#define CLIENT_SYSTEM_RENDER_ATTACK_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <client/component/ComponentRenderAttack.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>
#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentWeapon.hpp>

#include <ISceneManager.h>

#include <irrlicht.h>

namespace client
{
struct SystemRenderAttack
    : anax::System<anax::Requires<tempo::ComponentAttack,
                                  client::ComponentRenderSceneNode>> {
	void setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver * driver, irr::IrrlichtDevice *device);

	void update(irr::IrrlichtDevice *device);

	irr::core::dimension2d<irr::f32> original_size;
	uint32_t oldTime;
	uint32_t now;
	uint32_t elapsed;
};
}

#endif
