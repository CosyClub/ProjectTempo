#include <tempo/entity/EntityCreationServer.hpp>

#include <glm/vec2.hpp>

namespace tempo
{

EntityCreationData dumpEntity(anax::Entity e)
{
	EID type_id = (EID) e.getComponent<ComponentID>().type_id;
	glm::vec2 position;
	int instance_id   = e.getComponent<ComponentID>().instance_id;

	/* data.type_id = type_id; */
	/* data.instance_id = instance_id; */

	switch (type_id)
	{
		case EID_PLAYER:
			{
			//position = e.getComponent<ComponentStagePosition>().getPosition();
			//Player_t p;
			//p.foo = 0;
			//Entity_Type t;
			//t. player = p;
			//EntityCreationData data = {type_id, position, instance_id, t};
			//return data;
			assert(false);
			break;
			}
		case EID_AI:
			{
			//position = e.getComponent<ComponentStagePosition>().getPosition();
			//AI_t a;
			//a.foo = 0;
			//Entity_Type t;
			//t.ai = a;
			//EntityCreationData data = {type_id, position, instance_id, t};
			//return data;
			assert(false);
			break;
			}
		case EID_DES:
			{
			//position = e.getComponent<ComponentStagePosition>().getPosition();
			//Destroyable_t d;
			//memset(&(d.mesh_name), 0, 100);
			//
			//// TODO Sort this
			///* memcpy((void*)e.getComponent<ComponentRender>().path.c_str(), &(d.mesh_name), 100); */

			//Entity_Type t;
			//t.destroyable = d;
			//EntityCreationData data = {type_id, position, instance_id, t};
			//return data;
			assert(false);
			break;
			}
		case EID_NONDES:
			{
			//position = e.getComponent<ComponentStagePosition>().getPosition();
			//NonDestroyable_t n;
			//memset(&(n.mesh_name), 0, 100);
			//
			//// TODO Sort this
			///* memcpy((void*)e.getComponent<ComponentRender>().path.c_str(), &(n.mesh_name), 100); */
			//
			//Entity_Type t;
			//t.nondestroyable = n;
			//EntityCreationData data = {type_id, position, instance_id, t};
			//return data;
			assert(false);
			break;
			}
	}

}

anax::Entity newPlayer(anax::World& world, EID tid, tempo::SystemLevelManager system_grid_motion) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_player = world.createEntity();
	
	entity_player.addComponent<tempo::ComponentID>((int)tid);
	int iid = entity_player.getComponent<tempo::ComponentID>().instance_id;
	entity_player.addComponent<tempo::ComponentStagePosition>(system_grid_motion.spawn());
	entity_player.addComponent<tempo::ComponentStageTranslation>();
	entity_player.addComponent<tempo::ComponentPlayerRemoteServer>();	
	
	entity_player.activate();

	return entity_player;
}

anax::Entity newAI(anax::World& world, EID tid, int x, int y) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_ai = world.createEntity();

	entity_ai.addComponent<tempo::ComponentID>((int)tid);
	entity_ai.addComponent<tempo::ComponentStagePosition>(x, y, tempo::tileMask1by1, false);
	entity_ai.addComponent<tempo::ComponentStageTranslation>();
	entity_ai.addComponent<tempo::ComponentGridAi>();
	int iid = entity_ai.getComponent<tempo::ComponentID>().instance_id;
	entity_ai.activate();

	return entity_ai;
}

anax::Entity newDestroyable(anax::World& world, EID tid, int x, int y, std::string mesh_name) {

	//TODO:: Add HealthComponent
	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_object = world.createEntity();

	entity_object.addComponent<tempo::ComponentID>((int)tid);
	entity_object.addComponent<tempo::ComponentStagePosition>(x, y, tempo::tileMask1by1, false);
	entity_object.addComponent<tempo::ComponentStageTranslation>();

	entity_object.activate();

	return entity_object;

}

anax::Entity newNonDestroyable(anax::World& world, EID tid, int x, int y, std::string mesh_name) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_object = world.createEntity();

	entity_object.addComponent<tempo::ComponentID>((int)tid);
	entity_object.addComponent<tempo::ComponentStagePosition>(x, y, tempo::tileMask1by1, false);
	entity_object.addComponent<tempo::ComponentStageTranslation>();

	entity_object.activate();

	return entity_object;

}

}
