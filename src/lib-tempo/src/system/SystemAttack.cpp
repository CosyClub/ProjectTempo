#include <glm/glm.hpp>

#include <tempo/mask.hpp>

#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentWeapon.hpp>

#include <tempo/system/SystemAttack.hpp>

namespace tempo{

void SystemAttack::Attack(anax::Entity attacker){

	//Attacker
	glm::ivec2 attackerpos = attacker.getComponent<tempo::ComponentStagePosition>().getOrigin();
	auto& weapon = attacker.getComponent<tempo::ComponentWeapon>();
	Mask m = weapon.damage;
	glm::ivec2 size = m.size;

	//Victim
	auto entities = getEntities();

	for (auto& entity : entities) {

		//TODO:: Check whether this stays
		if(entity.getId() == attacker.getId()){
			continue;
		}


		glm::ivec2 pos = entity.getComponent<tempo::ComponentStagePosition>().getOrigin();
		auto& health = entity.getComponent<tempo::ComponentHealth>();

		//loop through all tiles that will recieve an attack
		for(int y = 0; y<size.y; y++){
			for(int x = 0; x<size.x; x++){

				//If Victim is located on one of the attacked tiles
				if(attackerpos.x + x == pos.x && attackerpos.y + y == pos.y){

					glm::ivec2 offset = {x,y};
					float damage = weapon.GetDamage(offset);
					health.HealthUpdate(damage);
				}

			}
		}

	}

}
}
