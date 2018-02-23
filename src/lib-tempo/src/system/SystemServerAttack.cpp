#include <tempo/system/SystemServerAttack.hpp>

namespace tempo{

void SystemServerAttack::Sync(anax::World& w)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(QueueID::SYSTEM_ATTACK);

	while( ! q->empty() )
	{
		sf::Packet p = q->front();
		q->pop();

		for (auto& client : clients)
		{
			sendMessage(QueueID::SYSTEM_ATTACK, p, client.first);
		}
	}
}

}
