#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/component/ComponentStage.hpp>
#include <tempo/system/SystemTrigger.hpp>

#include <glm/vec2.hpp>

namespace tempo
{
SystemTrigger::SystemTrigger(anax::World &world)
{
	subSystem = SubSystemGetPlayers();
	world.addSystem(subSystem);
	world.refresh();
}

void SystemTrigger::updateButtons(anax::World &world)
{
	// find all player locations in game
	playerPos = subSystem.getPlayers();

	// get all the button groups
	auto entities = getEntities();

	for (auto &entity : entities) {
		// get deque of all buttons in the group
		std::deque<button> &buttons = entity.getComponent<tempo::ComponentButtonGroup>().buttons;

		// search through each button in a group to see if a player location matches
		for (int i = 0; i < buttons.size(); i++) {
			glm::ivec2 tempButtonPos = buttons[i].pos;

			bool is_in = false;

			for (int j = 0; j < playerPos.size(); j++) {
				if ((tempButtonPos.x == playerPos[j].x) && (tempButtonPos.y == playerPos[j].y)) {
					is_in = true;
				}
			}

			if (is_in) {
				buttons[i].triggered = true;
			} else {
				buttons[i].triggered = false;
			}
		}
	}

	for (auto &entity : entities) {
		auto &button_group = entity.getComponent<tempo::ComponentButtonGroup>();
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

	if (currentPlayerPos.size() == 0) {
		printf("\n\nThere are no Players!!!!\n\n");
	}
	return currentPlayerPos;
}
}
