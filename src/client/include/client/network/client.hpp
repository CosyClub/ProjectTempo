#ifndef TEMPO_NETWORK_CLIENT_HPP
#define TEMPO_NETWORK_CLIENT_HPP

#include <tempo/config.hpp>
#include <tempo/network/ID.hpp>
#include <tempo/network/base.hpp>
#include <tempo/time.hpp>

#include <anax/Entity.hpp>
#include <anax/World.hpp>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>

#include <iostream>
#include <thread>

#include <cassert>

namespace tempo
{
// timeSyncClient
// Syncs client's "dirty time" with server's "master time"
//
// Arguments:
//         clock - pointer to current client clock with "dirty time"
// Returns:
//         The offset, in microseconds, of the server and local client
//         clock, where offset = time(server) - time(client)
//         If this function returns 0, assume an error has occured.
sf::Int64 timeSyncClient(tempo::Clock *clock);

// sendMessageToServer
// Sends a message to the server. No guarentee of delivery given.
//
// Arguments:
//         id        - ID of the System Queue or Handshake message to
//                     deliver the message to. {use static_cast<int>()}
//         payload   - The payload to send to the message queue.
// Returns:
//         bool - true if sent, false if unable to send.
bool sendMessage(QueueID id, sf::Packet payload);

// listenForServerUpdates
// WARNING: Should be run on separate thread.
// Listens and processes any updates (delta's) from the server on
// `port_ci`.
//
// Returns:
//         void (is a thread)
void listenForServerUpdates(std::atomic<bool> &running);

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
//         world    - Anax world to put entities from the server in.
//         system_gm - The Level Manager System for the client for the
//                    entities received from the server.
// Returns:
//         void - nothing is returned. Note side effects above.
bool connectToAndSyncWithServer(ClientRole roleID, ClientRoleData &roleData, anax::World &world);

void sendHeatbeat();

void disconnectFromServer(anax::Entity &entity_player);

bool broadcastMessage(QueueID id, sf::Packet p);
}

#endif
