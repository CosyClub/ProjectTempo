////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////

#include <tempo/entity/PlayerRemote.hpp>
#include <tempo/entity/ID.hpp>
#include <tempo/entity/SystemQID.hpp>
#include <tempo/network/queue.hpp>

#include <iostream>
#include <cstdio>

namespace tempo{
	void SystemPlayerRemote::advanceBeat()
	{
		auto entities = getEntities();

		for(auto& entity : entities) {
			auto& comp = entity.getComponent<tempo::ComponentPlayerRemote>();
			comp.moved_this_beat = false;
		}
	}

	bool SystemPlayerRemote::update(int player_id)
	{
		tempo::Queue<sf::Packet> *queue = get_system_queue(SystemQID::PLAYER_UPDATES);
	
		if (queue->empty()) return false;

		while (!queue->empty()) {
			std::cout << "We got a thing\n";
			sf::Packet update = queue->front();
			queue->pop();
			
			int instance_id = 0;
			int dx = 0;
			int dy = 0;
			update >> instance_id >> dx >> dy;

			if (player_id == instance_id) continue;

			// TODO This is horrifyingly bad and should be removed ASAP
			if (id_map.find(instance_id) == id_map.end()) {
				std::cout << "Entity " << instance_id << "tried "
				          << "to move, but we don't have entity for that.\n";
				continue;
			}

			std::cout << "instance id is " << instance_id << std::endl;
			anax::Entity entity = id_map.find(instance_id)->second;
			auto& input = entity.getComponent<tempo::ComponentPlayerRemote>();
			auto& motion = entity.getComponent<tempo::ComponentGridMotion>();
			// END of horrifyingly bad bit
			
			if(!input.moved_this_beat){
				input.moved_this_beat = true;
				motion.beginMovement(dx, dy);
			}

			if(!clock.within_delta()){
				std::cout << "Entity " << instance_id << " missed beat by " 
				          << std::min(clock.since_beat().asMilliseconds(), 
				                      clock.until_beat().asMilliseconds()) 
				          << std::endl;
			}
		}

		return true;
	}
}
