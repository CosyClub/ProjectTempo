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
	float arr[12] = {0, 0, 0,
	                 10, 10, 10,
	                 10, 10, 10,
	                 10, 10, 10};
	Mask  m(glm::ivec2(1, 0), arr, glm::ivec2(3, 4));

	int min = 0;
	int max = 5;
	int randNum = rand()%(max-min + 1) + min;
	std::string path = "resources/materials/textures/player-" + std::to_string(randNum) + ".png";
	// refactored
	entity_player.addComponent<tempo::ComponentCombo>();
	entity_player.addComponent<tempo::ComponentStagePosition>(glm::ivec2(40, 7));
	entity_player.addComponent<tempo::ComponentStageRotation>(NORTH);
	entity_player.addComponent<tempo::ComponentStageTranslation>();
	entity_player.addComponent<tempo::ComponentPlayerRemote>();
	entity_player.addComponent<tempo::ComponentModel>(path, glm::vec3(255, 255, 255), false, glm::vec2(4,4));
	entity_player.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	entity_player.addComponent<tempo::ComponentAttack>();
	entity_player.addComponent<tempo::ComponentWeapon>(m);
	entity_player.addComponent<tempo::ComponentAOEIndicator>();
	entity_player.addComponent<tempo::ComponentHealth>(100);
	entity_player.addComponent<tempo::ComponentTeam>(Team::BADGUYS);

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
	int a = rand()%2;
	std::string path = "resources/materials/textures/zombie-sheet-" + std::to_string(a) + ".png";
	e.addComponent<tempo::ComponentModel>(path, glm::vec3(255, 255, 255), false, glm::vec2(2,4));
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(10);
	float arr[2] = {0, 10};
	Mask  m(glm::ivec2(0, 0), arr, glm::ivec2(1, 2));
	e.addComponent<tempo::ComponentAttack>();
	e.addComponent<tempo::ComponentWeapon>(m);
	e.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);
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
	e.addComponent<tempo::ComponentModel>("resources/materials/textures/totem.png", glm::vec3(255, 255, 255), false, glm::vec2(1,1));
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(20);
	float arr[9] = {10, 10, 10,
	                10, 0,  10,
	                10, 10, 10};
	Mask  m(glm::ivec2(1, 1), arr, glm::ivec2(3, 3));
	e.addComponent<tempo::ComponentAttack>();
	e.addComponent<tempo::ComponentWeapon>(m);
	e.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);

	e.activate();
	return e;
}

anax::Entity createMobCreeper(anax::World &world, glm::ivec2 pos)
{
	anax::Entity e = world.createEntity();
	
	std::string path = "resources/materials/textures/creeper.png";

	e.addComponent<tempo::ComponentAI>(MoveType::MOVE_WANDER, false, false);
	e.addComponent<tempo::ComponentStagePosition>(pos);
	e.addComponent<tempo::ComponentStageRotation>(NORTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>(path, glm::vec3(255, 200, 200), false, glm::vec2(2,4));
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(50);
	float arr[9] = {50, 50, 50,
	                50, 50, 50,
	                50, 50, 50};
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

	int min = 0;
	int max = 2;
	int randNum = rand()%(max-min + 1) + min;
	std::string path1 = "resources/materials/textures/knight-" + std::to_string(randNum) + ".png";

	e.addComponent<tempo::ComponentAI>(MoveType::MOVE_PATROL, false, false, path);
	e.addComponent<tempo::ComponentStagePosition>(pos);
	e.addComponent<tempo::ComponentStageRotation>(NORTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>(path1, glm::vec3(255, 255, 255), false, glm::vec2(5,4));
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(50);
	float arr[2] = {0, 10};
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
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(50);
	float arr[2] = {0, 10};
	Mask  m(glm::ivec2(0, 0), arr, glm::ivec2(1, 2));
	e.addComponent<tempo::ComponentAttack>();
	e.addComponent<tempo::ComponentWeapon>(m, (unsigned int)0);
	e.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);

	e.activate();
	return e;
}

anax::Entity createButtonGroup(anax::World &           world,
                               std::vector<glm::ivec2> positions,
                               std::vector<glm::ivec2> tiles,
                               std::vector<glm::ivec2> spikes,
                               glm::ivec2              prev,
                               glm::ivec2              next,
                               bool                    triggerable,
                               int                     ID)
{
	anax::Entity entity_button = world.createEntity();
	entity_button.addComponent<tempo::ComponentButtonGroup>(positions, tiles, spikes, prev, next, triggerable, ID);
	entity_button.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	entity_button.activate();

	return entity_button;
}

void createSpikes(anax::World & world, std::vector<glm::ivec2> positions) {

	for(glm::ivec2 pos : positions){
		anax::Entity entity_spikes = world.createEntity();
		entity_spikes.addComponent<tempo::ComponentAI>(MoveType::MOVE_NONE, false, false);
		entity_spikes.addComponent<tempo::ComponentSpikes>(positions);
		entity_spikes.addComponent<tempo::ComponentStagePosition>(pos);
		entity_spikes.addComponent<tempo::ComponentStageTranslation>();
		entity_spikes.addComponent<tempo::ComponentStageRotation>(SOUTH);
		entity_spikes.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
		float arr[1] = {25};
		Mask  m(glm::ivec2(0, 0), arr, glm::ivec2(1, 1));
		entity_spikes.addComponent<tempo::ComponentAttack>();
		entity_spikes.addComponent<tempo::ComponentWeapon>(m, (unsigned int)0);
		entity_spikes.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);
		entity_spikes.activate();
	}

}

}  // namespace tempo
