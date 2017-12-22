////////////////////////////////////////////////////////////////////////////////
/// networkServer.hpp
///
/// Headers for tempo server-system queues
////////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_NETWORK_QUEUE_HPP
#define TEMPO_NETWORK_QUEUE_HPP

#include <tempo/structures.hpp>
#include <tempo/entity/SystemQID.hpp>

#include <SFML/Network.hpp>


namespace tempo
{
	tempo::Queue<sf::Packet>* get_system_queue(tempo::SystemQID);
}

#endif

