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
	if (comboCounter == 0) return;
	comboCounter -= std::max(comboCounter / 4, 1);
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
