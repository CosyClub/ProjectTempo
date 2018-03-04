#include <server/system/SystemAttack.hpp>

#include <tempo/component/NetworkedComponent.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentWeapon.hpp>

#include <tempo/network/ID.hpp>
#include <tempo/network/server.hpp>

#include <tempo/mask.hpp>

namespace server
{

void SystemAttack::Sync(anax::World &w)
{
	tempo::Queue<sf::Packet> *q = tempo::get_system_queue(tempo::QueueID::SYSTEM_ATTACK);

	while (!q->empty()) {
		sf::Packet p = q->front();
		q->pop();

		for (auto &client : tempo::clients) {
			tempo::sendMessage(tempo::QueueID::SYSTEM_ATTACK, p, client.first);
		}
	}
}

} // namespace server
