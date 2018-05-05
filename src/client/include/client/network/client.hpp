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

// Server Listener (To be run on a separate thread)
void listenForServerUpdates(std::atomic<bool> &running);

// Functions for connnection, syncing and disconnection
sf::Int64 timeSyncClient(tempo::Clock *clock);
bool connectToAndSyncWithServer(anax::World &world);
bool joinGame(ClientRole roleID, ClientRoleData &roleData, anax::World &world);
void disconnectFromServer(anax::Entity &entity_player);

// Functions for message sending
void sendHeatbeat();
bool sendMessage(QueueID id, sf::Packet payload);
bool broadcastMessage(QueueID id, sf::Packet p);

}

#endif
