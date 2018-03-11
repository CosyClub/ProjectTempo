#include <client/component/ComponentRenderSceneNode.hpp>

namespace client
{
	ComponentRenderSceneNode::ComponentRenderSceneNode(irr::scene::ISceneNode *node, std::string spritesheet, int spriteRows, int spriteCols)
		: node(node)
	{
		this->spritesheet = spritesheet;
		this->spriteRows = spriteRows;
		this->spriteCols = spriteCols;
	}

ComponentRenderSceneNode::~ComponentRenderSceneNode()
{
	node->remove();
}

}  // namespace client
