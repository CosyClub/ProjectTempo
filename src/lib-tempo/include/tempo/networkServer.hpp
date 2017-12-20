////////////////////////////////////////////////////////////////////////////////
/// networkServer.hpp
///
/// Header definitions for use on the Project Tempo server.
////////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_NETWORK_SERVER_HPP
#define TEMPO_NETWORK_SERVER_HPP

#include "networkBase.hpp"

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>
#include <thread>

namespace tempo
{
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

