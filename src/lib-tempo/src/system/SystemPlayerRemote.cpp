#include <tempo/system/SystemPlayerRemote.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>

namespace tempo{
	void SystemPlayerRemote::advanceBeat()
	{
		auto entities = getEntities();

		for(auto& entity : entities) {
			auto& comp = entity.getComponent<tempo::ComponentPlayerRemote>();
			comp.moved_this_beat = false;
		}
	}

	bool SystemPlayerRemote::update(int player_id, SystemID system_id)
	{
		tempo::Queue<sf::Packet> *queue = get_system_queue(SystemQID::PLAYER_UPDATES);
	
		if (queue->empty()) return false;

		while (!queue->empty()) {
			sf::Packet update = queue->front();
			queue->pop();
			
			int instance_id = 0;
			int dx = 0;
			int dy = 0;
			update >> instance_id >> dx >> dy;

			if (player_id == instance_id) continue;

			try
			{
				anax::Entity entity = system_id.get(instance_id);
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
   			catch(std::out_of_range& e)  
   			{  
				std::cerr << e.what() << std::endl;  
			}
		}

		return true;
	}
}