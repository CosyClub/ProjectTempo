#include <tempo/component/ComponentParty.hpp>


namespace tempo
{

	ComponentParty::ComponentParty()
	{
		party_number = 0;
	}

	ComponentParty::ComponentParty(uint32_t party_number) :
		party_number(party_number) {}
	/////
	// Required for networking
	/////
	ComponentParty::ComponentParty(sf::Packet p)
	{
		p >> party_number;
	}

	sf::Packet ComponentParty::dumpComponent()
	{
		sf::Packet p;
		p << party_number;

		return p;
	}

	ComponentID ComponentParty::getId()
	{
		return ComponentID::PARTY;
	}

}  // namespace tempo
