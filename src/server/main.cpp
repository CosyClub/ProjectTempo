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
	tempo::Song mainsong("resources/sound/ravecave_loop_clicktrack.ogg");
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
	// int emptySpace = 40;
	//
	// int fheight = 69 + emptySpace;
	int feeder_areas = 1;

	for(int i = 0; i < feeder_areas; i++){

		// std::vector<glm::ivec2> wall          = {{44,16},{45,16},{46,16},{47,16},{48,16},{49,16},{50,16},{51,16},{52,16}};
		// newButton(world, { { 45,10 } }, wall, {});
		//
		// std::vector<glm::ivec2> wall1          = {{39,22},{39,23},{39,24},{39,25},{39,26}};
		// newButton(world, { { 43,21 }, { 43,25 }, { 43,29 } }, wall1, {});
		//
		// std::vector<glm::ivec2> wall2          = {{30,33},{29,33},{28,33},{27,33},{26,33}};
		// newButton(world, { { 26,27 }, { 31, 27 } }, wall2, {});
		//
		// tempo::createMobStill(world, glm::ivec2(31, 26));
		// tempo::createMobStill(world, glm::ivec2(26, 26));
		//
		// std::vector<glm::ivec2> wall3          = {{26,45},{27,45},{28,45},{29,45},{30,45},{31,45},{32,45}};
		// RythmButton(world, { { { 28,37 }},{ { 28,38 } },{ { 29,38 } },{ { 29,37 } } }, wall3, {}, rhythmID);
		//
		// std::deque<glm::ivec2> path {glm::ivec2(26, 37),
		// 														 glm::ivec2(26, 38),
		// 														 glm::ivec2(26, 39),
		// 														 glm::ivec2(26, 40),
		// 														 glm::ivec2(27, 40),
		// 														 glm::ivec2(28, 40),
		// 														 glm::ivec2(29, 40),
		// 														 glm::ivec2(30, 40),
		// 														 glm::ivec2(31, 40),
		// 														 glm::ivec2(31, 39),
		// 														 glm::ivec2(31, 38),
		// 														 glm::ivec2(31, 37),
		// 														 glm::ivec2(31, 36),
		// 														 glm::ivec2(31, 35),
		// 														 glm::ivec2(30, 35),
		// 														 glm::ivec2(29, 35),
		// 														 glm::ivec2(28, 35),
		// 														 glm::ivec2(27, 35),
		// 														 glm::ivec2(26, 36)
		// 													 };
		// tempo::createMobPatroller(world, path[0], path);
		//
		// std::vector<glm::ivec2> spikes		   = {{ 23,59 },{ 24,59 },{ 25,59 },{ 26,59 },{ 27,59 }};
		// newButton(world, { { 28,55 } }, {}, spikes);
		//
		// std::vector<glm::ivec2> spikes1		   = {{ 28,59 },{ 29,59 },{ 30,59 },{ 31,59 },{ 32,59 }};
		// newButton(world, { { 28,63 } }, {}, spikes1);
		//
		// std::vector<glm::ivec2> wall4          = {{25,72},{26,72},{27,72},{28,72},{29,72},{30,72},
		//                                           {46,33},{47,33},{48,33},{49,33},{50,33}, {51,33}, {52,33}};
		// RythmButton(world, { { { 25,68 }, { 29,68 }},{ { 25,69 }, { 29,69 } },{ { 26,69 }, { 30,69 } },{ { 26,68 }, { 30,68 } } }, wall4, {}, rhythmID);
		//
		// std::vector<glm::ivec2> wall5          = {{36,79},{36,80},{36,81},{36,82},{36,83},{36,84},{36,85}, {36,86}, {36,87},
		//                                           {46,74},{47,74},{48,74},{49,74},{50,74}, {51,74}, {52,74}};
		// newButton(world, { {33,79 },{ 33,82 },{ 33,85 }}, wall5, {});
		//
		// tempo::createMobCreeper(world, glm::ivec2(23, 89));
		// tempo::createMobCreeper(world, glm::ivec2(23, 83));
		// tempo::createMobCreeper(world, glm::ivec2(23, 83));
		// tempo::createMobCreeper(world, glm::ivec2(23, 83));
		// tempo::createMobCreeper(world, glm::ivec2(23, 83));
		//
		// tempo::createMobStill(world, glm::ivec2(28,78));
		// tempo::createMobStill(world, glm::ivec2(28,75));
		// tempo::createMobStill(world, glm::ivec2(28,81));
		// tempo::createMobStill(world, glm::ivec2(28,84));
		// tempo::createMobStill(world, glm::ivec2(28,87));
		//
		// std::vector<glm::ivec2> spikes2		   = {{ 37,83 },{ 38,83 },{ 39,83 },{ 40,83 },{ 41,83 },{ 42,83 }};
		// newButton(world, { { 42,77 }, {46,77} }, {}, spikes2);
		//
		// std::vector<glm::ivec2> spikes3		   = {{ 47,93 },{ 48,93 },{ 49,93 },{ 50,93 },{ 51,93 },{ 52,93 }};
		// newButton(world, { { 39,98 } }, {}, spikes3);
		//
		// tempo::createMobCreeper(world, glm::ivec2(45, 130));
		// tempo::createMobCreeper(world, glm::ivec2(45, 130));
		// tempo::createMobCreeper(world, glm::ivec2(45, 130));
		// tempo::createMobCreeper(world, glm::ivec2(45, 130));
		// tempo::createMobCreeper(world, glm::ivec2(45, 130));
		// tempo::createMobCreeper(world, glm::ivec2(45, 130));
		//
		// std::vector<glm::ivec2> wall6          = {{37,102},{38,102},{39,102},{40,102},{41,102},{42,102},
		//                                           {41,122},{42,122},{43,122},{44,122},{45,122}, {46,122}, {47,122}};
		// RythmButton(world, { { { 41,116 }, { 45,116 }},{ { 41,117 }, { 45,117 } },{ { 42,117 }, { 46,117 } },{ { 42,116 }, { 46,116 } } }, wall6, {}, rhythmID);
		//
		// std::vector<glm::ivec2> wall7          = {{26,138},{27,138},{28,138},{29,138},{30,138},{31,138},{32,138}};
		// newButton(world, { { 26,27 }, { 31, 27 } }, wall7, {});

	//Starter areas

	std::vector<glm::ivec2> wall          = {{10,5}};
	RythmButton(world, { { { 10,2 }},{ { 10,3 } },{ {10,4} } }, wall, {}, rhythmID);

	std::vector<glm::ivec2> wall1          = {{8,5}};
	RythmButton(world, { { { 8,2 }},{ { 8,3 } },{ {8,4} } }, wall1, {}, rhythmID);

	std::vector<glm::ivec2> wall2          = {{10,5}};
	RythmButton(world, { { { 6,2 }},{ { 6,3 } },{ { 6 , 4} } }, wall2, {}, rhythmID);

	//Room 1
	std::vector<glm::ivec2> wall3		   = {{ 7,11 },{ 8,11 },{ 9,11 }};
	newButton(world, { { 8,9 }, }, wall3, {});

	//Room 2

	//Room 3
	std::vector<glm::ivec2> wall4		   = {{ 24,11 },{ 25,11 },{ 26,11 }};
	newButton(world, { { 24,7 },{26,7} }, wall4, {});

	//Room 4
	std::vector<glm::ivec2> spikes		   = {{14,22},{14,23},{14,24},{14,25},{15,22},{15,26},{15,27},{16,22},{16,24},{17,22},{17,25},{17,26},{17,27},{17,28},{18,22},{18,23}};
	tempo::createSpikes(world, spikes);

	std::vector<glm::ivec2> spikes2		   = {{14,26},{14,27},{14,28}, {15,23},{15,24},{15,25},{15,28},{16,23},{16,25},{16,26},{16,27},{16,28}, {17,23}, {17,24},{18,24},{18,25},{18,26},{18,27},{18,28}};
	newButton(world, { { 13,25 }, }, {}, spikes2);
	newButton(world, { { 18,29 }, }, {}, spikes2);

	// Room 5 (Prongs)
	newButton(world, { {17,30} }, {}, {{17,32}});
	newButton(world, { {16,34} }, {}, {{16,32}});
	newButton(world, { {15,34},{15,30} }, {}, {{15,32}});
	newButton(world, { {13,34},{13,30} }, {}, {{13,32}});
	newButton(world, { {11,34},{11,30} }, {}, {{11,32}});
	newButton(world, { {10,34},{10,30} }, {}, {{10,32}});
	newButton(world, { {9,34},{9,30} }, {}, {{9,32}});

	newButton(world, { {7,32} }, {{7,31},{7,33}}, {});

	// Room 6
	std::vector<glm::ivec2> wall5          = {{2,37},{3,37},{4,37}};
	RythmButton(world, { { {4,31}},{ { 3,31 } },{ { 2,31} },{{2,32}},{{3,32}} }, wall5, {}, rhythmID);

	std::vector<glm::ivec2> wall6          = {{2,38},{3,38},{4,38}};
	RythmButton(world, { { {2,35}},{ { 3,35 } },{ { 4,35 }},{{4,34}},{{3,34}} }, wall6, {}, rhythmID);

	// Room 7 (Maze)
	{
	std::vector<glm::ivec2> spikes		   = {{1,40},{2,40},{3,40},{4,40},{5,40},{8,39},{8,40},{2,42},{2,43},{2,44},{2,45},{4,44},{5,43},{5,44},{5,42},{6,42},{7,42},{8,42},{7,44},{7,45},{8,44}};
	tempo::createSpikes(world, spikes);

	glm::ivec2 spike1		   = {4,42};
	glm::ivec2 spike7		   = {6,40};
	glm::ivec2 spike2		   = {7,40};
	glm::ivec2 spike3		   = {8,41};
	glm::ivec2 spike4		   = {8,43};
	glm::ivec2 spike5		   = {9,42};
	glm::ivec2 spike6		   = {9,47};

	newButton(world, { {1,39} }, {}, {spike2});
	newButton(world, { {1,45} }, {}, {spike1});
	newButton(world, { {7,43} }, {}, {spike7,spike3});
	newButton(world, { {9,39} }, {}, {spike4});
	newButton(world, { {9,45} }, {}, {{3,42}, spike6});
	newButton(world, { {9,48} }, {}, {spike5 ,{8,45},{8,46}, {8,47}});
}

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
