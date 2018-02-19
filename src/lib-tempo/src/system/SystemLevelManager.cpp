#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/system/SystemLevelManager.hpp>

#include <glm/vec2.hpp>

namespace tempo{
	// SystemLevelManager
	SystemLevelManager::SystemLevelManager(anax::World& world, int size)
		: tile_heights(size, std::vector<float>(size)) {
		world.addSystem(this->grid_positions);
		for(int i = 0; i < size; i++){
			for(int j = 0; j< size; j++){
				tile_heights[i][j] = NO_TILE;
			}
		}
	}

	SystemLevelManager::SystemLevelManager(anax::World& world,
	                                       const char* heightMap,
	                                       const char* zoneMap)
		: tile_heights(100, std::vector<float>(100)),
		  player_spawn_zone(100*100) {
		world.addSystem(this->grid_positions);
		loadLevel(heightMap);
		loadZones(zoneMap);
	}

	bool SystemLevelManager::existsTile(glm::vec2 position) {
		return existsTile(position.x, position.y);
 	}

	bool SystemLevelManager::existsTile(int x, int y) {
		if( x < 0  || x >= tile_heights.size() ||
	    	y < 0  || y >= tile_heights[0].size() )
			return false;

		return tile_heights[x][y] != NO_TILE;
 	}

	void SystemLevelManager::deleteTile(glm::vec2 position) {
		tile_heights[position.x][position.y] = NO_TILE;
	}

	void SystemLevelManager::createTile(glm::vec2 position) {
		tile_heights[position.x][position.y] = 0;
	}

	void SystemLevelManager::setHeight(float height, glm::vec2 position) {
		if (existsTile(position)) {
			tile_heights[position.x][position.y] = height;
		} else {
			std::cout <<" Can't set height to non-existent tile";
		}
	}

	void SystemLevelManager::setHeight(float height, glm::vec2 position, int width, int length) {
		for(int i = position.x; i < width+position.x; i++){
			for(int j = position.y; j < length+position.y; j++){
				this->tile_heights[i][j] = height;
			}
		}
	}

	float SystemLevelManager::getHeight(int x, int y) {
		if (existsTile(x, y)) {
			return tile_heights[x][y];
		} else {
			return NO_TILE;
			std::cout <<" Can't get height of non-existent tile";
		}
	}

	void SystemLevelManager::loadLevel(const char* fileName) {

		SDL_Surface* level = SDL_LoadBMP(fileName);

		// Clear out any existing tiles
		for(int x = 0; x < tile_heights.size(); ++x){
			for(int y = 0; y < tile_heights[x].size(); ++y){
				tile_heights[x][y] = NO_TILE;
			}
		}

		// Load the new tiles
		for (int y = 0; y < level->h; y++) {
			for (int x = 0; x < level->w; x++) {

				int bpp = level->format->BytesPerPixel;
				/* Here p is the address to the pixel we want to retrieve */
				Uint8 *p = (Uint8 *)level->pixels + y * level->pitch + x * bpp;
				uint32_t pixel = 0;

				pixel = *p;

				if (pixel > 0) {
					int height = (int) (pixel - 127) / 25.6;
					this->tile_heights[x][y] = height;
				}
			}
		}
	}

	void SystemLevelManager::loadZones(const char* fileName) {

		SDL_Surface* level = SDL_LoadBMP(fileName);

		for (int y = 0; y < level->h; y++) {
			for (int x = 0; x < level->w; x++) {

				int bpp = level->format->BytesPerPixel;
				/* Here p is the address to the pixel we want to retrieve */
				Uint8 *p = (Uint8 *)level->pixels + y * level->pitch + x * bpp;
				uint32_t pixel = 0;

				pixel = p[0] | p[1] << 8 | p[2] << 16;

				//std::cout << (int) p[0]<<" "<<(int) p[1]<<" "<< (int) p[2]<<std::endl;

				if (p[1] > 250) {
					this->player_spawn_zone[spawn_zones] = {x,y};
					spawn_zones++;
				}
			}
		}

	}

	glm::vec2 SystemLevelManager::spawn() {
		srand (time(NULL));
		uint32_t random_location = rand() % spawn_zones;
		return player_spawn_zone[random_location];
	}

	glm::vec2 SystemLevelManager::getWorldSize(){
		glm::vec2 result;
		result.x = tile_heights.size();
		result.y = tile_heights[0].size();
		return result;
	}

	void SystemLevelManager::update(float dt){
		auto entities = getEntities();

		for (auto& entity : entities) {
			auto& pos = entity.getComponent<ComponentStagePosition>();
			auto& gm = entity.getComponent<ComponentStageTranslation>();

			{
			//if(gm.getCurrentMovement() != glm::vec2(0, 0)){
			//	gm.setMotionProgress(gm.getMotionProgress() + (dt * gm.movement_speed) / gm.getCurrentMovement().length());
			//	if(gm.getMotionProgress() >= 1){
			//		pos.setPosition(pos.getPosition() + gm.getCurrentMovement());
			//		gm.setDelta({0,0});
			//		gm.setMotionProgress(0);
			//		gm.setTargetLocked(false);
			//	}
			//}

			//glm::vec2 target_tile = pos.getPosition() + gm.getCurrentMovement();
			//float current_height = this->getHeight(pos.getPosition());
			//float target_height  = this->getHeight(target_tile);
			//float delta_height   = target_height - current_height;

			// Work out if entity is allowed to go to its current target
			// This depends on:
			// - Delta height between tiles
			// - If target tile is already occupied by another entity
			//if(gm.getMotionProgress() >= 0.5f && !gm.isMovementLocked()){
			//	bool can_make_move = true;

			//	// Check height difference
			//	if(abs(delta_height) > gm.max_jump_height){
			//		can_make_move = false;
			//	}

			//	if(!existsTile(target_tile)){
			//		can_make_move = false;
			//	}

			//	// :TODO:OPT: this might be quite slow - for each entity
			//	// check all others, might want to build quadtree / 2d boolean array
			//	// -> but we only do this check once per move - so might be okay - PROFILE!
			//	for(auto& collision_candidate : grid_positions.getEntities()){
			//		auto& pos_candidate = collision_candidate.getComponent<ComponentStagePosition>();

			//		if(pos_candidate.isEthereal() && pos.isEthereal()){ continue; }

			//		glm::vec2 candidate_delta = pos_candidate.getPosition() - target_tile;
			//		//printf("Found candidate: %i, %i\n", candidate_delta.x, candidate_delta.y);

			//		if (pos.getTileMask().isCollidingWith(pos_candidate.getTileMask(), candidate_delta)) {
			//			can_make_move = false;
			//		}
			//	}

			//	// :TODO: If multiple entities try to jump into same tile simultaneously
			//	// should they both bounce back? Should first one moving get priority?

			//	if(!can_make_move){
			//		// Then entity should bounce back to where it came from
			//		pos.setPosition(pos.getPosition() + gm.getCurrentMovement());
			//		gm.setDelta(-gm.getCurrentMovement());
			//		// Swap motion progress around so we don't have a obvious jump in motion
			//		gm.setMotionProgress(1 - gm.getMotionProgress());
			//		gm.setTargetLocked(true);
			//	} else {
			//		// :TODO: claim the target tile with level manager
			//		gm.setTargetLocked(true);
			}
		}

	}
}
