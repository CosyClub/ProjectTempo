#include <tempo/entity/weapon/weapon.hpp>


namespace tempo
{
	ComponentWeapon::ComponentWeapon(int rate, Mask m)
	{
		attack_rate = rate;
		damage = m;
	}

	int ComponentWeapon::GetDamage(glm::vec2 offset)
	{
	}
}
