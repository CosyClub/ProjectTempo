#include <tempo/component/ComponentButtonGroup.hpp>
#include <tempo/component/ComponentStage.hpp>

#include <tempo/network/base.hpp>

namespace tempo
{
ComponentButtonGroup::ComponentButtonGroup(std::vector<glm::ivec2> positions,
                                           std::vector<glm::ivec2> wall_positions)
    : wall_positions(wall_positions)
{
	for (glm::ivec2 pos : positions) {
		button newbutton;
		buttons.push_front(newbutton);
		buttons[0].pos = pos;
		buttons[0].triggered = false;
		buttons[0].combo_req = 0;
	}
}

/////
// Required for networking
/////
ComponentButtonGroup::ComponentButtonGroup(sf::Packet p)
{
	sf::Uint32 size = 0;
	p >> size;
	for (int i = 0; i < static_cast<int>(size); i++) {
		button b;
		p >> b;
		buttons.push_front(b);
	}

	p >> groupTriggered;
	p >> action_happened;

	p >> size;
	for (int i = 0; i < static_cast<int>(size); i++) {
		glm::ivec2 v;
		p >> v;
		wall_positions.push_back(v);

		// TODO Make stage sync properly - this is a hack
		for (int i = 0; i < static_cast<int>(_global_stage.size()); i++) {
			if (groupTriggered && _global_stage[i].position == v) {
				_global_stage[i].height = 0.f;
			}
		}
	}
}

ComponentID ComponentButtonGroup::getId()
{
	return ComponentID::BUTTON_GROUP;
}

// // This is a deque so that it is possible to push_front
// std::deque<button>      buttons;
// std::vector<glm::ivec2> wall_positions;
//
// bool groupTriggered  = false;
// bool action_happened = false;

sf::Packet ComponentButtonGroup::dumpComponent()
{
	sf::Packet p;

	p << (sf::Uint32)buttons.size();
	for (int i = 0; i < static_cast<int>(buttons.size()); i++) {
		button b = buttons.back();
		p << b;
		buttons.pop_back();
		buttons.push_front(b);
	}

	p << groupTriggered;
	p << action_happened;  // TODO maybe move this to rendering component

	p << (sf::Uint32)wall_positions.size();
	for (int i = 0; i < static_cast<int>(wall_positions.size()); i++) {
		p << wall_positions[i];
	}

	return p;
}

sf::Packet& operator<<(sf::Packet& p, const button& c)
{
	return p << c.pos << c.combo_req << c.triggered;
}

sf::Packet& operator>>(sf::Packet& p, button& c)
{
	return p >> c.pos >> c.combo_req >> c.triggered;
}

}  // namespace tempo
