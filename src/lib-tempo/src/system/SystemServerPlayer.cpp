#include <tempo/system/SystemServerPlayer.hpp>

#include <tempo/network/queue.hpp>
#include <tempo/network/server.hpp>

namespace tempo
{

void SystemServerPlayer::update()
{
	// tempo::Queue<sf::Packet> *queue = get_system_queue(QueueID::PLAYER_UPDATES);

	// if (queue->empty()) return false;

	// while (!queue->empty()) {
	// 	sf::Packet update = queue->front();
	// 	sf::Packet update_broadcast;

	// 	queue->pop();
		
	// 	int instance_id = 0;
	// 	int dx = 0;
	// 	int dy = 0;
	// 	update >> instance_id >> dx >> dy;
	// 	update_broadcast << instance_id << dx << dy;

	// 	try {
	// 		anax::Entity entity = system_id->get(instance_id);
	// 		auto& motion = entity.getComponent<tempo::ComponentStageTranslation>();

	// 		motion.delta.x = dx;
	// 		motion.delta.y = dy;
			
	// 		// Send update to all clients 
	// 		for (auto it = clients.begin(); it != clients.end(); ++it) {
	// 			sendMessage(QueueID::PLAYER_UPDATES,
	// 			            update_broadcast,
	// 				    it->first);
	// 		}
	// 	} catch(std::out_of_range& e) {  
	// 		std::cerr << e.what() << std::endl;  
	// 	}

	// }

	// return true;
}

} // namespace tempo
