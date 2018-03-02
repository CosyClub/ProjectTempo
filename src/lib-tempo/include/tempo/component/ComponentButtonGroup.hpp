#ifndef TEMPO_COMPONENT_BUTTON_GROUPS_HPP
#define TEMPO_COMPONENT_BUTTON_GROUPS_HPP

#include <anax/Component.hpp>

#include <deque>

#include <glm/vec2.hpp>

namespace tempo {

	struct button {
		glm::ivec2 pos;
		bool triggered = false;
		int combo_req;
	};

struct ComponentButtonGroup : anax::Component {

	std::deque<button> buttons;

	bool groupTriggered = false;

	ComponentButtonGroup(std::vector<glm::ivec2> positions);

};

} // namespace tempo

#endif
