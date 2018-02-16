#ifndef TEMPO_ENTITY_WEAPON_WEAPON_HPP
#define TEMPO_ENTITY_WEAPON_WEAPON_HPP

#include <tempo/config.hpp>
#include <tempo/mask.hpp>

#include <anax/System.hpp>
#include <anax/Component.hpp>
#include <glm/glm.hpp>

namespace tempo
{

struct ComponentWeapon : anax::Component
{
	int attack_rate;
	Mask damage;

	ComponentWeapon(int rate, Mask m);
	int GetDamage(glm::vec2 offset);
};

}

#endif
