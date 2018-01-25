#include <tempo/entity/EntityCreationServer.hpp>

#include <string.h>
#include <iostream>

namespace tempo
{

EntityCreationData dumpEntity(anax::Entity e)
{
	EID type_id = (EID) e.getComponent<ComponentID>().type_id;
	Vec2s position;
	int instance_id   = e.getComponent<ComponentID>().instance_id;

	/* data.type_id = type_id; */
	/* data.instance_id = instance_id; */

	switch (type_id)
	{
		case EID_PLAYER:
			{
			position = e.getComponent<ComponentGridPosition>().getPosition();
			Player_t p;
			p.foo = 0;
			Entity_Type t = (Entity_Type) {.player = p};
			EntityCreationData data = {type_id, position, instance_id, t};
			return data;
			break;
			}
		case EID_AI:
			{
			position = e.getComponent<ComponentGridPosition>().getPosition();
			AI_t a;
			a.foo = 0;
			Entity_Type t = (Entity_Type) {.ai = a};
			EntityCreationData data = {type_id, position, instance_id, t};
			return data;
			break;
			}
		case EID_DES:
			{
			position = e.getComponent<ComponentGridPosition>().getPosition();
			Destroyable_t d;
			memset(&(d.mesh_name), 0, 100);
			
			// TODO Sort this
			/* memcpy((void*)e.getComponent<ComponentRender>().path.c_str(), &(d.mesh_name), 100); */

			Entity_Type t = (Entity_Type) {.destroyable = d};
			EntityCreationData data = {type_id, position, instance_id, t};
			return data;
			break;
			}
		case EID_NONDES:
			{
			position = e.getComponent<ComponentGridPosition>().getPosition();
			NonDestroyable_t n;
			memset(&(n.mesh_name), 0, 100);
			
			// TODO Sort this
			/* memcpy((void*)e.getComponent<ComponentRender>().path.c_str(), &(n.mesh_name), 100); */
			
			Entity_Type t = (Entity_Type) {.nondestroyable = n};
			EntityCreationData data = {type_id, position, instance_id, t};
			return data;
			break;
			}
	}

}

anax::Entity newPlayer(anax::World& world, EID tid, tempo::SystemLevelManager system_grid_motion) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_player = world.createEntity();
	
	entity_player.addComponent<tempo::ComponentID>((int)tid);
	std::cout << "Created player with iid " << entity_player.getComponent<tempo::ComponentID>().instance_id << std::endl;
	int iid = entity_player.getComponent<tempo::ComponentID>().instance_id;
	id_map[iid] = entity_player;
	entity_player.addComponent<tempo::ComponentGridPosition>(system_grid_motion.spawn());
	entity_player.addComponent<tempo::ComponentGridMotion>();
	entity_player.addComponent<tempo::ComponentPlayerRemote>();	
	
	entity_player.activate();

	return entity_player;
}

anax::Entity newAI(anax::World& world, EID tid, int x, int y) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_ai = world.createEntity();

	entity_ai.addComponent<tempo::ComponentID>((int)tid);
	entity_ai.addComponent<tempo::ComponentGridPosition>(x, y, tempo::tileMask1by1, false);
	entity_ai.addComponent<tempo::ComponentGridMotion>();
	entity_ai.addComponent<tempo::ComponentGridAi>();
	int iid = entity_ai.getComponent<tempo::ComponentID>().instance_id;
	id_map[iid] = entity_ai;
	entity_ai.activate();

	return entity_ai;
}

anax::Entity newDestroyable(anax::World& world, EID tid, int x, int y, std::string mesh_name) {

	//TODO:: Add HealthComponent
	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_object = world.createEntity();

	entity_object.addComponent<tempo::ComponentID>((int)tid);
	entity_object.addComponent<tempo::ComponentGridPosition>(x, y, tempo::tileMask1by1, false);
	entity_object.addComponent<tempo::ComponentGridMotion>();

	entity_object.activate();

	return entity_object;

}

anax::Entity newNonDestroyable(anax::World& world, EID tid, int x, int y, std::string mesh_name) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_object = world.createEntity();

	entity_object.addComponent<tempo::ComponentID>((int)tid);
	entity_object.addComponent<tempo::ComponentGridPosition>(x, y, tempo::tileMask1by1, false);
	entity_object.addComponent<tempo::ComponentGridMotion>();

	entity_object.activate();

	return entity_object;

}

}
