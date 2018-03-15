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
	float arr[9] = {0, 0, 0,
	                1, 1, 1,
	                1, 1, 1};
	Mask  m(glm::ivec2(1, 0), arr, glm::ivec2(3, 3));

	// refactored
	entity_player.addComponent<tempo::ComponentCombo>();
	entity_player.addComponent<tempo::ComponentStagePosition>(glm::ivec2(5, 5));
	entity_player.addComponent<tempo::ComponentStageRotation>(NORTH);
	entity_player.addComponent<tempo::ComponentStageTranslation>();
	entity_player.addComponent<tempo::ComponentPlayerRemote>();
	entity_player.addComponent<tempo::ComponentModel>("resources/materials/textures/rogue.png", glm::vec3(255, 255, 255), false, glm::vec2(4,4));
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

	e.addComponent<tempo::ComponentAI>(MoveType::MOVE_NONE, false, false);
	e.addComponent<tempo::ComponentStagePosition>(pos);
	e.addComponent<tempo::ComponentStageRotation>(NORTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>("resources/materials/textures/player.png", glm::vec3(255, 255, 255), false, glm::vec2(4,4));
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

	e.addComponent<tempo::ComponentAI>(MoveType::MOVE_NONE, false, false);
	e.addComponent<tempo::ComponentStagePosition>(pos);
	e.addComponent<tempo::ComponentStageRotation>(NORTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>("resources/materials/textures/player.png", glm::vec3(255, 255, 255), false, glm::vec2(4,4));
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

	e.addComponent<tempo::ComponentAI>(MoveType::MOVE_WANDER, false, false);
	e.addComponent<tempo::ComponentStagePosition>(pos);
	e.addComponent<tempo::ComponentStageRotation>(NORTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>("resources/materials/textures/player.png", glm::vec3(255, 200, 200), false, glm::vec2(4,4));
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(5);
	float arr[9] = {5, 5, 5,
	                5, 5, 5,
	                5, 5, 5};
	Mask  m(glm::ivec2(1, 1), arr, glm::ivec2(3, 3));
	e.addComponent<tempo::ComponentAttack>();
	e.addComponent<tempo::ComponentWeapon>(m, (unsigned int)5);
	e.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);

	e.activate();
	return e;
}
anax::Entity createMobPatroller(anax::World &world, glm::ivec2 pos, std::deque<glm::ivec2> path)
{
	anax::Entity e = world.createEntity();

	e.addComponent<tempo::ComponentAI>(MoveType::MOVE_PATROL, false, false, path);
	e.addComponent<tempo::ComponentStagePosition>(pos);
	e.addComponent<tempo::ComponentStageRotation>(NORTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>("resources/materials/textures/player.png", glm::vec3(255, 255, 255), false, glm::vec2(4,4));
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(5);
	float arr[2] = {0, 1};
	Mask  m(glm::ivec2(0, 0), arr, glm::ivec2(1, 2));
	e.addComponent<tempo::ComponentAttack>();
	e.addComponent<tempo::ComponentWeapon>(m, (unsigned int)0);
	e.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);

	e.activate();
	return e;
}
anax::Entity createMobAntiSnail(anax::World &world, glm::ivec2 pos)
{
	anax::Entity e = world.createEntity();

	std::deque<glm::ivec2> path;
	path.push_back(pos);
	e.addComponent<tempo::ComponentAI>(MoveType::MOVE_PATH, false, false, path);
	e.addComponent<tempo::ComponentStagePosition>(pos);
	e.addComponent<tempo::ComponentStageRotation>(NORTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>("resources/materials/textures/player.png", glm::vec3(255, 255, 255), false, glm::vec2(4,4));
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(5);
	float arr[2] = {0, 1};
	Mask  m(glm::ivec2(0, 0), arr, glm::ivec2(1, 2));
	e.addComponent<tempo::ComponentAttack>();
	e.addComponent<tempo::ComponentWeapon>(m, (unsigned int)0);
	e.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);

	e.activate();
	return e;
}

}  // namespace tempo
