#include <tempo/system/SystemPlayerRemoteServer.hpp>

namespace tempo{
	void SystemPlayerRemoteServer::advanceBeat()
	{
		auto entities = getEntities();

		for(auto& entity : entities) {
			auto& comp = entity.getComponent<tempo::ComponentPlayerRemoteServer>();
			comp.moved_this_beat = false;
		}
	}

	bool SystemPlayerRemoteServer::update(SystemID system_id)
	{
		tempo::Queue<sf::Packet> *queue = get_system_queue(SystemQID::PLAYER_UPDATES);
	
		if (queue->empty()) return false;

		while (!queue->empty()) {

			
			sf::Packet update = queue->front();
			sf::Packet update_broadcast;

			queue->pop();
			
			int instance_id = 0;
			int dx = 0;
			int dy = 0;
			update >> instance_id >> dx >> dy;
			update_broadcast << instance_id << dx << dy;

			try
			{
				anax::Entity entity = system_id.get(instance_id);
				auto& input = entity.getComponent<tempo::ComponentPlayerRemoteServer>();
				auto& motion = entity.getComponent<tempo::ComponentGridMotion>();
				
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
   			catch(std::out_of_range& e)  
   			{  
				std::cerr << e.what() << std::endl;  
			}

		}

		return true;
	}
}
