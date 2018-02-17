#include <tempo/system/SystemCombo.hpp>

#include <tempo/network/QueueID.hpp>
#include <tempo/network/queue.hpp>

namespace tempo
{

void SystemCombo::advanceBeat()
{
	auto entities = getEntities();

	for (auto& entity : entities) {
		auto& combo = entity.getComponent<tempo::ComponentCombo>();
		combo.advanceBeat();
	}
}

bool SystemCombo::checkForUpdates()
{
	tempo::Queue<sf::Packet> *queue = get_system_queue(QueueID::COMBO);
	if (queue->empty()) return false;

	while (!queue->empty()) {
		sf::Packet update = queue->front();
		queue->pop();
	
		int     id;
		uint8_t message;
		update >> id >> message;

		switch (static_cast<MessageCombo>(message)) {
		case MessageCombo::BROKEN_COMBO:
			// TODO Implement with new ID
			break;
		case MessageCombo::INCREMENT_COMBO:
			// TODO Implement with new ID
			break;
		}
	}
	
}

} // namespace tempo