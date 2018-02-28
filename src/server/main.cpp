#define AM_SERVER

#include <tempo/time.hpp>

#include <tempo/entity/EntityCreationServer.hpp>
#include <tempo/system/SystemCombo.hpp>
#include <tempo/system/SystemGridAi.hpp>
#include <tempo/system/SystemHealth.hpp>
#include <tempo/system/SystemLevelManager.hpp>
#include <tempo/system/SystemServerPlayer.hpp>
#include <tempo/system/SystemAttack.hpp>

#include <tempo/network/base.hpp>
#include <tempo/network/server.hpp>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>

#include <anax/World.hpp>
#include <anax/Entity.hpp>

#include <iostream>
#include <cstdio>
#include <thread>
#include <vector>


#define BPM 120              // Beats per minutes
#define PLAYER_DELTA 125     // Delta around a beat a player can hit (millisecs)
#define TIME 60000000 / BPM  // Time between beats (microsecs)

int main(int argc, const char** argv) {

	tempo::Clock clock = tempo::Clock(sf::microseconds(TIME),
	                                  sf::milliseconds(PLAYER_DELTA));

	// Set up remote address, local ports and remote handshake port
	tempo::port_si = DEFAULT_PORT_IN;
	tempo::port_so = DEFAULT_PORT_OUT;
	tempo::port_st = DEFAULT_PORT_TS;

	//////////////////////////////////
	// Set up ECS
	anax::World world;
	
	// Create Systems
	tempo::SystemAttack       system_attack;
	tempo::SystemCombo        system_combo;
	tempo::SystemGridAi       system_grid_ai;
	tempo::SystemHealth       system_health;
	tempo::SystemServerPlayer system_player(clock);

	world.addSystem(system_attack);
	world.addSystem(system_combo);
	world.addSystem(system_grid_ai);
	world.addSystem(system_health);
	world.addSystem(system_player);
	world.refresh();

	// Create some Test Entities
	tempo::newAI(world, 5, 5);
	tempo::newAI(world, 3, 3);
	tempo::newAI(world, 8, 8);
	
	//Destroyables
	tempo::newDestroyable(world, 4, 4, "Cube");

	//NonDestroyables
	tempo::newNonDestroyable(world, 6, 6, "Cube");

	//////////////////////////////////
	// Thread Startup
	std::thread timeSyncThread (tempo::timeSyncServer, &clock);
	// Hack to allow printouts to line up a bit nicer :)
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	std::thread clientUpdatesThread (tempo::listenForClientUpdates);
	
	tempo::bindSocket('o', tempo::port_so);

	// sf::Clock dt_timer;
	// float last_dt_time = dt_timer.getElapsedTime().asSeconds();

	sf::Int64 tick = clock.get_time().asMicroseconds() / sf::Int64(TIME);
	tick++;
  
	// Main loop, with beat printouts
	while (true) {
		// Handshake call, DO NOT REMOVE
		tempo::checkForNewClients(&world);
		world.refresh();
		
		// float next_dt_time = dt_timer.getElapsedTime().asSeconds();
		// float dt = next_dt_time - last_dt_time;
		// last_dt_time = next_dt_time;
		
		if (clock.passed_delta_start()) {
			// std::cout << "Start" << std::endl;
		}

		if (clock.passed_beat()) {
			system_grid_ai.update();
			system_combo.advanceBeat();
      
			if (tick++ % 20 == 0)
				std::cout << "TICK (" << tick << ") " << clock.get_time().asMilliseconds() << "+++++++++++++++" << std::endl;
		}

		if (clock.passed_delta_end()) {
			// std::cout << "End" << std::endl;
			system_combo.advanceBeat();
		}

		system_attack.Broadcast(world);
		system_combo.checkForUpdates();
		system_health.CheckHealth();
		system_player.update(world);

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	return 0;
}
