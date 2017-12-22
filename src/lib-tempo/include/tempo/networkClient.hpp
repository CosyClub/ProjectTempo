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

	// sendMessageToServer
	// Sends a message to the server. No guarentee of delivery given.
	//
	// Arguments:
	//         id        - ID of the System Queue or Handshake message to 
	//                     deliver the message to. {use static_cast<int>()}
	//         payload   - The payload to send to the message queue.
	//         isHandshake - Whether or not the message is a handshake 
	//                     message (true) or game message (false, default).
	// Returns:
	//         bool - true if sent, false if unable to send.
	bool sendMessage(SystemQID id, sf::Packet payload, bool isHandshake);

	// listenForServerUpdates
	// WARNING: Should be run on separate thread.
	// Listens and processes any updates (delta's) from the server on 
	// `port_ci`.
	//
	// Returns:
	//         void (is a thread)
	void listenForServerUpdates();

	// connectToAndSyncWithServer
	// WARNING: This requires the listenForServerUpdates thread to have
	//          started succesfully before being run.
	// Connects to the server on addr_r:port_sh, syncs the entire level and 
	// then requests the given role.
	//
	// Side Effects:
	//  - This function will initialise the entire level to the same state 
	//    as the server. The client will be registered for updates which 
	//    should be actioned when this function returns.
	//  - The server will create the required entity for the requesting role 
	//    and send it to the client. This function will then orchestrate the 
	//    creation of this entity.
	//
	// Arguments:
	//         roleID   - Client's requested role from the server.
	//         roleData - Point to client's requested role data (if any)
	//                    null inputs assume no role data requied.
	// Returns:
	//         void - nothing is returned. Note side effects above.
	void connectToAndSyncWithServer(ClientRole roleID, 
	                                ClientRoleData &roleData);

}

#endif
