#ifndef TEMPO_COMPONENT_BUTTON_GROUPS_HPP
#define TEMPO_COMPONENT_BUTTON_GROUPS_HPP

#include <anax/Component.hpp>

#include <deque>

#include <glm/vec2.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{
struct button {
	glm::ivec2 pos;
	int        combo_req;
	bool       triggered = false;
};

struct ComponentButtonGroup : anax::Component , NetworkedComponent {
	// This is a deque so that it is possible to push_front
	std::deque<button>      buttons;
	std::vector<glm::ivec2> wall_positions;
	std::vector<glm::ivec2> spike_positions;

	glm::ivec2 prev = {-1,-1};
	glm::ivec2 next = {-1,-1};

	bool groupTriggered   = false;
	bool action_happened  = false; // TODO maybe move this to rendering component
	bool groupTriggerable = false;
	bool blocked		  = false;

	int rhythmID;

	ComponentButtonGroup(std::vector<glm::ivec2> button_positions,
	                     std::vector<glm::ivec2> wall_positions,
						 std::vector<glm::ivec2> spike_positions,
						 glm::ivec2              prev,
					     glm::ivec2              next,
						 bool					 triggerable,
						 int					 ID);

	/////
	// Required for networking
	/////
	ComponentButtonGroup(sf::Packet p);
	ComponentID getId();
	sf::Packet  dumpComponent();
};

sf::Packet &operator<<(sf::Packet &p, const button &c);
sf::Packet &operator>>(sf::Packet &p, button &c);

}  // namespace tempo

#endif
