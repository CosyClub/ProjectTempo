#include <tempo/entity/EntityCreationServer.hpp>

#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentGridAi.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/component/ComponentModel.hpp>
#include <glm/vec2.hpp>

namespace tempo
{

anax::Entity newPlayer(anax::World& world, tempo::SystemLevelManager system_grid_motion) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_player = world.createEntity();

	// not refactored
	//entity_player.addComponent<tempo::ComponentStagePosition>(system_grid_motion.spawn());
	
	// refactored
	entity_player.addComponent<tempo::ComponentCombo>();
	entity_player.addComponent<tempo::ComponentStagePosition>(glm::ivec2(5,5));
	entity_player.addComponent<tempo::ComponentStageTranslation>();
	entity_player.addComponent<tempo::ComponentPlayerRemote>();
	entity_player.addComponent<tempo::ComponentModel>("");
	
	entity_player.activate();

	return entity_player;
}


anax::Entity newAI(anax::World& world, int x, int y)
{
	anax::Entity entity_ai = world.createEntity();

	entity_ai.addComponent<tempo::ComponentGridAi>();
	entity_ai.addComponent<tempo::ComponentStagePosition>(glm::ivec2(x,y));
	entity_ai.addComponent<tempo::ComponentStageTranslation>();
	entity_ai.addComponent<tempo::ComponentModel>("");
	entity_ai.activate();

	return entity_ai;
}

anax::Entity newDestroyable(anax::World& world, int x, int y, std::string mesh_name)
{
	//TODO:: Add HealthComponent
	//TODO:: Add Entity to Specific Tile
	anax::Entity entity_object = world.createEntity();

	entity_object.addComponent<tempo::ComponentStagePosition>(glm::ivec2(x,y));
	entity_object.addComponent<tempo::ComponentStageTranslation>();
	entity_object.addComponent<tempo::ComponentModel>("");

	entity_object.activate();

	return entity_object;

}


anax::Entity newNonDestroyable(anax::World& world, int x, int y, std::string mesh_name)
{
	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_object = world.createEntity();

	entity_object.addComponent<tempo::ComponentStagePosition>(glm::ivec2(x,y));
	entity_object.addComponent<tempo::ComponentStageTranslation>();
	entity_object.addComponent<tempo::ComponentModel>("");

	entity_object.activate();

	return entity_object;

}

} // namespace tempo
