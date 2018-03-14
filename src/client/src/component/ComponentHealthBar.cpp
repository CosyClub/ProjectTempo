#include <client/component/ComponentHealthBar.hpp>

namespace client
{
ComponentHealthBar::ComponentHealthBar(irr::scene::IBillboardSceneNode *node)
    : node(node)
{
}

ComponentHealthBar::~ComponentHealthBar()
{
	node->remove();
	// If you ever need to delete a healthbar go and find Raef
}

}  // namespace client
