#include <tempo/component/ComponentButtonGroup.hpp>

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
		buttons[0].pos       = pos;
		buttons[0].triggered = false;
		buttons[0].combo_req = 0;
	}
}

/////
// Required for networking
/////
ComponentButtonGroup::ComponentButtonGroup(sf::Packet p)
{
	printf("MAKING A FUCKING BUTTON!!!!!!\n\n\n\n\n\n");
	sf::Uint32 size = 0;
	p >> size;
	for (int i = 0; i < size; i++) {
		button b;
		p >> b;
		buttons.push_front(b);
	}

	p >> size;
	for (int i = 0; i < size; i++) {
		glm::ivec2 v;
		p >> v;
		wall_positions.push_back(v);
	}

	p >> groupTriggered;
	p >> action_happened;
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
	printf("MAKING A FUCKING BUTTON PACKET!!!!!!\n\n\n\n\n\n");
	sf::Packet p;

	p << (sf::Uint32) buttons.size();
	for (int i = 0; i < buttons.size(); i++) {
		button b = buttons.back();
		p << b;
		buttons.pop_back();
		buttons.push_front(b);
	}

	p << (sf::Uint32) wall_positions.size();
	for (int i = 0; i < wall_positions.size(); i++) {
		p << wall_positions[i];
	}

	p << groupTriggered;
	p << action_happened;

	return p;
}

sf::Packet &operator<<(sf::Packet &p, const button &c)
{
	return p << c.pos << c.combo_req << c.triggered;
}

sf::Packet &operator>>(sf::Packet &p, button &c)
{
	return p >> c.pos >> c.combo_req >> c.triggered;
}

}  // namespace tempo
