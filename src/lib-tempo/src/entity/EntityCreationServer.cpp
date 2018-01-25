#include <tempo/entity/EntityCreationServer.hpp>

#include <string.h>

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

anax::Entity newPlayer(anax::World& world, int iid, EID tid, tempo::SystemLevelManager system_grid_motion) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_player = world.createEntity();
	
	entity_player.addComponent<tempo::ComponentID>(iid, (int)tid);
	entity_player.addComponent<tempo::ComponentGridPosition>(system_grid_motion.spawn());
	entity_player.addComponent<tempo::ComponentGridMotion>();
	
	
	// TODO v this
	/* entity_player.addComponent<tempo::ComponentPlayerLocal>(); */
	
	
	entity_player.activate();

	return entity_player;
}

anax::Entity newAI(anax::World& world, int iid, EID tid, int x, int y) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_ai = world.createEntity();

	entity_ai.addComponent<tempo::ComponentID>(iid, (int)tid);
	entity_ai.addComponent<tempo::ComponentGridPosition>(x, y);
	entity_ai.addComponent<tempo::ComponentGridMotion>();
	entity_ai.addComponent<tempo::ComponentGridAi>();
	entity_ai.activate();

	return entity_ai;
}

anax::Entity newDestroyable(anax::World& world, int iid, EID tid, int x, int y, std::string mesh_name) {

	//TODO:: Add HealthComponent
	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_object = world.createEntity();

	entity_object.addComponent<tempo::ComponentID>(iid, (int)tid);
	entity_object.addComponent<tempo::ComponentGridPosition>(x, y);
	entity_object.addComponent<tempo::ComponentGridMotion>();

	entity_object.activate();

	return entity_object;

}

anax::Entity newNonDestroyable(anax::World& world, int iid, EID tid, int x, int y, std::string mesh_name) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_object = world.createEntity();

	entity_object.addComponent<tempo::ComponentID>(iid, (int)tid);
	entity_object.addComponent<tempo::ComponentGridPosition>(x, y);
	entity_object.addComponent<tempo::ComponentGridMotion>();

	entity_object.activate();

	return entity_object;

}

}
