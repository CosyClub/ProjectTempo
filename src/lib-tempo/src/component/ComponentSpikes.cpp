#include <tempo/component/ComponentSpikes.hpp>
#include <tempo/component/ComponentStage.hpp>

#include <tempo/network/base.hpp>

namespace tempo {
	ComponentSpikes::ComponentSpikes(std::vector<glm::ivec2> positions)
		: spike_positions(positions) {

		isTriggered = true;
	}

	/////
	// Required for networking
	/////
	ComponentSpikes::ComponentSpikes(sf::Packet p)
	{
		sf::Uint32 size = 0;

		p >> isTriggered;

		p >> size;
		for (uint32_t i = 0; i < size; i++) {
			glm::ivec2 v;
			p >> v;
			spike_positions.push_back(v);
		}

	}


	ComponentID ComponentSpikes::getId()
	{
		return ComponentID::SPIKES;
	}

	sf::Packet ComponentSpikes::dumpComponent()
	{
		sf::Packet p;

		p << isTriggered;

		p << (sf::Uint32) spike_positions.size();
		for (uint32_t i = 0; i < spike_positions.size(); i++) {
			p << spike_positions[i];
		}

		return p;
	}

}  // namespace tempo
