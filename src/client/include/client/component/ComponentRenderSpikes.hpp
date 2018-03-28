#ifndef CLIENT_COMPONENT_RENDER_SPIKES_HPP
#define CLIENT_COMPONENT_RENDER_SPIKES_HPP

#include <IMeshSceneNode.h>
#include <anax/Component.hpp>

struct SpikeNode {
	irr::scene::IMeshSceneNode *spikeNode;
};

namespace client
{

struct ComponentRenderSpikes : anax::Component {

	std::vector<SpikeNode> spikes;

	ComponentRenderSpikes();
	~ComponentRenderSpikes();
};

} // namespace client

#endif
