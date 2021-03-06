#include <tempo/entity/EntityCreationServer.hpp>
#include <tempo/constants.hpp>

namespace tempo
{
anax::Entity newPlayer(anax::World &world, uint32_t party_number)
{
	// TODO:: Add Entity to Specific Tile

	anax::Entity entity_player = world.createEntity();

	// not refactored
	// entity_player.addComponent<tempo::ComponentStagePosition>(system_grid_motion.spawn());
	//
	float arr[12] = {0, 0, 0,
	                 9, 10, 8,
	                 7, 10, 7,
	                 0, 8, 0};
	Mask  m(glm::ivec2(1, 0), arr, glm::ivec2(3, 4));

	int min = 0;
	int max = 5;
	int randNum = rand()%(max-min + 1) + min;
	std::string path = "resources/materials/textures/player-" + std::to_string(randNum) + ".png";
	// refactored
	entity_player.addComponent<tempo::ComponentCombo>();

	int emptySpace = 40;
	int fheight = 40 + emptySpace;

	auto& comp_party = entity_player.addComponent<tempo::ComponentParty>(party_number);

	glm::ivec2 player_spawn_location = glm::ivec2(10 + (party_number * fheight), 0);

	// Modify the spawn location based on the entity index within the party
	// This is hard coded but relies on the level design!
	switch(comp_party.entity_index){

	case 1: player_spawn_location += glm::ivec2(- 2,  0); break;
	case 2: player_spawn_location += glm::ivec2(- 4,  0); break;

	case 3: player_spawn_location += glm::ivec2(-10,  6); break;
	case 4: player_spawn_location += glm::ivec2(-10,  8); break;
	case 5: player_spawn_location += glm::ivec2(-10, 10); break;
	}

	entity_player.addComponent<tempo::ComponentRespawn>(player_spawn_location);
	entity_player.addComponent<tempo::ComponentStagePosition>(player_spawn_location);
	entity_player.addComponent<tempo::ComponentStageRotation>(SOUTH);
	entity_player.addComponent<tempo::ComponentStageTranslation>();
	entity_player.addComponent<tempo::ComponentPlayerRemote>();
	entity_player.addComponent<tempo::ComponentModel>(path, glm::vec3(255, 255, 255), false, glm::vec2(4,4));
	entity_player.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	entity_player.addComponent<tempo::ComponentAttack>();
	entity_player.addComponent<tempo::ComponentWeapon>(m);
	entity_player.addComponent<tempo::ComponentAOEIndicator>();
	entity_player.addComponent<tempo::ComponentHealth>(PLAYER_MAX_HEALTH);
	entity_player.addComponent<tempo::ComponentTeam>(Team::BADGUYS);

	entity_player.activate();

	return entity_player;
}

anax::Entity createMobStill(anax::World &world, glm::ivec2 pos)
{
	anax::Entity e = world.createEntity();

	e.addComponent<tempo::ComponentAI>(MoveType::MOVE_NONE, false, false);
	e.addComponent<tempo::ComponentStagePosition>(pos, false);
	e.addComponent<tempo::ComponentStageRotation>(SOUTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	int a = rand()%2;
	std::string path = "resources/materials/textures/zombie-sheet-" + std::to_string(a) + ".png";
	e.addComponent<tempo::ComponentModel>(path, glm::vec3(255, 255, 255), false, glm::vec2(2,4));
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(9);
	float arr[2] = {0, PLAYER_MAX_HEALTH * 0.4};
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
	e.addComponent<tempo::ComponentStagePosition>(pos, false);
	e.addComponent<tempo::ComponentStageRotation>(SOUTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>("resources/materials/textures/totem.png", glm::vec3(255, 255, 255), false, glm::vec2(1,1));
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(18);
	float arr[9] = { PLAYER_MAX_HEALTH * 0.3, PLAYER_MAX_HEALTH * 0.3, PLAYER_MAX_HEALTH * 0.3,
		PLAYER_MAX_HEALTH * 0.3, 0,  PLAYER_MAX_HEALTH * 0.3,
		PLAYER_MAX_HEALTH * 0.3, PLAYER_MAX_HEALTH * 0.3, PLAYER_MAX_HEALTH * 0.3 };
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

	e.addComponent<tempo::ComponentAI>(MoveType::MOVE_AGGRO, false, false);
	e.addComponent<tempo::ComponentStagePosition>(pos, false);
	e.addComponent<tempo::ComponentStageRotation>(SOUTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>(path, glm::vec3(255, 200, 200), false, glm::vec2(2,4));
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(28);
	float arr[9] = { PLAYER_MAX_HEALTH * 0.7, PLAYER_MAX_HEALTH * 0.8, PLAYER_MAX_HEALTH * 0.8,
		PLAYER_MAX_HEALTH * 0.8, PLAYER_MAX_HEALTH * 0.9, PLAYER_MAX_HEALTH * 0.8,
		PLAYER_MAX_HEALTH * 0.7, PLAYER_MAX_HEALTH * 0.8, PLAYER_MAX_HEALTH * 0.7 };
	Mask  m(glm::ivec2(1, 1), arr, glm::ivec2(3, 3));
	e.addComponent<tempo::ComponentAttack>();
	e.addComponent<tempo::ComponentWeapon>(m, (unsigned int)3);
	e.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);

	e.activate();
	return e;
}

anax::Entity createMobChaser(anax::World &world, glm::ivec2 pos)
{
	anax::Entity e = world.createEntity();

	std::string path1 = "resources/materials/textures/knight-0.png";

	e.addComponent<tempo::ComponentAI>(MoveType::MOVE_AGGRO, false, false);
	e.addComponent<tempo::ComponentStagePosition>(pos, false);
	e.addComponent<tempo::ComponentStageRotation>(SOUTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>(path1, glm::vec3(255, 255, 255), false, glm::vec2(5, 4));
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(38);
	float arr[2] = { 0, PLAYER_MAX_HEALTH * 0.3 };
	Mask  m(glm::ivec2(0, 0), arr, glm::ivec2(1, 2));
	e.addComponent<tempo::ComponentAttack>();
	e.addComponent<tempo::ComponentWeapon>(m, (unsigned int)3);
	e.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);

	e.activate();
	return e;
}

anax::Entity createMobPatroller(anax::World &world, glm::ivec2 pos, std::deque<glm::ivec2> path)
{
	anax::Entity e = world.createEntity();

	int min = 1;
	int max = 2;
	int randNum = rand()%(max-min + 1) + min;
	std::string path1 = "resources/materials/textures/knight-" + std::to_string(randNum) + ".png";

	e.addComponent<tempo::ComponentAI>(MoveType::MOVE_PATROL, false, false, path);
	e.addComponent<tempo::ComponentStagePosition>(pos, false);
	e.addComponent<tempo::ComponentStageRotation>(SOUTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>(path1, glm::vec3(255, 255, 255), false, glm::vec2(5,4));
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(40);
	float arr[2] = {0, PLAYER_MAX_HEALTH * 0.25 };
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
	e.addComponent<tempo::ComponentStagePosition>(pos, false);
	e.addComponent<tempo::ComponentStageRotation>(SOUTH);
	e.addComponent<tempo::ComponentStageTranslation>();
	e.addComponent<tempo::ComponentModel>("resources/materials/textures/player.png", glm::vec3(255, 255, 255), false, glm::vec2(4,4));
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	e.addComponent<tempo::ComponentHealth>(28);
	float arr[2] = {0, PLAYER_MAX_HEALTH * 0.2 };
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
                               glm::ivec2              respawn_pos,
                               glm::ivec2              prev,
                               glm::ivec2              next,
                               bool                    triggerable,
                               int                     ID)
{
	anax::Entity entity_button = world.createEntity();
	entity_button.addComponent<tempo::ComponentButtonGroup>(positions, tiles, spikes, respawn_pos, prev, next, triggerable, ID);
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
		float arr[1] = { PLAYER_MAX_HEALTH * 0.65 };
		Mask  m(glm::ivec2(0, 0), arr, glm::ivec2(1, 1));
		entity_spikes.addComponent<tempo::ComponentAttack>();
		entity_spikes.addComponent<tempo::ComponentWeapon>(m, (unsigned int)0);
		entity_spikes.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);
		entity_spikes.activate();
	}

}

anax::Entity createSnake(anax::World& world, glm::ivec2 pos, tempo::Facing f, int len)
{
	anax::Entity head = world.createEntity();

	ai_index_top++;
	head.addComponent<tempo::ComponentAI>(MoveType::MOVE_SNAKE, false, false);
	head.addComponent<tempo::ComponentStagePosition>(pos, false);
	head.addComponent<tempo::ComponentStageRotation>(f);
	head.addComponent<tempo::ComponentStageTranslation>();
	head.addComponent<tempo::ComponentModel>("resources/materials/textures/player.png", glm::vec3(255, 200, 200), false, glm::vec2(4,4));
	head.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	head.addComponent<tempo::ComponentHealth>(10);
	float arr[9] = { PLAYER_MAX_HEALTH * 0.4, PLAYER_MAX_HEALTH * 0.4, PLAYER_MAX_HEALTH * 0.4,
		PLAYER_MAX_HEALTH * 0.4,  0, PLAYER_MAX_HEALTH * 0.4,
		PLAYER_MAX_HEALTH * 0.4, PLAYER_MAX_HEALTH * 0.4, PLAYER_MAX_HEALTH * 0.4 };
	Mask  m(glm::ivec2(1, 1), arr, glm::ivec2(3, 3));
	head.addComponent<tempo::ComponentAttack>();
	head.addComponent<tempo::ComponentWeapon>(m, (unsigned int)0);
	head.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);

	head.activate();

	auto& cs = head.getComponent<ComponentStage>();
	float height = cs.getHeight(pos);
	pos += -1 * f;

	for (int I = 1; I < len && cs.isNavigable(pos, height) ; I++)
	{

		anax::Entity seg = world.createEntity();

		seg.addComponent<tempo::ComponentAI>(MoveType::MOVE_SNAKE, false, false);
		seg.addComponent<tempo::ComponentStagePosition>(pos);
		seg.addComponent<tempo::ComponentStageRotation>(SOUTH);
		seg.addComponent<tempo::ComponentStageTranslation>();
		seg.addComponent<tempo::ComponentModel>("resources/materials/textures/player.png", glm::vec3(255, 200, 200), false, glm::vec2(4,4));
		seg.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
		seg.addComponent<tempo::ComponentHealth>(10);
		seg.addComponent<tempo::ComponentAttack>();
		seg.addComponent<tempo::ComponentWeapon>(m, (unsigned int)5);
		seg.addComponent<tempo::ComponentTeam>(Team::GOODGUYS);

		seg.activate();

		pos += -1 * f;
	}

	ai_index_top++;

	return head;
}

}  // namespace tempo
