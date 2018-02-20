#include <tempo/system/SystemPlayer.hpp>

#include <tempo/network/base.hpp>
#include <tempo/network/QueueID.hpp>
#include <tempo/network/queue.hpp>

namespace tempo
{

void SystemPlayer::update(anax::Entity::Id player_id, anax::World &world)
{
	tempo::Queue<sf::Packet> *queue = get_system_queue(QueueID::PLAYER_UPDATES);

	if (queue->empty()) return;

	while (!queue->empty()) {
		sf::Packet update = queue->front();
		queue->pop();
		
		anax::Entity::Id instance_id;
		int dx = 0;
		int dy = 0;
		update >> instance_id >> dx >> dy;

		instance_id = servertolocal[instance_id];

		if (player_id == instance_id) continue;

		try {
			anax::Entity entity = anax::Entity(world, instance_id);
			auto& combo = entity.getComponent<tempo::ComponentCombo>();
			auto& motion = entity.getComponent<tempo::ComponentStageTranslation>();
			// END of horrifyingly bad bit
			
			combo.performAction();
			motion.delta.x = dx;
			motion.delta.y = dy;

		} catch(std::out_of_range& e) {  
			std::cerr << e.what() << std::endl;  
		}
	}
}

} //namespace tempo
