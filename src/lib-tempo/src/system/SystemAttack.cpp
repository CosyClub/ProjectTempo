#include <glm/glm.hpp>

#include <tempo/mask.hpp>

#include <tempo/system/SystemAttack.hpp>

#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentWeapon.hpp>


namespace tempo{

SystemAttack::SystemAttack()
{
}

//
void SystemAttack::Attack(anax::Entity attacker){

	glm::ivec2 attackerpos = attacker.getComponent<tempo::ComponentStagePosition>().getOrigin();
	auto& weapon = attacker.getComponent<tempo::ComponentWeapon>();
	Mask m = weapon.damage;

	glm::ivec2 centre = m.centre;
	glm::ivec2 size = m.size;

	auto entities = getEntities();

	for (auto& entity : entities) {

		//TODO:: Check whether this stays
		if(entity.getId() == attacker.getId()){
			continue;
		}

		glm::ivec2 pos = entity.getComponent<tempo::ComponentStagePosition>().getOrigin();
		auto& health = entity.getComponent<tempo::ComponentHealth>();

		for(int y = 0; y<size.y; y++){
			for(int x = 0; x<size.x; x++){

				if(attackerpos.x + x == pos.x && attackerpos.y + y == pos.y){

					glm::ivec2 offset = {x,y};
					float damage = weapon.GetDamage(offset);
					health.HealthUpdate(damage);
				}

			}
		}

	}

}

//Get Mask positions
//Get positions of all attackable things (contain HealthComponent)
//if any match: entity.healthupdate();

SystemAttack::~SystemAttack()
{
}

// anax::Entity SystemID::get(int instance_id){
// 	if (id_map.find(instance_id) != id_map.end())
// 	{
// 		return id_map.find(instance_id)->second;
// 	}
// 	else
// 	{
// 		std::cout << "Entity with ID " << instance_id
// 		          << " not found in map" << std::endl;
// 		throw std::out_of_range("Entity not found in map");
// 	}
// }

// void SystemID::onEntityAdded(anax::Entity& e)
// {
// 	int instance_id = e.getComponent<ComponentID>().instance_id;
// 	id_map[instance_id] = e;
// }
// void SystemID::onEntityRemoved(anax::Entity& e)
// {
// 	id_map.erase(id_map.find(e.getComponent<ComponentID>().instance_id));
// }

}
