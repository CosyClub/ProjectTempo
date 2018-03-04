#ifndef TEMPO_COMPONENT_COMPONENT_COMBO_HPP
#define TEMPO_COMPONENT_COMPONENT_COMBO_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{
// Used to communicate the state of the combo when multiplayer
enum MessageCombo { BROKEN_COMBO, INCREMENT_COMBO };

// Component captures a combo mechanic for the entity with this component.
struct ComponentCombo
    : anax::Component
    , NetworkedComponent {
	bool         actionedThisBeat;
	unsigned int comboCounter;

	// Creates a new component combo with no combo (and optionally given
	ComponentCombo();

	// Contains logic that should be run when the beat advances
	void advanceBeat();

	// Contains logic that should be run when the entity with a combo
	// performs an action.
	// Returns whether or not the combo was broken
	bool performAction();

	// Contains logic to break the entity's combo without hesitation. Once
	// a combo is broken it will not be recoverable until advanceBeat() is
	// called.
	void breakCombo();

	/////
	// Required for networking
	/////
	ComponentCombo(sf::Packet p);
	ComponentID getId();
	sf::Packet  dumpComponent();
};

}  // namespace tempo

#endif
