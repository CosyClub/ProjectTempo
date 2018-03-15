#define AM_SERVER

#include <server/system/SystemAI.hpp>
#include <server/system/SystemAttack.hpp>
#include <server/system/SystemMovement.hpp>

#include <tempo/time.hpp>

#include <tempo/entity/EntityCreationServer.hpp>
#include <tempo/system/SystemCombo.hpp>
#include <tempo/system/SystemHealth.hpp>
#include <tempo/system/SystemTrigger.hpp>

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
	server::SystemAI system_ai;
	server::SystemAttack   system_attack(world);
	server::SystemMovement system_movement;
	tempo::SystemCombo  system_combo;
	tempo::SystemHealth system_health;
	tempo::SystemTrigger system_trigger(world);

	world.addSystem(system_ai);
	world.addSystem(system_attack);
	world.addSystem(system_movement);
	world.addSystem(system_combo);
	world.addSystem(system_health);
	world.addSystem(system_trigger);
	world.refresh();

	// Create some Test Entities
	  
	tempo::createMobStill(world, glm::ivec2(36, 42));
	tempo::createMobStill(world, glm::ivec2(40, 42));
	tempo::createMobStill(world, glm::ivec2(44, 42));
	
	tempo::createMobCreeper(world, glm::ivec2(40, 64));

	std::deque<glm::ivec2> path {glm::ivec2(64, 68),
	                             glm::ivec2(64, 72),
	                             glm::ivec2(68, 72),
	                             glm::ivec2(68, 68)};
	tempo::createMobPatroller(world, path[0], path);

	std::deque<glm::ivec2> path2 {glm::ivec2(11, 67),
	                              glm::ivec2(11, 73),
	                              glm::ivec2(15, 73),
	                              glm::ivec2(15, 67)};
	tempo::createMobPatroller(world, path2[0], path2);
	  
	// tempo::createMobCreeper(world, glm::ivec2(12, 12));
	// tempo::createMobCreeper(world, glm::ivec2(14, 14));
	// tempo::createMobAntiSnail(world, glm::ivec2(4, 4));

	std::vector<glm::ivec2> wall          = {{37,17},{38,17},{39,17},{40,17},{41,17},{42,17},{43,17},{44,17}, {45,17}};
	tempo::createButtonGroup(world, {{40, 12}}, wall);

	std::vector<glm::ivec2> wall1          = {{37,48},{38,48},{39,48},{40,48},{41,48},{42,48},{43,48},{44,48},};
	tempo::createButtonGroup(world, {{40, 43},{44,43},{36,43}}, wall1);

	std::vector<glm::ivec2> wall2          = {{36,62},{36,63},{36,64},{36,65},{36,66},{36,67},{36,68},
																						{50,62},{50,63},{50,64},{50,65},{50,66},{50,67},{50,68}};
	tempo::createButtonGroup(world, {{40, 65}}, wall2);

	std::vector<glm::ivec2> wall3          = {{37,69},{38,69},{39,69},{40,69},{41,69},{42,69},{43,69}};
	tempo::createButtonGroup(world, {{66,70},{13,69}}, wall3);

	std::vector<glm::ivec2> wall4          = {{40,132},{41,132},{42,132}};
	tempo::createButtonGroup(world, {{41,110},{26,128},{57,128}}, wall4);

	world.refresh();

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
			system_health.broadcastHealth();
		}

		if (clock.passed_antibeat())
		{
		}

		////////////////
		// Events at "Delta Start"
		if (clock.passed_delta_start()) {
			// std::cout << "Start" << std::endl;
		}

		////////////////
		// Events at "Beat Passed"
		if (clock.passed_beat()) {
			system_ai.update(system_attack);
			system_combo.advanceBeat();
			system_trigger.updateButtons(world);

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
