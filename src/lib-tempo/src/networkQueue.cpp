#include <tempo/config.hpp>
#include <tempo/networkQueue.hpp>

#include <tempo/entity/SystemQID.hpp>

#include <SFML/Network.hpp>

namespace tempo
{
	tempo::Queue<sf::Packet> system_queues[tempo::QID_RESERVED_TOP];

	tempo::Queue<sf::Packet>* get_system_queue(tempo::SystemQID id)
	{
		return &(system_queues[id]);
	}
}
