#include <server/system/SystemCombo.hpp>

#include <tempo/network/ID.hpp>
#include <tempo/network/base.hpp>
#include <tempo/network/queue.hpp>

namespace server
{

using tempo::operator<<;
using tempo::operator>>;

void SystemCombo::advanceBeat()
{
	auto entities = getEntities();

	for (auto &entity : entities) {
		auto &combo = entity.getComponent<tempo::ComponentCombo>();
		combo.advanceBeat();
	}
}

void SystemCombo::checkForUpdates(anax::World &world)
{
	tempo::Queue<sf::Packet> *queue = get_system_queue(tempo::QueueID::COMBO_UPDATES);
	if (queue->empty())
		return;

	while (!queue->empty()) {
		sf::Packet update = queue->front();
		sf::Packet broadcast;
		queue->pop();

		anax::Entity::Id id;
		uint8_t         message;
		update    >> id >> message;
		broadcast << id << message; 
		anax::Entity e(world, id);

		switch (static_cast<tempo::MessageCombo>(message)) {
		case tempo::MessageCombo::BROKEN_COMBO:
			if (e.hasComponent<tempo::ComponentCombo>()) {
				e.getComponent<tempo::ComponentCombo>().breakCombo();
			}
			break;
		case tempo::MessageCombo::INCREMENT_COMBO:
			if (e.hasComponent<tempo::ComponentCombo>()) {
				e.getComponent<tempo::ComponentCombo>().performAction();
			}
			break;
		}

		tempo::broadcastMessage(tempo::QueueID::COMBO_UPDATES, broadcast);
	}
}

}  // namespace tempo