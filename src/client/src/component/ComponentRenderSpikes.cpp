#include <client/component/ComponentRenderSpikes.hpp>

namespace client
{
ComponentRenderSpikes::ComponentRenderSpikes() {
}

ComponentRenderSpikes::~ComponentRenderSpikes()
{
	for(SpikeNode spike : spikes){
		spike.spikeNode->remove();
	}
}

}  // namespace client
