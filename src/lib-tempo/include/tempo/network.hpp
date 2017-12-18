////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Application.hpp
/// \author Raef Coles
/// \date 2017/11/13
/// \Functions and classes related to timing
//////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_NETWORK_HPP
#define TEMPO_NETWORK_HPP

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>

namespace tempo
{
	// Default Port for Time Sync (TS) protocol
	#define NET_PORT_TS 1337
	// Default Address - TODO Change this
	#define NET_ADDR "127.0.0.0"
	// Wait time for time sync protocol (millisecs)
	#define TIMESYNC_DELTA 500

	// tcpRemoteToStr - Returns string with remote host and port of a TCP
	//                  socket
	// Arguments:
	//         client - The TCP client socket to return the remote host and
	//                  port of
	// Returns:
	//         A string with the remote host and port of the given TCP
	//         socket in the form of "HOST:PORT"
	#define tcpRemoteToStr(client) \
		(client->getRemoteAddress().toString() + ":" + \
			std::to_string(client->getRemotePort()))

	// timeSyncClient - Syncs client's "dirty time" with server's "master
	//                  time"
	// Arguments:
	//         clock - pointer to current client clock with "dirty time"
	// Returns:
	//         The correct "master time" as of call completion
        sf::Time timeSyncClient(tempo::Clock *clock);

	// timeSyncServer - Server with "master time" for clients to sync to.
	//                  Should be run in separate thread.
	// Arguments:
	//         clock - pointer to server's "master time"
	// Returns:
	//         void
	void timeSyncServer(tempo::Clock *clock);
}

#endif
