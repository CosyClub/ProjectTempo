#include <tempo/system/SystemTrigger.hpp>
#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>

#include <glm/vec2.hpp>

namespace tempo{

	SystemTrigger::SystemTrigger(anax::World& world){

		printf("System Trigger activated\n");

		subSystem = SubSystemGetPlayers();
		world.addSystem(subSystem);
		world.refresh();
		printf("\n\n\nTrigger Line 16\n\n\n");

	}


	void SystemTrigger::updateButtons(){

		playerPos = subSystem.getPlayers();

		auto entities = getEntities();

		if(playerPos.size() == 0){
			printf("\n\n\nPlayerpos = 0\n\n\n");
		}

		for(auto& entity : entities){

			std::deque<button> buttons = entity.getComponent<tempo::ComponentButtonGroup>().buttons;
			printf("\n\n\nTrigger Line 33\n\n\n");
			for(int i = 0; i < buttons.size(); i++){
				printf("\n\n\nTrigger Line 35\n\n\n");
				glm::ivec2 tempButtonPos = buttons[i].pos;
				printf("\n\n\nTrigger Line 37\n\n\n");
				bool is_in = false;
				printf("\n\n\nTrigger Line 39\n\n\n");
				for(int j = 0; j < playerPos.size(); j++){
					printf("\n\n\nTrigger Line 41\n\n\n");
					if((tempButtonPos.x == playerPos[j].x) && (tempButtonPos.y == playerPos[j].y)){
						printf("\n\n\nIS_IN\n\n\n");
						is_in = true;
					}
				}

				printf("\n\n\nTrigger Line 48\n\n\n");

				if(is_in){
					printf("\nIs In: {%d,%d}", tempButtonPos.x,tempButtonPos.y);
					buttons[i].triggered = true;
				}
				else{
					printf("\n\n\nNot In\n\n\n");
				}
			}
		}

		printf("\n\n\nUPDATE BUTTONS HAS FINISHED\n\n\n");

	}

	std::vector<glm::ivec2> SubSystemGetPlayers::getPlayers(){

		std::vector<glm::ivec2> currentPlayerPos;

		auto entities = getEntities();

		printf("entities size = %d", entities.size());
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
		printf("\n\n\nTrigger Line 76\n\n\n");
		return currentPlayerPos;
	}



}
