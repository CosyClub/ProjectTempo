#include <tempo/config.hpp>

namespace tempo
{
	//Hidden queue list
	tempo::Queue<sf::Packet> system_queues[tempo::QID_RESERVED_TOP];

	tempo::Queue<sf::Packet>* get_system_queue(tempo::SystemQID id)
	{
		return &(system_queues[id]);
	}
}
