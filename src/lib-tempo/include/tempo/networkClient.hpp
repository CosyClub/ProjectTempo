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
	// will establish the connection and initialise the level state. Delta's
	// may also be sent by the server but should not be applied until this
	// function call has finished.
	//
	// Returns:
	//        The ClientID/Token to request a role with, or 0 on failure. 
	uint32_t connectToServer();

	// requestRolep
	// Should be run once connectToServer has been run, and game is ready 
	// for the player to carry out their requested role. The server will
	// create the required entity for the role and send it to the client.
	// This function will then orchestrate the creation of this entity, so
	// the game will be in a ready to play state upon completion.
	//
	// Arguments:
	//         roleID   - Client's requested role from the server.
	//         roleData - Point to client's requested role data (if any)
	//                    null inputs assume no role data requied.
	// Returns:
	//         void - nothing is returned. Player entity will be added to 
	//                the ECS, and the game will be in a playable state.
	void requestRole(ClientRole roleID, ClientRoleData &roleData);

}

#endif
