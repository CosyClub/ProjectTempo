#ifndef TEMPO_ENTITY_WEAPON_WEAPON_HPP
#define TEMPO_ENTITY_WEAPON_WEAPON_HPP

#include <tempo/config.hpp>
#include <tempo/mask.hpp>

#include <anax/System.hpp>
#include <anax/Component.hpp>
#include <glm/glm.hpp>

#include <SFML/System/Time.hpp>

namespace tempo
{

struct ComponentWeapon : anax::Component
{
	int attack_rate;
	sf::Time last_attack;
	Mask damage;

	ComponentWeapon(int rate, Mask m);
	float GetDamage(glm::vec2 offset);
};

} // namespace tempo

#endif
