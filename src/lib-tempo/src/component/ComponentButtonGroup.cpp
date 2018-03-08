#include <tempo/component/ComponentButtonGroup.hpp>

namespace tempo
{
ComponentButtonGroup::ComponentButtonGroup(std::vector<glm::ivec2> positions,
                                           std::vector<glm::ivec2> wall_positions) :
																					 wall_positions(wall_positions)
{
	for (glm::ivec2 pos : positions) {
		button newbutton;
		buttons.push_front(newbutton);
		buttons[0].pos       = pos;
		buttons[0].triggered = false;
		buttons[0].combo_req = 0;
	}

}

}  // namespace tempo
