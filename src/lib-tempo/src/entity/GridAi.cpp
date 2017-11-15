////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file GridAi.cpp
/// \author Jamie Terry
/// \date 2017/11/15
/// \brief Contains definition of Grid Ai system
////////////////////////////////////////////////////////////////////////////

#include <tempo/entity/GridAi.hpp>

namespace tempo{
	void SystemGridAi::update(){
		auto entities = getEntities();

		for(auto& entity : entities){
			auto& grid_motion = entity.getComponent<tempo::ComponentGridMotion>();

			int dir = rand() % 2; // between 0 and 1
			int amount = (rand() % 2) * 2 - 1; //-1 or 1
			if (dir) {
				grid_motion.moveBy(amount, 0);
			} else {
				grid_motion.moveBy(0, amount);
			}
		}
	}
}
