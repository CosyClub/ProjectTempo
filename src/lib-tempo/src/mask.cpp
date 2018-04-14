#include <tempo/mask.hpp>

namespace tempo
{
Mask::Mask(glm::ivec2 centre, float *data, glm::ivec2 size)
{
	ctr = centre;
	sz  = size;

	for (int I = 0; I < sz.x * sz.y; I++) {
		mask.push_back(data[I]);
		if (data[I] != 0)
			positions.push_back(glm::vec2(I % sz.x, I / sz.x));
	}
}

Mask::Mask()
{
	ctr  = glm::ivec2(0, 0);
	sz   = glm::ivec2(0, 0);
	mask = std::vector<float>();
}

float Mask::get_value(glm::ivec2 offset)
{
	return Mask::get_value(offset.x, offset.y);
}

float Mask::get_value(int x, int y)
{
	glm::ivec2 pos = ctr;
	pos.x += x;
	pos.y += y;
	if (pos.x < 0 || pos.y < 0 || pos.x >= sz.x || pos.y >= sz.y) {
		return 0;  // out of bounds
	} else {
		return mask[sz.x * pos.y + pos.x];  // Maybe works?
	}
}

sf::Packet &operator<<(sf::Packet &packet, const tempo::Mask &m)
{
	packet << m.ctr;
	packet << m.sz;

	for (int I = 0; I < m.sz.x * m.sz.y; I++) {
		packet << m.mask[I];
	}

	uint32_t size = m.positions.size();
	packet << size;

	for (uint32_t I = 0; I < size; I++) {
		packet << m.positions[I];
	}

	return packet;
}

sf::Packet &operator>>(sf::Packet &packet, tempo::Mask &m)
{
	packet >> m.ctr;
	packet >> m.sz;

	m.mask = std::vector<float>();

	for (int I = 0; I < m.sz.x * m.sz.y; I++) {
		float tmp;
		packet >> tmp;
		m.mask.push_back(tmp);
	}

	uint32_t size;
	packet >> size;

	m.positions = std::vector<glm::ivec2>();

	for (uint32_t I = 0; I < size; I++) {
		glm::ivec2 tmp_vec;
		packet >> tmp_vec;
		m.positions.push_back(tmp_vec);
	}

	return packet;
}
}
