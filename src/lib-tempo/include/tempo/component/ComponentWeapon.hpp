#ifndef TEMPO_ENTITY_WEAPON_WEAPON_HPP
#define TEMPO_ENTITY_WEAPON_WEAPON_HPP

#include <tempo/config.hpp>
#include <tempo/mask.hpp>

#include <anax/Component.hpp>
#include <anax/System.hpp>
#include <glm/glm.hpp>

#include <tempo/component/NetworkedComponent.hpp>

#include <SFML/System/Time.hpp>

namespace tempo
{
struct ComponentWeapon
    : anax::Component
    , NetworkedComponent {
	Mask     damage;
	bool     isDelayed;
	sf::Time delay;

	ComponentWeapon(Mask &m);
	ComponentWeapon(Mask &m, sf::Time delay);

	float GetDamage(glm::vec2 offset);

	/////
	// Required for inital network sync
	/////
	ComponentWeapon(sf::Packet p);
	sf::Packet  dumpComponent();
	ComponentID getId();
};
}

#endif
