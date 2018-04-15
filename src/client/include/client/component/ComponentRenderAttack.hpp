#ifndef CLIENT_COMPONENT_RENDER_ATTACK_HPP
#define CLIENT_COMPONENT_RENDER_ATTACK_HPP

#include <anax/Component.hpp>

#include <IBillboardSceneNode.h>
#include <ISceneNode.h>

namespace client
{
struct ComponentRenderAttack : public anax::Component {
	irr::scene::IBillboardSceneNode *node;
	ComponentRenderAttack(irr::scene::IBillboardSceneNode *node);
	~ComponentRenderAttack();
};

}  // namespace client

#endif
