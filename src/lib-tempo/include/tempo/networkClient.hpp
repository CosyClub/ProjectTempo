////////////////////////////////////////////////////////////////////////////////
/// networkClient.hpp
///
/// Header definitions for use on the Project Tempo client.
////////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_NETWORK_CLIENT_HPP
#define TEMPO_NETWORK_CLIENT_HPP

#include <tempo/networkBase.hpp>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>
#include <thread>

namespace tempo
{
	// timeSyncClient
	// Syncs client's "dirty time" with server's "master time"
	//
	// Arguments:
	//         clock - pointer to current client clock with "dirty time"
	// Returns:
	//         The correct "master time" as of call completion
        sf::Time timeSyncClient(tempo::Clock *clock);

	// listenForServerUpdates
	// WARNING: Should be run on separate thread.
	// Listens and processes any updates (delta's) from the server.
	//
	// Arguments:
	//         port - the port to listen for updates on.
	// Returns:
	//         void (is a thread)
	void listenForServerUpdates(int port);

	// connectToServer
	// First part of connecting a client to a remote server. The client
	// should be listening for server updates before this is run. This call
	// will establish the connection and return the level state. Delta's
	// may also be sent by the server but should not be applied until the
	// level has been initailised.
	//
	// Arguments:
	//         ipAddress - Pointer to an SFML IP Address object, containing
	//                     the IP address of the server to connect to.
	//         port      - Port of the server we wish to connect to.
	//         ourPort   - Port in which we, the client, are listening for
	//                     server updates on.
	// Returns:
	//         TODO Level data
	int connectToServer(sf::IpAddress *ipAddress, 
	                    unsigned short port,
	                    unsigned short ourPort);

	// requestRolep
	// Should be run one connectToServer has been run, and the level
	// initialised and ready for the player to play/spectate/do whatevr they
	// request to do in their role.
	//
	// Arguments:
	//         roleID   - Client's requested role from the server
	//         roleData - Point to client's requested role data (if any)
	//                    null inputs assume no role data requied
	// Returns:
	//         TODO The requested client entity
	int requestRole(tempo::ClientRole roleID, tempo::ClientRoleData *roleData);

}

#endif
