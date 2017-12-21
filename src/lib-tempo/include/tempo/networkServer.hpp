////////////////////////////////////////////////////////////////////////////////
/// networkServer.hpp
///
/// Header definitions for use on the Project Tempo server.
////////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_NETWORK_SERVER_HPP
#define TEMPO_NETWORK_SERVER_HPP

#include <mutex>
#include <thread>

#include <tempo/networkBase.hpp>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>


namespace tempo
{
	typedef struct {
		sf::Uint32 ip;       // Use sf::IpAddress's toInt() method
		unsigned short port;
	} clientConnection;

	// Reserved client ID for a null client
	#define NO_CLIENT_ID 0

	// Map of all the connected clients IP addresses and ID's, with some
	// handy typedefs.
	// Notes:
	//         - Unique assigned Client ID as the key
	//         - clientConnection struct as the value 
	typedef std::pair<uint32_t, tempo::clientConnection> clientpair;
	typedef std::map<uint32_t, tempo::clientConnection>  clientmap;
	extern clientmap clients;

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

