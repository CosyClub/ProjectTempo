#ifndef TEMPO_SYSTEM_SYSTEMATTACK_HPP
#define TEMPO_SYSTEM_SYSTEMATTACK_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <SFML/System/Networking>

namespace tempo{

class SystemAbstract {
	public:
	sf::Packet pack_data(anax::entity& e);
	void unpack_data(anax::entity e, sf::Packet p);
};

}

#endif
