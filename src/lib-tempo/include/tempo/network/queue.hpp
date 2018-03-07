#ifndef TEMPO_NETWORK_QUEUE_HPP
#define TEMPO_NETWORK_QUEUE_HPP

#include <tempo/network/ID.hpp>
#include <tempo/structures.hpp>

#include <SFML/Network.hpp>

namespace tempo
{
////////////////////////////////////////////////////////////////////////
/// Function declarations

// get_system_queue
// Gets a pointer to the network interface queue for a particular system
//
// Usage:
// 	tempo::Queue<Packet>* q = get_system_queue(tempo::SystemQID id)
//
// Arguments:
// 	id	- The systemQID of the system
//
// Returns:
// 	Pointer to a tempo::Queue<sf::Packet>
tempo::Queue<sf::Packet> *get_system_queue(tempo::QueueID);
}

#endif
