#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/component/ComponentStage.hpp>
#include <tempo/system/SystemTrigger.hpp>

//#include <algorithm>
#include <iostream>

namespace tempo
{

SystemTrigger::SystemTrigger(anax::World &world)
{
	subSystemPlayers = SubSystemGetPlayers();
	subSystemSpikes = SubSystemUpdateSpikes();
	world.addSystem(subSystemPlayers);
	world.addSystem(subSystemSpikes);
	world.refresh();
}

void SystemTrigger::updateButtons(anax::World &world)
{
	// find all player locations in game
	playerPos = subSystemPlayers.getPlayers();

	untriggerPos.clear();

	// get all the button groups
	auto entities = getEntities();

	for (int i = 0; i < entities.size(); i++) {

		auto &entity = entities[i];
		// get deque of all buttons in the group
		auto &button_group = entity.getComponent<tempo::ComponentButtonGroup>();
		std::deque<button> &buttons = button_group.buttons;

		if (button_group.next.x == -1 && button_group.next.y == -1 && button_group.prev.x == -1 && button_group.prev.y == -1) { // rythm-less buttons

			// search through each button in a group to see if a player location matches
			bool isGroupTriggered = true;

			for (int j = 0; j < buttons.size(); j++) {
				glm::ivec2 tempButtonPos = buttons[j].pos;

				bool is_in = false;

				for (int k = 0; k < playerPos.size(); k++) {
					if ((tempButtonPos.x == playerPos[k].x) && (tempButtonPos.y == playerPos[k].y)) {
						is_in = true;
						break;
					}
				}

				if (is_in) {
					buttons[j].triggered = true;
				}
				else {
					buttons[j].triggered = false;
					isGroupTriggered = false;
				}
			}
			button_group.groupTriggered |= isGroupTriggered;

			if (isGroupTriggered) {
				for (glm::ivec2 pos : button_group.spike_positions) {
					untriggerPos.push_back(pos);
				}
			}

			//If spike buttons then never stay activated
			if (button_group.spike_positions.size() > 0) {
				button_group.groupTriggered = false;
			}

			if (button_group.groupTriggered == true && !button_group.action_happened) {
				button_group.action_happened = true;
				for (auto &entity : world.getEntities()) {
					if (entity.hasComponent<tempo::ComponentStage>()) {
						auto &component_stage = entity.getComponent<tempo::ComponentStage>();
						component_stage.setHeight(button_group.wall_positions, 0.f);
					}
				}
			}
		}

		else { //rythm-based buttons

			// search through each button in a group to see if a player location matches
			bool isGroupTriggered = true;

			for (int j = 0; j < buttons.size(); j++) {
				glm::ivec2 tempButtonPos = buttons[j].pos;

				bool is_in = false;

				for (int k = 0; k < playerPos.size(); k++) {
					if ((tempButtonPos.x == playerPos[k].x) && (tempButtonPos.y == playerPos[k].y)) {
						is_in = true;
						break;
					}
				}

				if (is_in && button_group.groupTriggerable) {
					buttons[j].triggered = true;
				}


				else if (is_in && !button_group.groupTriggerable) {
					resetButtons(entities);
					isGroupTriggered = false;
				}

				else if (!is_in && button_group.groupTriggerable) {
					resetButtons(entities);
					isGroupTriggered = false;
				}

				else {
					buttons[j].triggered = false;
					isGroupTriggered = false;
				}
			}
			button_group.groupTriggered |= isGroupTriggered;

			if (button_group.groupTriggered) {
				if (button_group.next.x == -1 && button_group.next.y == -1) { //if last button in sequence
					
					//If spike buttons then never stay activated
					if (button_group.spike_positions.size() > 0) {
						button_group.groupTriggered = false;
					}

					if (button_group.groupTriggered == true && !button_group.action_happened) {
						button_group.action_happened = true;
						for (auto &entity : world.getEntities()) {
							if (entity.hasComponent<tempo::ComponentStage>()) {
								auto &component_stage = entity.getComponent<tempo::ComponentStage>();
								component_stage.setHeight(button_group.wall_positions, 0.f);
							}
						}
					}
				}

				else {
					button_group.groupTriggerable = false;
					entities[i + 1].getComponent<tempo::ComponentButtonGroup>().groupTriggerable = true; //next button can be pressed
				}
			}

		}

	}

	subSystemSpikes.updateSpikes(untriggerPos);
}

void SystemTrigger::resetButtons(std::vector<anax::Entity> entities) {

	printf("\n\n Reset Called! \n\n");

	for (int i = 0; i < entities.size(); i ++) {

		auto &entity = entities[i];
		// get deque of all buttons in the group
		auto &button_group = entity.getComponent<tempo::ComponentButtonGroup>();
		std::deque<button> &buttons = button_group.buttons;

		if (button_group.next.x == -1 && button_group.next.y == -1 && button_group.prev.x == -1 && button_group.prev.y == -1) { // rythm-less buttons
			continue;
		}

		else {
			if (button_group.prev.x == -1 && button_group.prev.y == -1) {
				button_group.groupTriggerable = true;
				button_group.groupTriggered = false;
			}
			else {
				button_group.groupTriggerable = false;
				button_group.groupTriggered = false;
			}
		}
	}

}

std::vector<glm::ivec2> SubSystemGetPlayers::getPlayers()
{
	std::vector<glm::ivec2> currentPlayerPos;

	auto entities = getEntities();

	// Get all the players and save their locations
	for (auto &entity : entities) {
		if (entity.hasComponent<tempo::ComponentPlayerLocal>()
		    || entity.hasComponent<tempo::ComponentPlayerRemote>()) {
			glm::ivec2 tempPlayerPos =
			  entity.getComponent<tempo::ComponentStagePosition>().getOrigin();
			currentPlayerPos.push_back(tempPlayerPos);
		}
	}

	return currentPlayerPos;
}

void SubSystemUpdateSpikes::updateSpikes(std::vector<glm::ivec2> untriggerPos)
{
	auto entities = getEntities();

	for (auto &entity : entities) {

		auto &comp = entity.getComponent<ComponentSpikes>();
		std::vector<glm::ivec2> spikes = comp.spike_positions;

		bool subset = true;

		for (glm::ivec2 pos : spikes) {

			if (!(std::find(untriggerPos.begin(), untriggerPos.end(), pos) != untriggerPos.end())) {
				subset = false;
			}
		}

		if (subset) {
			comp.isTriggered = false;
		}
		else {
			comp.isTriggered = true;
		}

	}
}
}
