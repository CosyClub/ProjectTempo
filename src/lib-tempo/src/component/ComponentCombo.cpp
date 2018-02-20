#include <tempo/component/ComponentCombo.hpp>

namespace tempo
{

ComponentCombo::ComponentCombo()
{
	actionedThisBeat = false;
	comboCounter = 0;
}

void ComponentCombo::advanceBeat()
{
	if (actionedThisBeat == false) {
		comboCounter = 0;
	}
	actionedThisBeat = false;
}

bool ComponentCombo::performAction() 
{
	if (!actionedThisBeat) {
		actionedThisBeat = true;
		comboCounter++;
		return true;
	} else {
		comboCounter = 0;
		return false;
	}
}

void ComponentCombo::breakCombo() 
{
	actionedThisBeat = true;
	comboCounter     = 0;
}
	

/////
// Required for networking
/////
ComponentCombo::ComponentCombo(sf::Packet p)
{
	p >> actionedThisBeat
	  >> comboCounter;
}

ComponentID ComponentCombo::getId()
{
	return ComponentID::COMBO;
}

sf::Packet ComponentCombo::dumpComponent()
{
	sf::Packet p;
	p << actionedThisBeat
	  << comboCounter;
	return p;
}

} // namespace tempo
