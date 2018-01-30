////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////

#include <tempo/entity/PlayerRemoteS.hpp>
#include <tempo/entity/ID.hpp>
#include <tempo/entity/SystemQID.hpp>
#include <tempo/network/queue.hpp>
#include <tempo/network/server.hpp>

#include <iostream>
#include <cstdio>

namespace tempo{
	void SystemPlayerRemoteS::advanceBeat()
	{
		auto entities = getEntities();

		for(auto& entity : entities) {
			auto& comp = entity.getComponent<tempo::ComponentPlayerRemoteS>();
			comp.moved_this_beat = false;
		}
	}

	bool SystemPlayerRemoteS::update()
	{
		tempo::Queue<sf::Packet> *queue = get_system_queue(SystemQID::PLAYER_UPDATES);
	
		if (queue->empty()) return false;

		while (!queue->empty()) {

			std::cout << "THINGS BE GOT\n";
			
			sf::Packet update = queue->front();
			sf::Packet update_broadcast;

			queue->pop();
			
			int instance_id = 0;
			int dx = 0;
			int dy = 0;
			update >> instance_id >> dx >> dy;
			update_broadcast << instance_id << dx << dy;

			// TODO This is horrifyingly bad and should be removed ASAP
			if (id_map.find(instance_id) == id_map.end()) {
				std::cout << "Entity " << instance_id << "tried "
				          << "to move, but we don't have entity for that.";
				continue;
			}

			anax::Entity entity = id_map.find(instance_id)->second;
			auto& input = entity.getComponent<tempo::ComponentPlayerRemoteS>();
			auto& motion = entity.getComponent<tempo::ComponentGridMotion>();
			// END of horrifyingly bad bit
			
			if (!input.moved_this_beat){
				input.moved_this_beat = true;
				motion.beginMovement(dx, dy);
			}

			if (!clock.within_delta()){
				std::cout << "Entity " << instance_id << " missed beat by " 
				          << std::min(clock.since_beat().asMilliseconds(), 
				                      clock.until_beat().asMilliseconds()) 
				          << std::endl;
			}

			for (auto it = clients.begin(); it != clients.end(); ++it) {
				sendMessage(SystemQID::PLAYER_UPDATES,
				            update_broadcast,
					    it->first);
			}

		}

		return true;
	}
}
