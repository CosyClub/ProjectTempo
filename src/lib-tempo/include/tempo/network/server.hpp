#ifndef TEMPO_NETWORK_SERVER_HPP
#define TEMPO_NETWORK_SERVER_HPP

#include <tempo/entity/EntityCreationServer.hpp>
#include <tempo/network/base.hpp>
#include <tempo/network/queue.hpp>
#include <tempo/time.hpp>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>

#include <anax/World.hpp>

#include <iostream>
#include <mutex>
#include <thread>

namespace tempo
{
typedef struct {
	sf::Uint32     ip;    // Use sf::IpAddress's toInteger() method
	unsigned short iport; // Incoming port, where we should send messages to
	unsigned short oport; // Outgoing port, port they send messages on
	ClientRole     role;
} clientConnection;

// Map of all the connected clients IP addresses and ID's, with some
// handy typedefs.
// Notes:
//         - Unique assigned Client ID as the key
//         - clientConnection struct as the value
typedef std::pair<uint32_t, tempo::clientConnection> clientpair;
typedef std::map<uint32_t, tempo::clientConnection>  clientmap;
extern clientmap                                     clients;

// Client Map Mutex Lock for when modifying Map
// This should always be used when touching the clients map.
extern std::mutex cmtx;

// timeSyncServer
// WARNING: Should be run on separate thread.
// Server with "master time" for clients to sync to.
//
// Arguments:
//         clock - pointer to server's "master time"
// Returns:
//         void (is a thread)
void timeSyncServer(tempo::Clock *clock);

// listenForNewClients
// Checks for and orchestrates initialisation of new clients on
// `port_sh`.
//
// TODO: Write parameters
//
// Returns:
//         void (is a thread)
void checkForClientCreation(anax::World *world);
void checkForClientDeletion(anax::World &world);

// listenForClientUpdates
// WARNING: Should be run on separate thread.
// Listens and processes any incoming client updates on `port_si`.
//
// Returns:
//         void (is a thread)
void listenForClientUpdates();

// findClientID
// Finds a client with the information given. Note that this will lock
// the clients map from other threads so use as sparingly as possible.
//
// Arguments:
//         ip   - the IP address of the client you wish to find
//         port - the port of the client you wish to find
// Returns:
//         The ID of the client if it exists, or NO_CLIENT_ID (also
//         known as THIS_IS_NOT_THE_CLIENT_YOU_ARE_LOOKING_FOR) if no
//         such client exists.
uint32_t findClientID(sf::Uint32 ip, unsigned short port);

bool sendMessage(tempo::QueueID id, sf::Packet p);

bool broadcastMessage(tempo::QueueID, sf::Packet p);

bool sendMessage(tempo::QueueID id, sf::Packet payload, uint32_t client_id);
}

#endif
