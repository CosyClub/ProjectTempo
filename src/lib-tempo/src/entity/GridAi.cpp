#include <tempo/entity/GridAi.hpp>

namespace tempo{
	void SystemGridAi::update(){
		auto entities = getEntities();

		for(auto& entity : entities){
			auto& grid_motion = entity.getComponent<tempo::ComponentGridMotion>();

			int dir = rand() % 2; // between 0 and 1
			int amount = (rand() % 2) * 2 - 1; //-1 or 1
			if (dir) {
				grid_motion.beginMovement(amount, 0);
			} else {
				grid_motion.beginMovement(0, amount);
			}
		}
	}
}
