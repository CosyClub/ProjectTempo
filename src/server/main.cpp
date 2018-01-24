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

//#include <tempo/entity/GridAi.hpp>

#include <tempo/network/base.hpp>
#include <tempo/network/server.hpp>

#include <tempo/entity/LevelManager.hpp>
#include <tempo/entity/GridAi.hpp>
#include <tempo/entity/Health.hpp>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>

#include <anax/World.hpp>
#include <anax/Entity.hpp>

#define BPM 174              // Beats per minutes
#define PLAYER_DELTA 150     // Delta around a beat a player can hit (millisecs)
#define TIME 60000000 / BPM  // Time between beats (microsecs)

int main(int argc, const char** argv) {

	tempo::Clock clock = tempo::Clock(sf::microseconds(TIME),
	                                  sf::milliseconds(PLAYER_DELTA));

	// Set up remote address, local ports and remote handshake port
	tempo::port_sh = DEFAULT_PORT_HS;
	tempo::port_si = DEFAULT_PORT_IN;
	tempo::port_so = DEFAULT_PORT_OUT;
	tempo::port_st = DEFAULT_PORT_TS;

	//////////////////////////////////
	// Set up ECS
	anax::World world;
	tempo::SystemLevelManager system_level(world,
	                                       "../bin/resources/levels/levelTest.bmp",
	                                       "../bin/resources/levels/zonesTest.bmp"
	                                       );
	tempo::SystemGridAi       system_grid_ai;
	tempo::SystemHealth       system_health;
	
	world.addSystem(system_level);
	world.addSystem(system_grid_ai);
	world.addSystem(system_health);
	world.refresh();

	// Create Systems
	/* tempo::SystemGridAi system_grid_ai; */

	//////////////////////////////////
	// Thread Startup
	std::thread timeSyncThread (tempo::timeSyncServer, &clock);
	std::thread newClientsThread (tempo::listenForNewClients);
	std::thread clientUpdatesThread (tempo::listenForClientUpdates);

	sf::Clock dt_timer;

	float last_dt_time = dt_timer.getElapsedTime().asSeconds();

	// Main loop, with beat printouts
	while (true) {
		if (clock.passed_beat()) {
			system_grid_ai.update();

			std::cout << "Server Beat Passed ("
			          << clock.get_time().asSeconds() << ")"
			          << std::endl;
		}

		float next_dt_time = dt_timer.getElapsedTime().asSeconds();
		float dt = next_dt_time - last_dt_time;
		last_dt_time = next_dt_time;

		world.refresh();
		system_level.update(dt);
		system_health.CheckHealth();

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	return 0;
}
