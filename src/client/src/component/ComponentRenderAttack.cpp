#include <client/component/ComponentRenderAttack.hpp>

namespace client
{
ComponentRenderAttack::ComponentRenderAttack(irr::scene::IBillboardSceneNode *node)
    : node(node)
{
}

ComponentRenderAttack::~ComponentRenderAttack()
{
	node->remove();
}

}  // namespace client
