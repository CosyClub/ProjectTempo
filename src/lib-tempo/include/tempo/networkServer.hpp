////////////////////////////////////////////////////////////////////////////////
/// networkServer.hpp
///
/// Header definitions for use on the Project Tempo server.
////////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_NETWORK_SERVER_HPP
#define TEMPO_NETWORK_SERVER_HPP

#include <tempo/networkBase.hpp>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>
#include <thread>

namespace tempo
{
	typedef struct {
		sf::Uint32 ip;
		unsigned short port;
	} clientConnection;

	// Map of all the connected clients IP addresses and ID's
	// Use sf::IpAddress toInt() for the key (int representation of IP addr)
	// Assigned Client ID as the value
	extern std::map<uint32_t, tempo::clientConnection> clients;
	
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
	// WARNING: Should be run on separate thread.
	// Listens for and orchestrates initialisation of new clients
	//
	// Arguments:
	//         port - The port to listen on
	// Returns:
	//         void (is a thread)
	void listenForNewClients(unsigned short port);

	// listenForClientUpdates
	// WARNING: Should be run on separate thread.
	// Listens and processes any incoming client updates.
	//
	// Arguments:
	//         port - The port ot listen for client updates from.
	// Returns:
	//         void (is a thread)
	void listenForClientUpdates(unsigned short port);
}

#endif

