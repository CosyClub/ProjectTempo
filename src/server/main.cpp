#define AM_SERVER

#include <server/system/SystemAttack.hpp>
#include <server/system/SystemMovement.hpp>

#include <tempo/time.hpp>

#include <tempo/entity/EntityCreationServer.hpp>
#include <tempo/system/SystemCombo.hpp>
#include <tempo/system/SystemGridAi.hpp>
#include <tempo/system/SystemHealth.hpp>

#include <tempo/network/base.hpp>
#include <tempo/network/server.hpp>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>

#include <anax/Entity.hpp>
#include <anax/World.hpp>

#include <cstdio>
#include <iostream>
#include <thread>
#include <vector>

#define BPM 138                // Beats per minutes
#define PHASE 0                // Microseconds
#define PLAYER_DELTA 100       // Delta around a beat a player can hit (millisecs)
#define TIME 60000000.f / BPM  // Time between beats (microsecs)

int main(int argc, const char **argv)
{
	tempo::Song mainsong("resources/sound/ravecave_loop.ogg");
	mainsong.set_volume(0.f);
	mainsong.skip(sf::microseconds(PHASE));
	mainsong.set_volume(20.f);
	tempo::Clock clock = tempo::Clock(sf::microseconds(TIME), sf::milliseconds(PLAYER_DELTA));
	mainsong.start();


	// Set up remote address, local ports and remote handshake port
	tempo::port_si = DEFAULT_PORT_IN;
	tempo::port_so = DEFAULT_PORT_OUT;
	tempo::port_st = DEFAULT_PORT_TS;

	//////////////////////////////////
	// Set up ECS
	anax::World world;

	// Create Systems
	server::SystemAttack   system_attack(world);
	server::SystemMovement system_movement;
	tempo::SystemCombo  system_combo;
	tempo::SystemGridAi system_grid_ai;
	tempo::SystemHealth system_health;

	world.addSystem(system_attack);
	world.addSystem(system_movement);
	world.addSystem(system_combo);
	world.addSystem(system_grid_ai);
	world.addSystem(system_health);
	world.refresh();

	// Create some Test Entities
	// tempo::newAI(world, 5, 5);
	// tempo::newAI(world, 3, 3);
	// tempo::newAI(world, 8, 8);

	// Destroyables
	tempo::newDestroyable(world, 4, 4, "Cube");

	// NonDestroyables
	tempo::newNonDestroyable(world, 6, 6, "Cube");

	//////////////////////////////////
	// Thread Startup
	std::thread timeSyncThread(tempo::timeSyncServer, &clock);
	// Hack to allow printouts to line up a bit nicer :)
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	std::thread clientUpdatesThread(tempo::listenForClientUpdates);

	tempo::bindSocket('o', tempo::port_so);

	// sf::Clock dt_timer;
	// float last_dt_time = dt_timer.getElapsedTime().asSeconds();

	sf::Int64 tick = clock.get_time().asMicroseconds() / sf::Int64(TIME);
	tick++;

	// Main loop, with beat printouts
	while (true) {
		// Handshake call, DO NOT REMOVE
		tempo::checkForClientCreation(&world);
		tempo::checkForClientDeletion(world);

		// float next_dt_time = dt_timer.getElapsedTime().asSeconds();
		// float dt = next_dt_time - last_dt_time;
		// last_dt_time = next_dt_time;

		////////////////
		// Events all the time
		{
			system_movement.recieveTranslations(world);
			system_attack.recieveAttacks(world);
			system_combo.checkForUpdates();
			system_health.CheckHealth();
		}

		////////////////
		// Events at "Delta Start"
		if (clock.passed_delta_start()) {
			// std::cout << "Start" << std::endl;
		}

		////////////////
		// Events at "Beat Passed"
		if (clock.passed_beat()) {
			system_grid_ai.update();
			system_combo.advanceBeat();

			if (tick++ % 20 == 0)
				std::cout << "TICK (" << tick << ") " << clock.get_time().asMilliseconds()
				          << "+++++++++++++++" << std::endl;
		}

		////////////////
		// Events at "Delta End"
		if (clock.passed_delta_end()) {
			// std::cout << "End" << std::endl;
			system_combo.advanceBeat();
			system_attack.processAttacks();
			system_movement.processTranslation();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	return 0;
}
