#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/component/ComponentStage.hpp>
#include <tempo/component/ComponentRespawn.hpp>
#include <tempo/system/SystemTrigger.hpp>

#include <iostream>

namespace tempo
{

SystemTrigger::SystemTrigger(anax::World& world)
{
	subSystemPlayers = SubSystemGetPlayers();
	subSystemSpikes = SubSystemUpdateSpikes();
	world.addSystem(subSystemPlayers);
	world.addSystem(subSystemSpikes);
	world.refresh();
}

void SystemTrigger::updateButtons(anax::World& world)
{
	//Sets to keep track of which rhythm Button groups need resetting or blocking after each update
	std::set<int> rhythmID_resets;
	std::set<int> rhythmID_blocks;

	//this stops the overwrite when making nextButton Triggerable
	bool skipNext = false;

	// find all player locations in game
	std::vector<anax::Entity> players = subSystemPlayers.getPlayers();
	untriggerPos.clear();

	// get all the button groups
	auto& entities = getEntities();

	for (uint32_t i = 0; i < entities.size(); i++) {

		auto &entity = entities[i];
		// get deque of all buttons in the group
		auto& button_group = entity.getComponent<tempo::ComponentButtonGroup>();
		std::deque<button>& buttons = button_group.buttons;
		glm::ivec2 spawn_loc = button_group.respawn_pos;

		// RHYTHMLESS BUTTONS

		if (button_group.rhythmID == 0) {

			// search through each button in a group to see if a player location matches
			bool isGroupTriggered = true;

			for (uint32_t j = 0; j < buttons.size(); j++) {
				glm::ivec2 tempButtonPos = buttons[j].pos;

				bool is_in = false;

				for (uint32_t k = 0; k < players.size(); k++) {
					int x = players[k].getComponent<tempo::ComponentStagePosition>().getOrigin().x;
					int y = players[k].getComponent<tempo::ComponentStagePosition>().getOrigin().y;
					if ((tempButtonPos.x == x) && (tempButtonPos.y == y)) {
						is_in = true;
						players[k].getComponent<tempo::ComponentRespawn>().spawn_location = spawn_loc;
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

		//RHYTHM-BASED BUTTONS

		else {

			//ignore if already completed
			if (button_group.action_happened) {
				continue;
			}

			if (skipNext) {
				skipNext = false;
				continue;
			}

			// search through each button in a group to see if a player location matches
			bool isGroupTriggered = true;

			for (uint32_t j = 0; j < buttons.size(); j++) {
				glm::ivec2 tempButtonPos = buttons[j].pos;

				bool is_in = false;

				for (uint32_t k = 0; k < players.size(); k++) {
					int x = players[k].getComponent<tempo::ComponentStagePosition>().getOrigin().x;
					int y = players[k].getComponent<tempo::ComponentStagePosition>().getOrigin().y;
					if ((tempButtonPos.x == x) && (tempButtonPos.y == y)) {
						is_in = true;
						players[k].getComponent<tempo::ComponentRespawn>().spawn_location = spawn_loc;
						break;
					}
				}

				if (is_in) {
					if (button_group.groupTriggerable) {
						buttons[j].triggered = true;
					}

					else {
						rhythmID_blocks.insert(button_group.rhythmID);
						isGroupTriggered = false;
					}
				}

				else {

					if (button_group.groupTriggerable) {

						rhythmID_resets.insert(button_group.rhythmID);

						buttons[j].triggered = false;
						isGroupTriggered = false;
					}

					else {

						//If first button is blocked, reset
						if (button_group.prev.x == -1 && button_group.prev.y == -1 && button_group.blocked) {
							rhythmID_resets.insert(button_group.rhythmID);
						}

						isGroupTriggered = false;
					}
				}

			}

			button_group.groupTriggered |= isGroupTriggered;

			//Set next button to triggerable
			if (button_group.groupTriggered && button_group.groupTriggerable) {
				button_group.groupTriggerable = false;
				if (!(button_group.next.x == -1 && button_group.next.y == -1)) {

					auto &entityNext = entities[i + 1];
					// get deque of all buttons in the group
					auto &button_groupNext = entityNext.getComponent<tempo::ComponentButtonGroup>();

					button_groupNext.groupTriggerable = true;
					skipNext = true;
				}
			}

			//If this is the last button and is triggered, trigger action and set all previous buttons accordingly
			if (button_group.groupTriggered && !button_group.action_happened && button_group.next.x == -1 && button_group.next.y == -1) {
				button_group.action_happened = true;

				for (auto& entity : entities) {

					auto &tempGroup = entity.getComponent<tempo::ComponentButtonGroup>();
					int rhythmID = tempGroup.rhythmID;

					if (rhythmID != button_group.rhythmID) {
						continue;
					}

					else {
						tempGroup.groupTriggered = true;
						tempGroup.action_happened = true;
					}
				}


				//wall drop
				for (auto &entity : world.getEntities()) {
					if (entity.hasComponent<tempo::ComponentStage>()) {
						auto &component_stage = entity.getComponent<tempo::ComponentStage>();
						component_stage.setHeight(button_group.wall_positions, 0.f);
					}
				}
			}

		}

	}

	//reset all groups that need resetting
	resetButtons(rhythmID_resets);

	//block all groups that need blocking
	for (int id : rhythmID_blocks) {
		blockButtons(id);
	}


	//Drop spikes where buttons have been triggered
	subSystemSpikes.updateSpikes(untriggerPos);
}

void SystemTrigger::syncFloorWithButtons() {

	auto& entities = getEntities();
	auto &stage = entities[0].getComponent<tempo::ComponentStage>();

	for (uint32_t i = 0; i < entities.size(); i++) {
		auto &entity = entities[i];
		// get deque of all buttons in the group
		auto &button_group = entity.getComponent<tempo::ComponentButtonGroup>();
		if (button_group.groupTriggered) {
			for (glm::ivec2 pos : button_group.wall_positions)
				stage.setHeight(pos, 0);
		}
	}

}
void SystemTrigger::resetButtons(std::set<int>& rhythmID_resets) {

	auto& entities = getEntities();

	for (uint32_t i = 0; i < entities.size(); i++) {
		auto &entity = entities[i];
		// get deque of all buttons in the group
		auto &button_group = entity.getComponent<tempo::ComponentButtonGroup>();
		std::deque<button> &buttons = button_group.buttons;

		for (uint32_t j = 0; j < buttons.size(); j++) {

			if (rhythmID_resets.find(button_group.rhythmID) == rhythmID_resets.end()) {
				continue;
			}

			button_group.blocked = false;

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

void SystemTrigger::blockButtons(int rhythmID) {

	auto& entities = getEntities();

	for (uint32_t i = 0; i < entities.size(); i++) {
		auto &entity = entities[i];
		// get deque of all buttons in the group
		auto &button_group = entity.getComponent<tempo::ComponentButtonGroup>();
		std::deque<button> &buttons = button_group.buttons;

		for (uint32_t j = 0; j < buttons.size(); j++) {

			if (button_group.rhythmID != rhythmID) {
				continue;
			}

			else {
				button_group.groupTriggerable = false;
				button_group.groupTriggered = false;
				button_group.blocked = true;
			}

		}
	}

}

std::vector<anax::Entity> SubSystemGetPlayers::getPlayers()
{
	std::vector<anax::Entity> currentPlayers;

	auto& entities = getEntities();

	// Get all the players and save their locations
	for (auto& entity : entities) {
		if (entity.hasComponent<tempo::ComponentPlayerLocal>() ||
		    entity.hasComponent<tempo::ComponentPlayerRemote>()) {
			currentPlayers.push_back(entity);
		}
	}

	return currentPlayers;
}

void SubSystemUpdateSpikes::updateSpikes(std::vector<glm::ivec2> untriggerPos)
{
	auto& entities = getEntities();

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
