#ifndef CLIENT_SYSTEM_SYSTEMRENDERATTACK_HPP
#define CLIENT_SYSTEM_SYSTEMRENDERATTACK_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <tempo/component/ComponentAttack.hpp>

#include <ISceneManager.h>

namespace client
{
class SystemRenderAttack
	: public anax::System<anax::Requires<tempo::ComponentAttack>> {

private:
	irr::scene::ISceneManager* smgr;

public:
	SystemRenderAttack(irr::scene::ISceneManager* smgr);

	void update();
};
}

#endif
