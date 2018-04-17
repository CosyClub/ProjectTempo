#define AM_SERVER

#include <server/system/SystemAI.hpp>
#include <server/system/SystemAttack.hpp>
#include <server/system/SystemCombo.hpp>
#include <server/system/SystemHeartbeat.hpp>
#include <server/system/SystemMovement.hpp>

#include <tempo/time.hpp>

#include <tempo/entity/EntityCreationServer.hpp>
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

void RythmButton(anax::World &          world,
				std::vector<std::vector<glm::ivec2>> positions,
				std::vector<glm::ivec2> tiles,
				std::vector<glm::ivec2> spikes,
				int &ID) {

	for (uint32_t i = 0; i < positions.size(); i++) {

		std::vector<glm::ivec2> group = positions[i];
		glm::ivec2 prev;
		glm::ivec2 next;

		bool triggerable = true;

		if (i == 0) {
			prev = {-1,-1};
		}
		else {
			prev = positions[i-1][0]; //first ivec2 in previous group
			triggerable = false;
		}

		if (i == positions.size() - 1) {
			next = {-1,-1};
		}
		else {
			next = positions[i + 1][0]; //first ivec2 in next group
		}

		tempo::createButtonGroup(world, group, tiles, spikes, prev, next, triggerable, ID);

		if (spikes.size() > 0) {
			tempo::createSpikes(world, spikes);
		}
	}

	ID++;
}

void newButton(anax::World &          world,
	std::vector<glm::ivec2> positions,
	std::vector<glm::ivec2> tiles,
	std::vector<glm::ivec2> spikes) {

	tempo::createButtonGroup(world, positions, tiles, spikes, { -1,-1 }, { -1,-1 }, true, 0);

	if (spikes.size() > 0) {
		tempo::createSpikes(world, spikes);
	}

}

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
	server::SystemAI        system_ai;
	server::SystemAttack    system_attack(world);
	server::SystemCombo     system_combo;
	server::SystemHeartbeat system_heatbeat;
	server::SystemMovement  system_movement;
	tempo::SystemHealth     system_health;
	tempo::SystemTrigger    system_trigger(world);

	world.addSystem(system_ai);
	world.addSystem(system_attack);
	world.addSystem(system_combo);
	world.addSystem(system_heatbeat);
	world.addSystem(system_movement);
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

	int rhythmID = 1;

	std::vector<glm::ivec2> wall          = {{37,17},{38,17},{39,17},{40,17},{41,17},{42,17},{43,17},{44,17}, {45,17}};
	RythmButton(world, { { { 40,12 }},{ { 40,13 } },{ { 41,13 } },{ { 41,12 } } }, wall, {}, rhythmID);

	std::vector<glm::ivec2> wall1          = {{37,48},{38,48},{39,48},{40,48},{41,48},{42,48},{43,48},{44,48},};
	newButton(world, { { 40, 43 } }, wall1, {});

	std::vector<glm::ivec2> wall2          = {{36,62},{36,63},{36,64},{36,65},{36,66},{36,67},{36,68},
																						{50,62},{50,63},{50,64},{50,65},{50,66},{50,67},{50,68}};
	newButton(world, { { 40, 65 } }, wall2, {});

	std::vector<glm::ivec2> wall3          = {{37,69},{38,69},{39,69},{40,69},{41,69},{42,69},{43,69}};
	newButton(world, { { 66,70 },{ 13,69 } }, wall3, { { 13,70 } });

	std::vector<glm::ivec2> wall4          = {{40,132},{41,132},{42,132}};
	newButton(world, { { 41,110 },{ 26,128 },{ 57,128 } }, wall4, {});

	std::vector<glm::ivec2> spikes		   = {{ 35,65 }};
	newButton(world, { { 35,67 } }, {}, spikes);

	spikes = {{ 34,65 }};
	newButton(world, { { 34,67 } }, {}, spikes);

	spikes = {{ 33,65 }};
	newButton(world, { { 33,67 } }, {}, spikes);

	spikes = {{ 32,65 }};
	newButton(world, { { 32,67 } }, {}, spikes);

	spikes = {{ 31,65 }};
	newButton(world, { { 31,67 } }, {}, spikes);

	spikes = {{ 30,65 }};
	newButton(world, { { 30,67 } }, {}, spikes);

	spikes = { { 29,65 } };
	newButton(world, { { 29,67 } }, {}, spikes);

	spikes = { { 28,65 } };
	newButton(world, { { 28,67 } }, {}, spikes);

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
			system_movement.receiveTranslations(world);
			system_attack.receiveAttacks(world);
			system_combo.checkForUpdates(world);
			system_health.CheckHealth();
			//system_health.server_receiveHealth(world);
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
			system_trigger.updateButtons(world);

			if (tick++ % 20 == 0)
				std::cout << "TICK (" << tick << ") "
				          << clock.get_time().asMilliseconds()
				          << std::endl;
		}

		////////////////
		// Events at "Delta End"
		if (clock.passed_delta_end()) {
			// std::cout << "End" << std::endl;
			system_heatbeat.checkForHeatbeats(world);
			system_combo.advanceBeat();
			system_attack.processAttacks();
			system_health.regenerate();
			system_movement.processTranslation();
		}

		////////////////
		// Events all the time
		{
			system_health.broadcastHealth();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	return 0;
}
