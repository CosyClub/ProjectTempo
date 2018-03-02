#include <tempo/system/SystemTrigger.hpp>
#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>

#include <glm/vec2.hpp>

namespace tempo{

	SystemTrigger::SystemTrigger(anax::World& world){
		subSystem = SubSystemGetPlayers();
		world.addSystem(subSystem);
		world.refresh();

	}


	void SystemTrigger::updateButtons(){

		playerPos = subSystem.getPlayers();

		auto entities = getEntities();

		for(auto& entity : entities){

			std::deque<button>& buttons = entity.getComponent<tempo::ComponentButtonGroup>().buttons;
			for(int i = 0; i < buttons.size(); i++){

				glm::ivec2 tempButtonPos = buttons[i].pos;

				bool is_in = false;

				for(int j = 0; j < playerPos.size(); j++){

					if((tempButtonPos.x == playerPos[j].x) && (tempButtonPos.y == playerPos[j].y)){
						is_in = true;
					}
				}

				if(is_in){
					printf("\nIs In: {%d,%d}", tempButtonPos.x,tempButtonPos.y);
					buttons[i].triggered = true;
				}
				else {
					buttons[i].triggered = false;
				}
			}
		}

	}

	std::vector<glm::ivec2> SubSystemGetPlayers::getPlayers(){

		std::vector<glm::ivec2> currentPlayerPos;

		auto entities = getEntities();

		for(auto& entity : entities){
			if(entity.hasComponent<tempo::ComponentPlayerLocal>() || entity.hasComponent<tempo::ComponentPlayerRemote>()){
				glm::ivec2 tempPlayerPos = entity.getComponent<tempo::ComponentStagePosition>().getOrigin();
				printf("\nPlayer Position: {%d,%d}\n",tempPlayerPos.x,tempPlayerPos.y);
				currentPlayerPos.push_back(tempPlayerPos);
			}
		}

		if(currentPlayerPos.size() == 0){
			printf("\n\nWTF getPlayers\n\n");
		}
		return currentPlayerPos;
	}



}
