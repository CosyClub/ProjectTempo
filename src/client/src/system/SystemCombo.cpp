#include <client/system/SystemCombo.hpp>

#include <tempo/network/ID.hpp>
#include <tempo/network/base.hpp>
#include <tempo/network/queue.hpp>

namespace client
{

using tempo::operator<<;
using tempo::operator>>;

void SystemCombo::advanceBeat()
{
	auto& entities = getEntities();

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
		queue->pop();

		anax::Entity::Id id;
		uint8_t          message;
		update >> id >> message;

		SERVERTOLOCAL(id)
		if (id.isNull()) continue;
		anax::Entity e(world, id);

		switch (static_cast<tempo::MessageCombo>(message)) {
		case tempo::MessageCombo::BROKEN_COMBO:
			e.getComponent<tempo::ComponentCombo>().breakCombo();
			break;
		case tempo::MessageCombo::INCREMENT_COMBO:
			e.getComponent<tempo::ComponentCombo>().performAction();
			break;
		}
	}
}

}  // namespace client
