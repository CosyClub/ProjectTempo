////////////////////////////////////////////////////////////////////////////////
/// \file main.cpp
/// \author Anthony Wharton
/// \date 2017/11/13
/// \brief Contains entry point for RaveCave Server
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdio>
#include <thread>
#include <vector>

#include <tempo/time.hpp>
#include <tempo/networkServer.hpp>
#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>

#define BPM 174              // Beats per minutes
#define PLAYER_DELTA 150     // Delta around a beat a player can hit (millisecs)
#define TIME 60000000 / BPM  // Time between beats (microsecs)

//////////////////
// Looking for Time Sync Protocol Code? Check src/libtempo in network(.cpp|.hpp)
//////////////////

int main(int argc, const char** argv) {

	tempo::Clock clock = tempo::Clock(sf::microseconds(TIME),
	                                  sf::milliseconds(PLAYER_DELTA));

	// Start up timeSyncThread
	std::thread timeSyncThread (tempo::timeSyncServer, &clock); 

	while (true) {
		if (clock.passed_beat()) {
			std::cout << "Server Beat Passed (" 
			          << clock.get_time().asSeconds() << ")" 
			          << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	return 0;
}
