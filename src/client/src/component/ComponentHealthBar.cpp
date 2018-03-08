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
}

}  // namespace client
