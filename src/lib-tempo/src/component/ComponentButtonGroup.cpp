#include <tempo/component/ComponentButtonGroup.hpp>
#include <tempo/component/ComponentStage.hpp>

#include <tempo/network/base.hpp>

namespace tempo
{
ComponentButtonGroup::ComponentButtonGroup(std::vector<glm::ivec2> positions,
                                           std::vector<glm::ivec2> wall_positions,
                                           std::vector<glm::ivec2> spike_positions,
	                                   glm::ivec2              respawn_pos,
                                           glm::ivec2              prev,
                                           glm::ivec2              next,
                                           bool	                   triggerable,
                                           int                     ID)
    : wall_positions(wall_positions), spike_positions(spike_positions), respawn_pos(respawn_pos), prev(prev), next(next), groupTriggerable(triggerable), rhythmID(ID)
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
	for (uint32_t i = 0; i < size; i++) {
		button b;
		p >> b;
		buttons.push_front(b);
	}

	p >> groupTriggered;
	p >> action_happened;
	p >> groupTriggerable;
	p >> blocked;
	
	p >> size;
	for (uint32_t i = 0; i < size; i++) {
		glm::ivec2 v;
		p >> v;
		wall_positions.push_back(v);

		// TODO Make stage sync properly - this is a hack
		for (auto &a : _global_stage)
		{
			if (groupTriggered && a.second.position == v)
			{
				a.second.height = 0.f;
			}
		}
	}

	p >> size;
	for (uint32_t i = 0; i < size; i++) {
		glm::ivec2 v;
		p >> v;
		spike_positions.push_back(v);
	}

	glm::ivec2 v1, v2, respawn;
	
	p >> respawn >> v1 >> v2 >> rhythmID;
	
	respawn_pos = respawn;
	prev = v1;
	next = v2;
}

ComponentID ComponentButtonGroup::getId()
{
	return ComponentID::BUTTON_GROUP;
}

sf::Packet ComponentButtonGroup::dumpComponent()
{
	sf::Packet p;

	p << (sf::Uint32) buttons.size();
	for (uint32_t i = 0; i < buttons.size(); i++) {
		button b = buttons.back();
		p << b;
		buttons.pop_back();
		buttons.push_front(b);
	}

	p << groupTriggered;
	p << action_happened; // TODO maybe move this to rendering component
	p << groupTriggerable;
	p << blocked;

	p << (sf::Uint32) wall_positions.size();
	for (uint32_t i = 0; i < wall_positions.size(); i++) {
		p << wall_positions[i];
	}

	p << (sf::Uint32) spike_positions.size();
	for (uint32_t i = 0; i < spike_positions.size(); i++) {
		p << spike_positions[i];
	}

	p << respawn_pos << prev << next << rhythmID;

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
