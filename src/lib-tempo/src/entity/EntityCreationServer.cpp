#include <tempo/entity/EntityCreationServer.hpp>

namespace tempo
{
anax::Entity newPlayer(anax::World &world)
{
	// TODO:: Add Entity to Specific Tile

	anax::Entity entity_player = world.createEntity();

	// not refactored
	// entity_player.addComponent<tempo::ComponentStagePosition>(system_grid_motion.spawn());
	//
	float arr[6] = {0, 0, 0,
	                1, 1, 1};
	Mask  m(glm::ivec2(1, 0), arr, glm::ivec2(3, 2));

	// refactored
	entity_player.addComponent<tempo::ComponentCombo>();
	entity_player.addComponent<tempo::ComponentStagePosition>(glm::ivec2(5, 5));
	entity_player.addComponent<tempo::ComponentStageRotation>(NORTH);
	entity_player.addComponent<tempo::ComponentStageTranslation>();
	entity_player.addComponent<tempo::ComponentPlayerRemote>();
	entity_player.addComponent<tempo::ComponentModel>("", glm::vec3(255, 0, 0), false);
	entity_player.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	entity_player.addComponent<tempo::ComponentAttack>();
	entity_player.addComponent<tempo::ComponentWeapon>(m);
	entity_player.addComponent<tempo::ComponentAOEIndicator>();
	entity_player.addComponent<tempo::ComponentHealth>(10);

	entity_player.activate();

	return entity_player;
}

anax::Entity createMobStill(anax::World &world, glm::ivec2 pos)
{
	anax::Entity e = world.createEntity();

	e.addComponent<tempo::ComponentAI>();
	e.addComponent<tempo::ComponentStagePosition>(pos);
	e.addComponent<tempo::ComponentStageRotation>(NORTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>("", glm::vec3(0, 255, 0), false);
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(1);
	float arr[2] = {0, 1};
	Mask  m(glm::ivec2(0, 0), arr, glm::ivec2(1, 2));
	e.addComponent<tempo::ComponentAttack>();
	e.addComponent<tempo::ComponentWeapon>(m);
	e.addComponent<tempo::ComponentTeam>(Team::BADGUYS);

	e.activate();
	return e;
}

anax::Entity createMobStillAOE(anax::World &world, glm::ivec2 pos)
{
	anax::Entity e = world.createEntity();

	e.addComponent<tempo::ComponentAI>();
	e.addComponent<tempo::ComponentStagePosition>(pos);
	e.addComponent<tempo::ComponentStageRotation>(NORTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>("", glm::vec3(0, 255, 0), false);
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(2);
	float arr[9] = {1, 1, 1,
	                1, 0, 1,
	                1, 1, 1};
	Mask  m(glm::ivec2(1, 1), arr, glm::ivec2(3, 3));
	e.addComponent<tempo::ComponentAttack>();
	e.addComponent<tempo::ComponentWeapon>(m);
	e.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);
	e.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);

	e.activate();
	return e;
}

anax::Entity createMobCreeper(anax::World &world, glm::ivec2 pos)
{
	anax::Entity e = world.createEntity();

	e.addComponent<tempo::ComponentAI>();
	e.addComponent<tempo::ComponentStagePosition>(pos);
	e.addComponent<tempo::ComponentStageRotation>(NORTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>("", glm::vec3(0, 255, 0), false);
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(5);
	float arr[9] = {5, 5, 5,
	                5, 5, 5,
	                5, 5, 5};
	Mask  m(glm::ivec2(1, 1), arr, glm::ivec2(3, 3));
	e.addComponent<tempo::ComponentAttack>();
	e.addComponent<tempo::ComponentWeapon>(m, 5);
	e.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);

	e.activate();
	return e;
}

}  // namespace tempo
