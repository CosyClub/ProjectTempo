#include <server/system/SystemHeartbeat.hpp>

#include <tempo/network/ID.hpp>
#include <tempo/network/queue.hpp>
#include <tempo/network/server.hpp>

#include <anax/Entity.hpp>

namespace server {

using tempo::operator<<;
using tempo::operator>>;

void SystemHeartbeat::checkForHeatbeats(anax::World &world)
{
	tempo::Queue<sf::Packet> *queue = tempo::get_system_queue(tempo::QueueID::HEARTBEAT);

	// Increment everyone's heartbeat
	tempo::cmtx.lock();
	for (tempo::clientpair element : tempo::clients) {
		tempo::clients[element.first].heartbeat += 1;
		std::cout << "Heartbeat inc from " << element.first << " " << tempo::clients[element.first].heartbeat << std::endl;
	}
	tempo::cmtx.unlock();

	// Check for keep-alive pacjets from clients
	while (!queue->empty()) {
		sf::Packet p = queue->front();
		queue->pop();
		
		uint32_t         ip_d;
		uint32_t         port;
		p >> ip_d >> port;
		sf::IpAddress ip(ip_d);

		uint32_t cid = tempo::findClientID(ip.toInteger(), port);
		if (cid != NO_CLIENT_ID) {
			tempo::cmtx.lock();
			tempo::clients[cid].heartbeat = 0;
			std::cout << "Heartbeat from " << cid << " " << tempo::clients[cid].heartbeat << std::endl;
			tempo::cmtx.unlock();
		}

	}

	// Check for anyone who hasn't sent a message in 10 beats
	tempo::Queue<sf::Packet> *dcq = tempo::get_system_queue(tempo::QueueID::ENTITY_DELETION);
	bool killed = false;
	tempo::cmtx.lock();
	for (tempo::clientpair element : tempo::clients) {
		if (element.second.heartbeat > 10) {
			sf::Packet dp;
			dp << element.second.id << element.second.ip << element.second.iport << false;
			dcq->push(dp);
			killed = true;
		}
	}
	tempo::cmtx.unlock();

	if (killed) {
		tempo::checkForClientDeletion(world);
	}
}

} // namespace server
