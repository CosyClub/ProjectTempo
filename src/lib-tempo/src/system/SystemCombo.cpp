#include <tempo/system/SystemCombo.hpp>

#include <tempo/network/ID.hpp>
#include <tempo/network/base.hpp>
#include <tempo/network/queue.hpp>

namespace tempo
{
void SystemCombo::advanceBeat()
{
	auto entities = getEntities();

	for (auto &entity : entities) {
		auto &combo = entity.getComponent<tempo::ComponentCombo>();
		combo.advanceBeat();
	}
}

void SystemCombo::checkForUpdates()
{
	tempo::Queue<sf::Packet> *queue = get_system_queue(QueueID::COMBO_UPDATES);
	if (queue->empty())
		return;

	while (!queue->empty()) {
		sf::Packet update = queue->front();
		queue->pop();

		anax::Entity::Id id;
		uint8_t          message;
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

}  // namespace tempo
