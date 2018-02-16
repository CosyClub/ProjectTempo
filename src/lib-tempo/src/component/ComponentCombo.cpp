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

} // namespace tempo
