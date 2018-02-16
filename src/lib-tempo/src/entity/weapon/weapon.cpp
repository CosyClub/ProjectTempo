#include <tempo/entity/weapon/weapon.hpp>


namespace tempo
{
	ComponentWeapon::ComponentWeapon(int rate, Mask m)
	{
		attack_rate = rate;
		last_attack = sf::microseconds(0);
		damage = m;
	}

	float ComponentWeapon::GetDamage(glm::vec2 offset)
	{
		//TODO
	}
}
