#include <tempo/component/ComponentCombo.hpp>

namespace tempo
{
ComponentCombo::ComponentCombo()
{
	actionedThisBeat = false;
	comboCounter     = 0;
}

void ComponentCombo::ComboBreakAction()
{
	comboCounter -= std::max(comboCounter / 4, 1);
	comboCounter =  std::max(comboCounter, 0);
}

void ComponentCombo::advanceBeat()
{
	if (actionedThisBeat == false) {
		ComboBreakAction();
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
		ComboBreakAction();
		return false;
	}
}

void ComponentCombo::breakCombo()
{
	actionedThisBeat = true;
	ComboBreakAction();
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
