#define AM_SERVER

#include <server/system/SystemAI.hpp>
#include <server/system/SystemAttack.hpp>
#include <server/system/SystemCombo.hpp>
#include <server/system/SystemHeartbeat.hpp>
#include <server/system/SystemMovement.hpp>

#include <tempo/time.hpp>

#include <tempo/entity/EntityCreationServer.hpp>
#include <tempo/system/SystemHealth.hpp>
#include <tempo/system/SystemPlayer.hpp>
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
				std::vector<std::vector<glm::ivec2>>  positions,
				std::vector<glm::ivec2>               tiles,
				std::vector<glm::ivec2>               spikes,
				glm::ivec2                            respawn_pos,
				int                                  &ID) {

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

		tempo::createButtonGroup(world, group, tiles, spikes, respawn_pos, prev, next, triggerable, ID);

		if (spikes.size() > 0) {
			tempo::createSpikes(world, spikes);
		}
	}

	ID++;
}

void newButton(anax::World &    world,
	std::vector<glm::ivec2> positions,
	std::vector<glm::ivec2> tiles,
	std::vector<glm::ivec2> spikes,
	glm::ivec2              respawn_pos)
{
	tempo::createButtonGroup(world, positions, tiles, spikes, respawn_pos, { -1,-1 }, { -1,-1 }, true, 0);

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
	tempo::SystemPlayer     system_player;
	tempo::SystemTrigger    system_trigger(world);

	world.addSystem(system_ai);
	world.addSystem(system_attack);
	world.addSystem(system_combo);
	world.addSystem(system_heatbeat);
	world.addSystem(system_movement);
	world.addSystem(system_health);
	world.addSystem(system_player);
	world.addSystem(system_trigger);
	world.refresh();

	// tempo::createMobCreeper(world, glm::ivec2(12, 12));
	// tempo::createMobCreeper(world, glm::ivec2(14, 14));
	// tempo::createMobAntiSnail(world, glm::ivec2(4, 4));

	int rhythmID = 1;

	// Needed so that the player cannot see other feeder areas
	int emptySpace = 40;
	int fheight = 10 + emptySpace;
	int feeder_areas = 10;

	for (int i = 0; i < feeder_areas; i++) {
		// Starter areas
		glm::ivec2 spawn_loc = glm::ivec2(10 + (i * fheight), 0);
		{
			std::vector<glm::ivec2> wall = {{10+ (i * fheight), 5}};
			RythmButton(world, {{{10+ (i * fheight), 1}}, {{10+ (i * fheight), 2}}, {{10+ (i * fheight), 3}}}, wall, {}, spawn_loc, rhythmID);

			std::vector<glm::ivec2> wall1 = {{8+ (i * fheight), 5}};
			RythmButton(world, {{{8+ (i * fheight), 1}}, {{8+ (i * fheight), 2}}, {{8+ (i * fheight), 3}}}, wall1, {}, spawn_loc, rhythmID);

			std::vector<glm::ivec2> wall2 = {{10+ (i * fheight), 5}};
			RythmButton(world, {{{6+ (i * fheight), 1}}, {{6+ (i * fheight), 2}}, {{6+ (i * fheight), 3}}}, wall2, {}, spawn_loc, rhythmID);

			std::vector<glm::ivec2> wall3 = {{5+ (i * fheight), 6}};
			RythmButton(world, {{{1+ (i * fheight), 6}}, {{2+ (i * fheight), 6}}, {{3+ (i * fheight), 6}}}, wall3, {}, spawn_loc, rhythmID);

			std::vector<glm::ivec2> wall4 = {{5+ (i * fheight), 8}};
			RythmButton(world, {{{1+ (i * fheight), 8}}, {{2+ (i * fheight), 8}}, {{3+ (i * fheight), 8}}}, wall4, {}, spawn_loc, rhythmID);

			std::vector<glm::ivec2> wall5 = {{5+ (i * fheight), 10}};
			RythmButton(world, {{{1+ (i * fheight), 10}}, {{2+ (i * fheight), 10}}, {{3+ (i * fheight), 10}}}, wall5, {}, spawn_loc, rhythmID);
		}

		// Room 1
		spawn_loc = glm::ivec2(8 + (i * fheight), 12);
		{
			std::vector<glm::ivec2> wall = {{7+ (i * fheight), 11}, {8+ (i * fheight), 11}, {9+ (i * fheight), 11}};
			newButton(world, {{8+ (i * fheight), 9}}, wall, {}, spawn_loc);
		}

		// Room 2
		spawn_loc = glm::ivec2(8 + (i * fheight), 12);
		{
			tempo::createMobStill(world, glm::ivec2(7+ (i * fheight), 13));
			tempo::createMobStill(world, glm::ivec2(7+ (i * fheight), 20));

			std::deque<glm::ivec2> path = {glm::ivec2(9+ (i * fheight), 13), glm::ivec2(9+ (i * fheight), 14), glm::ivec2(9+ (i * fheight), 15),
			                               glm::ivec2(9+ (i * fheight), 16), glm::ivec2(9+ (i * fheight), 17), glm::ivec2(9+ (i * fheight), 18),
			                               glm::ivec2(9+ (i * fheight), 19), glm::ivec2(9+ (i * fheight), 20)};
			tempo::createMobPatroller(world, path[0], path);
		}

		// Room 3
		{
			std::vector<glm::ivec2> wall = {{11+ (i * fheight), 24}, {11+ (i * fheight), 25}, {11+ (i * fheight), 26}};
			newButton(world, {{7+ (i * fheight), 24}, {7+ (i * fheight), 26}}, wall, {}, spawn_loc);
		}


		// Room 4
		{
			std::vector<glm::ivec2> spikes = {
			  {14+ (i * fheight), 22}, {14+ (i * fheight), 23}, {14+ (i * fheight), 24}, {14+ (i * fheight), 25}, {15+ (i * fheight), 22}, {15+ (i * fheight), 26}, {15+ (i * fheight), 27}, {16+ (i * fheight), 22},
			  {16+ (i * fheight), 24}, {17+ (i * fheight), 22}, {17+ (i * fheight), 25}, {17+ (i * fheight), 26}, {17+ (i * fheight), 27}, {17+ (i * fheight), 28}, {18+ (i * fheight), 22}, {18+ (i * fheight), 23}};
			tempo::createSpikes(world, spikes);

			std::vector<glm::ivec2> spikes2 = {{14+ (i * fheight), 26}, {14+ (i * fheight), 27}, {14+ (i * fheight), 28}, {15+ (i * fheight), 23}, {15+ (i * fheight), 24},
			                                   {15+ (i * fheight), 25}, {15+ (i * fheight), 28}, {16+ (i * fheight), 23}, {16+ (i * fheight), 25}, {16+ (i * fheight), 26},
			                                   {16+ (i * fheight), 27}, {16+ (i * fheight), 28}, {17+ (i * fheight), 23}, {17+ (i * fheight), 24}, {18+ (i * fheight), 24},
			                                   {18+ (i * fheight), 25}, {18+ (i * fheight), 26}, {18+ (i * fheight), 27}, {18+ (i * fheight), 28}};
			newButton(world, {{13+ (i * fheight), 25}}, {}, spikes2, spawn_loc);
			newButton(world, {{18+ (i * fheight), 29}}, {}, spikes2, spawn_loc);

			tempo::createMobCreeper(world, glm::ivec2(15+ (i * fheight), 23));
			tempo::createMobCreeper(world, glm::ivec2(15+ (i * fheight), 27));
			tempo::createMobCreeper(world, glm::ivec2(16+ (i * fheight), 25));
			tempo::createMobCreeper(world, glm::ivec2(17+ (i * fheight), 23));
			tempo::createMobCreeper(world, glm::ivec2(17+ (i * fheight), 27));
		}

		// Room 5 (Prongs)
		spawn_loc = glm::ivec2(18 + (i * fheight), 30);
		{
			newButton(world, {{17+ (i * fheight), 30}}, {}, {{17+ (i * fheight), 32}}, spawn_loc);
			newButton(world, {{16+ (i * fheight), 34}}, {}, {{16+ (i * fheight), 32}}, spawn_loc);
			newButton(world, {{15+ (i * fheight), 34}, {15+ (i * fheight), 30}}, {}, {{15+ (i * fheight), 32}}, spawn_loc);
			newButton(world, {{13+ (i * fheight), 34}, {13+ (i * fheight), 30}}, {}, {{13+ (i * fheight), 32}}, spawn_loc);
			newButton(world, {{11+ (i * fheight), 34}, {11+ (i * fheight), 30}}, {}, {{11+ (i * fheight), 32}}, spawn_loc);
			newButton(world, {{10+ (i * fheight), 34}, {10+ (i * fheight), 30}}, {}, {{10+ (i * fheight), 32}}, spawn_loc);
			newButton(world, {{9+ (i * fheight), 34}, {9+ (i * fheight), 30}}, {}, {{9+ (i * fheight), 32}}, spawn_loc);

			newButton(world, {{7+ (i * fheight), 32}}, {{7+ (i * fheight), 31}, {7+ (i * fheight), 33}}, {}, spawn_loc);
		}


		// Room 6
		{
			std::vector<glm::ivec2> wall = {{2+ (i * fheight), 37}, {3+ (i * fheight), 37}, {4+ (i * fheight), 37}};
			RythmButton(world, {{{4+ (i * fheight), 31}}, {{3+ (i * fheight), 31}}, {{2+ (i * fheight), 31}}, {{2+ (i * fheight), 32}}, {{3+ (i * fheight), 32}}}, wall, {}, spawn_loc,
			            rhythmID);

			std::vector<glm::ivec2> wall1 = {{2+ (i * fheight), 38}, {3+ (i * fheight), 38}, {4+ (i * fheight), 38}};
			RythmButton(world, {{{2+ (i * fheight), 35}}, {{3+ (i * fheight), 35}}, {{4+ (i * fheight), 35}}, {{4+ (i * fheight), 34}}, {{3+ (i * fheight), 34}}}, wall1, {}, spawn_loc,
			            rhythmID);

			tempo::createMobStill(world, glm::ivec2(5+ (i * fheight), 31));
			tempo::createMobStill(world, glm::ivec2(5+ (i * fheight), 33));

			std::deque<glm::ivec2> path = {glm::ivec2(4+ (i * fheight), 31), glm::ivec2(3+ (i * fheight), 31), glm::ivec2(2+ (i * fheight), 31),
			                               glm::ivec2(3+ (i * fheight), 32), glm::ivec2(3+ (i * fheight), 32), glm::ivec2(4+ (i * fheight), 32)};
			tempo::createMobPatroller(world, path[0], path);

			std::deque<glm::ivec2> path1 = {glm::ivec2(2+ (i * fheight), 35), glm::ivec2(3+ (i * fheight), 35), glm::ivec2(4+ (i * fheight), 35),
			                               glm::ivec2(4+ (i * fheight), 34), glm::ivec2(3+ (i * fheight), 34), glm::ivec2(4+ (i * fheight), 34)};
			tempo::createMobPatroller(world, path1[0], path1);
		}


		// Room 7 (Maze)
		spawn_loc = glm::ivec2(3 + (i * fheight), 31);
		{
			std::vector<glm::ivec2> spikes = {{1+ (i * fheight), 40}, {2+ (i * fheight), 40}, {3+ (i * fheight), 40}, {4+ (i * fheight), 40}, {5+ (i * fheight), 40}, {8+ (i * fheight), 39},
			                                  {8+ (i * fheight), 40}, {2+ (i * fheight), 42}, {2+ (i * fheight), 43}, {2+ (i * fheight), 44}, {2+ (i * fheight), 45}, {4+ (i * fheight), 44},
			                                  {5+ (i * fheight), 43}, {5+ (i * fheight), 44}, {5+ (i * fheight), 42}, {6+ (i * fheight), 42}, {7+ (i * fheight), 42}, {8+ (i * fheight), 42},
			                                  {7+ (i * fheight), 44}, {7+ (i * fheight), 45}, {8+ (i * fheight), 44}};
			tempo::createSpikes(world, spikes);

			glm::ivec2 spike1 = {4+ (i * fheight), 42};
			glm::ivec2 spike7 = {6+ (i * fheight), 40};
			glm::ivec2 spike2 = {7+ (i * fheight), 40};
			glm::ivec2 spike3 = {8+ (i * fheight), 41};
			glm::ivec2 spike4 = {8+ (i * fheight), 43};
			glm::ivec2 spike5 = {9+ (i * fheight), 42};
			glm::ivec2 spike6 = {9+ (i * fheight), 47};

			newButton(world, {{1+ (i * fheight), 39}}, {}, {spike2}, spawn_loc);
			newButton(world, {{1+ (i * fheight), 45}}, {}, {spike1}, spawn_loc);
			newButton(world, {{7+ (i * fheight), 43}}, {}, {spike7, spike3}, spawn_loc);
			newButton(world, {{9+ (i * fheight), 39}}, {}, {spike4}, spawn_loc);
			newButton(world, {{9+ (i * fheight), 44}}, {}, {{3+ (i * fheight), 42}, spike6}, spawn_loc);
			newButton(world, {{9+ (i * fheight), 48}}, {}, {spike5, {8+ (i * fheight), 45}, {8+ (i * fheight), 46}, {8+ (i * fheight), 47}}, spawn_loc);

			tempo::createMobCreeper(world, glm::ivec2(5+ (i * fheight), 42));
			tempo::createMobCreeper(world, glm::ivec2(5+ (i * fheight), 42));
			tempo::createMobCreeper(world, glm::ivec2(5+ (i * fheight), 42));
			tempo::createMobCreeper(world, glm::ivec2(5+ (i * fheight), 42));
			tempo::createMobCreeper(world, glm::ivec2(5+ (i * fheight), 42));
			tempo::createMobCreeper(world, glm::ivec2(5+ (i * fheight), 42));
			tempo::createMobCreeper(world, glm::ivec2(5+ (i * fheight), 42));
		}

		// Room 8
		{
			std::vector<glm::ivec2> wall = {{12+ (i * fheight), 44}, {12+ (i * fheight), 43}, {12+ (i * fheight), 42},
			                                {13+ (i * fheight), 44}, {13+ (i * fheight), 43}, {13+ (i * fheight), 42}};
			RythmButton(world,
			            {{{12+ (i * fheight), 47}, {12+ (i * fheight), 49}},
			             {{13+ (i * fheight), 47}, {13+ (i * fheight), 49}},
			             {{13+ (i * fheight), 46}, {13+ (i * fheight), 50}},
			             {{12+ (i * fheight), 46}, {12+ (i * fheight), 50}}},
			            wall, {}, spawn_loc, rhythmID);
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
			system_ai.update(world, system_player, system_attack);
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
			system_player.TeleportPlayers();
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
