#include <tempo/entity/EntityCreationServer.hpp>

#include <glm/vec2.hpp>

namespace tempo
{

anax::Entity newPlayer(anax::World& world, tempo::SystemLevelManager system_grid_motion) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_player = world.createEntity();
	
	int iid = entity_player.getComponent<tempo::ComponentID>().instance_id;
	//entity_player.addComponent<tempo::ComponentStagePosition>(system_grid_motion.spawn());
	entity_player.addComponent<tempo::ComponentStageTranslation>();
	entity_player.addComponent<tempo::ComponentPlayerRemoteServer>();	
	
	entity_player.activate();

	return entity_player;
}

anax::Entity newAI(anax::World& world, int x, int y) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_ai = world.createEntity();

	//entity_ai.addComponent<tempo::ComponentStagePosition>(x, y, tempo::tileMask1by1, false);
	entity_ai.addComponent<tempo::ComponentStageTranslation>();
	entity_ai.addComponent<tempo::ComponentGridAi>();
	int iid = entity_ai.getComponent<tempo::ComponentID>().instance_id;
	entity_ai.activate();

	return entity_ai;
}

anax::Entity newDestroyable(anax::World& world, int x, int y, std::string mesh_name) {

	//TODO:: Add HealthComponent
	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_object = world.createEntity();

	//entity_object.addComponent<tempo::ComponentStagePosition>(x, y, tempo::tileMask1by1, false);
	entity_object.addComponent<tempo::ComponentStageTranslation>();

	entity_object.activate();

	return entity_object;

}

anax::Entity newNonDestroyable(anax::World& world, int x, int y, std::string mesh_name) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_object = world.createEntity();

	//entity_object.addComponent<tempo::ComponentStagePosition>(x, y, tempo::tileMask1by1, false);
	entity_object.addComponent<tempo::ComponentStageTranslation>();

	entity_object.activate();

	return entity_object;

}

}
