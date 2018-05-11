#include <tempo/component/ComponentParty.hpp>

namespace {
	static const constexpr int NUM_PARTIES       = 100;
	static const constexpr int PLAYERS_PER_PARTY =   6;

	//static bool occupied_party_slots[NUM_PARTIES][PLAYERS_PER_PARTY] = {0};

	static int  next_party_index[NUM_PARTIES] = {0};
}

namespace tempo
{

	ComponentParty::ComponentParty()
		: ComponentParty(0)
	{
		// no-op
	}

	ComponentParty::ComponentParty(uint32_t party_number) :
		party_number(party_number) {

		/*int index = -1;
		for(int i = 0; i < PLAYERS_PER_PARTY; ++i) {
			if(!occupied_party_slots[party_number][i]){
				index = i;
				occupied_party_slots[party_number][i] = true;
				break;
			}
		}

		this->entity_index = index;*/

		this->entity_index = next_party_index[party_number] % PLAYERS_PER_PARTY;
	  ++next_party_index[party_number];

		printf("Player has joined party %i in slot %i\n\n",
		       this->party_number, this->entity_index);
	}

	/////
	// Required for networking
	/////
	ComponentParty::ComponentParty(sf::Packet p)
	{
		p >> party_number;
		p >> entity_index;
	}

	sf::Packet ComponentParty::dumpComponent()
	{
		sf::Packet p;
		p << party_number;
		p << entity_index;

		return p;
	}

	ComponentID ComponentParty::getId()
	{
		return ComponentID::PARTY;
	}

	void ComponentParty::freePartySlot()
	{
		/*if(this->entity_index >= 0 && this->entity_index < PLAYERS_PER_PARTY &&
		   this->party_number >= 0 && this->party_number < NUM_PARTIES
		  ){
			occupied_party_slots[party_number][this->entity_index] = false;

			printf("Player has left party %i in slot %i\n\n",
		       this->party_number, this->entity_index);
		}
		this->entity_index = -1;*/
	}

}  // namespace tempo
