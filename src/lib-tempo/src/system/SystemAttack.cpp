#include <glm/glm.hpp>

#include <tempo/mask.hpp>

#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentWeapon.hpp>

#include <tempo/system/SystemAttack.hpp>

namespace tempo{

void SystemAttack::Attack(anax::Entity attacker)
{

	//Attacker
	glm::ivec2 attackerpos = attacker.getComponent<tempo::ComponentStagePosition>().getOrigin();
	glm::ivec2 rot = attacker.getComponent<tempo::ComponentStageRotation>().facing;
	auto& weapon = attacker.getComponent<tempo::ComponentWeapon>();
	Mask m = weapon.damage;

	//Victim
	auto entities = getEntities();

	for (auto& entity : entities) {

		//TODO some team system

		glm::ivec2 pos = entity.getComponent<tempo::ComponentStagePosition>().getOrigin();
		auto& health = entity.getComponent<tempo::ComponentHealth>();

		glm::vec2 forward = rot;
		glm::vec2 left = glm::ivec2(rot.y * -1, rot.x * -1); //Hacky cross product

		glm::vec2 diff = pos - attackerpos;
		glm::ivec2 relative_diff = glm::ivec2(glm::dot(diff, left), glm::dot(diff, forward));

		float damage = weapon.GetDamage(relative_diff);
		if ( damage != 0 )
		{
			std::cout << "hit entity " << entity.getId().index
			          << " for " << damage << std::endl;
		}
		health.HealthUpdate( -1 * damage );	
	}
}

}
