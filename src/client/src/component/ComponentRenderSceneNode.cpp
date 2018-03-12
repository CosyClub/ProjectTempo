#include <client/component/ComponentRenderSceneNode.hpp>

namespace client
{

ComponentRenderSceneNode::ComponentRenderSceneNode(irr::scene::ISceneNode *node) : node(node)
{}

ComponentRenderSceneNode::~ComponentRenderSceneNode()
{
	node->remove();
}

}  // namespace client
