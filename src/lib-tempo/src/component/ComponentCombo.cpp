#include <tempo/component/ComponentCombo.hpp>

namespace tempo
{
ComponentCombo::ComponentCombo()
{
	actionedThisBeat = false;
	comboCounter     = 0;
}

void ComponentCombo::breakAction()
{
	if (comboCounter <= 0) {
		comboCounter = 0;
	}
	else if (comboCounter <= 10) {
		comboCounter -= 1;
	}
	else if (comboCounter <= 20) {
		comboCounter -= 2;
	}
	else if (comboCounter <= 35) {
		comboCounter -= 5;
	}
	else if (comboCounter <= 50) {
		comboCounter -= 7;
	}
	else if (comboCounter <= 80) {
		comboCounter -= 10;
	}
	else if (comboCounter <= 120) {
		comboCounter -= 50;
	}
	else if (comboCounter <= 200) {
		comboCounter -= 10;
	}
	else if (comboCounter <= 500) {
		comboCounter -= 200;
	}
 
	comboCounter =  std::max(comboCounter, 0);
}

void ComponentCombo::advanceBeat()
{
	if (actionedThisBeat == false) {
		breakAction();
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
		breakAction();
		return false;
	}
}

void ComponentCombo::breakCombo()
{
	actionedThisBeat = true;
	breakAction();
}

void ComponentCombo::zeroCombo()
{
	actionedThisBeat = true;
	comboCounter = 0;
}

/////
// Required for networking
/////
ComponentCombo::ComponentCombo(sf::Packet p)
{
	p >> actionedThisBeat >> comboCounter;
}

ComponentID ComponentCombo::getId()
{
	return ComponentID::COMBO;
}

sf::Packet ComponentCombo::dumpComponent()
{
	sf::Packet p;
	p << actionedThisBeat << comboCounter;
	return p;
}

}  // namespace tempo
