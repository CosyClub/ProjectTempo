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

void newButton(anax::World &    world,
	std::vector<glm::ivec2> positions,
	std::vector<glm::ivec2> tiles,
	std::vector<glm::ivec2> spikes)
{
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

	// tempo::createMobCreeper(world, glm::ivec2(12, 12));
	// tempo::createMobCreeper(world, glm::ivec2(14, 14));
	// tempo::createMobAntiSnail(world, glm::ivec2(4, 4));

	int rhythmID = 1;

	//Needed so that the player cannot see other feeder areas
	int emptySpace = 40;

	int fheight = 69 + emptySpace;
	int feeder_areas = 10;


	std::vector<glm::ivec2> spikes = {{ 40 + (2 * fheight), 3 }};
	newButton(world, { { 40 + (2 * fheight), 2 } }, {}, spikes);

	for(int i = 0; i < feeder_areas; i++){

		// Create some Test Entities

		for(int j = 0; j < 10; j++){
		tempo::createMobCreeper(world, glm::ivec2(40 + (i* fheight), 40));
	}

		std::deque<glm::ivec2> path {glm::ivec2(64 + (i* fheight), 68),
		                             glm::ivec2(64 + (i* fheight), 72),
		                             glm::ivec2(68 + (i* fheight), 72),
		                             glm::ivec2(68 + (i* fheight), 68)};
		tempo::createMobPatroller(world, path[0], path);

		std::deque<glm::ivec2> path2 {glm::ivec2(11 + (i* fheight), 67),
		                              glm::ivec2(11 + (i* fheight), 73),
		                              glm::ivec2(15 + (i* fheight), 73),
		                              glm::ivec2(15 + (i* fheight), 67)};
		tempo::createMobPatroller(world, path2[0], path2);

		std::vector<glm::ivec2> wall          = {{37 + (i* fheight),17},{38 + (i* fheight),17},{39 + (i* fheight),17},
		                                         {40 + (i* fheight),17},{41 + (i* fheight),17},{42 + (i* fheight),17},
		                                         {43 + (i* fheight),17},{44 + (i* fheight),17}, {45 + (i* fheight),17}};

		RythmButton(world, { { { 40 + (i* fheight),12 }},{ { 40 + (i* fheight),13 } },{ { 41 + (i* fheight),13 } },{ { 41 + (i* fheight),12 } } }, wall, {}, rhythmID);
	//
	// std::vector<glm::ivec2> wall1          = {{37,48},{38,48},{39,48},{40,48},{41,48},{42,48},{43,48},{44,48},};
	// newButton(world, { { 40, 43 } }, wall1, {});
	//
	// std::vector<glm::ivec2> wall2          = {{36,62},{36,63},{36,64},{36,65},{36,66},{36,67},{36,68},
	// 																					{50,62},{50,63},{50,64},{50,65},{50,66},{50,67},{50,68}};
	// newButton(world, { { 40, 65 } }, wall2, {});
	//
	// std::vector<glm::ivec2> wall3          = {{37,69},{38,69},{39,69},{40,69},{41,69},{42,69},{43,69}};
	// newButton(world, { { 66,70 },{ 13,69 } }, wall3, { { 13,70 } });
	//
	// std::vector<glm::ivec2> wall4          = {{40,132},{41,132},{42,132}};
	// newButton(world, { { 41,110 },{ 26,128 },{ 57,128 } }, wall4, {});
	//
	// std::vector<glm::ivec2> spikes		   = {{ 35,65 }};
	// newButton(world, { { 35,63 },{ 35,67 } }, {}, spikes);
	//
	// spikes = {{ 34,65 }};
	// newButton(world, { { 34,63 },{ 34,67 } }, {}, spikes);
	//
	// spikes = {{ 33,65 }};
	// newButton(world, { { 33,63 },{ 33,67 } }, {}, spikes);
	//
	// spikes = {{ 32,65 }};
	// newButton(world, { { 32,63 },{ 32,67 } }, {}, spikes);
	//
	// spikes = {{ 31,65 }};
	// newButton(world, { { 31,63 },{ 31,67 } }, {}, spikes);
	//
	// spikes = {{ 30,65 }};
	// newButton(world, { { 30,63 },{ 30,67 } }, {}, spikes);
	//
	// spikes = { { 29,65 } };
	// newButton(world, { { 29,63 },{ 29,67 } }, {}, spikes);
	//
	// spikes = { { 28,65 } };
	// newButton(world, { { 28,63 },{ 28,67 } }, {}, spikes);

}
	world.refresh();

	//////////////////////////////////
	// Thread Startup
	std::thread timeSyncThread(tempo::timeSyncServer, &clock);
	// Hack to allow printouts to line up a bit nicer :)
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	std::thread clientUpdatesThread(tempo::listenForClientUpdates);
	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	tempo::bindSocket('o', tempo::port_so);

	std::cout << "Server now runnning on: "
	          << sf::IpAddress::getLocalAddress().toString() << ":"
	          << tempo::port_si << std::endl;

	sf::Int64 tick = clock.get_time().asMicroseconds() / sf::Int64(TIME);
	tick++;
	sf::Int64 tick_time_s = clock.get_time().asMicroseconds();
	sf::Int64 tick_time_e = 0;

	// Main loop, with beat printouts
	while (true) {
		tick_time_s = clock.get_time().asMicroseconds();

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
			system_health.check_health();
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

		// Sleep for for some time, making the game loop take 20ms
		// Time spend doing things (in microseconds) is:
		// tick_time_e - tick_time_s
		tick_time_e = clock.get_time().asMicroseconds();
		std::this_thread::sleep_for(std::chrono::microseconds(20000 - (tick_time_e - tick_time_s)));
	}
	return 0;
}
