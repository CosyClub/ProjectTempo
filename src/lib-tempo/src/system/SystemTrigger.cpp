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

	for (auto &entity : entities) {
		// get deque of all buttons in the group
		auto &button_group = entity.getComponent<tempo::ComponentButtonGroup>();
		std::deque<button> &buttons = button_group.buttons;

		// search through each button in a group to see if a player location matches
		bool isGroupTriggered = true;

		for (int i = 0; i < buttons.size(); i++) {
			glm::ivec2 tempButtonPos = buttons[i].pos;

			bool is_in = false;

			for (int j = 0; j < playerPos.size(); j++) {
				if ((tempButtonPos.x == playerPos[j].x) && (tempButtonPos.y == playerPos[j].y)) {
					is_in = true;
					break;
				}
			}

			if (is_in) {
				buttons[i].triggered = true;
			} else {
				buttons[i].triggered = false;
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

	subSystemSpikes.updateSpikes(untriggerPos);
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
