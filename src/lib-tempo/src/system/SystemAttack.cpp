#include <tempo/system/SystemAttack.hpp>

namespace tempo{

//SystemCanBeAttacked
SystemAttack::SystemAttack()
{
}

//BeAttacked

SystemAttack::Attack(anax::Entity attacker){

	glm::ivec2 Attackerpos = attacker.getComponent<tempo::ComponentStagePosition>().getOrigin();
	auto& weapon = attacker.getComponent<tempo::ComponentWeapon>();
	Mask m = weapon.damage;

	glm::ivec2 centre = m.centre
	glm::ivec2 size = m.size;

	auto entities = getEntities();

	for (auto& entity : entities) {

		auto& health = entity.getComponent<tempo::ComponentHealth>();

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
