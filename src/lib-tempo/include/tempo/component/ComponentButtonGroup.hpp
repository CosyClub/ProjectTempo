#ifndef TEMPO_COMPONENT_BUTTON_GROUPS_HPP
#define TEMPO_COMPONENT_BUTTON_GROUPS_HPP

#include <anax/Component.hpp>

#include <deque>

#include <glm/vec2.hpp>

namespace tempo
{
struct button {
	glm::ivec2 pos;
	int        combo_req;
	bool       triggered = false;
};

struct ComponentButtonGroup : anax::Component {
	// This is a deque so that it is possible to push_front
	std::deque<button> buttons;

	bool groupTriggered  = false;
	bool action_happened = false;

	ComponentButtonGroup(std::vector<glm::ivec2> positions);
};

}  // namespace tempo

#endif
