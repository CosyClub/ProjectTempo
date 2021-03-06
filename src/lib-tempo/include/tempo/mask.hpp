#ifndef TEMPO_MASK_HPP
#define TEMPO_MASK_HPP

#include <tempo/config.hpp>
#include <tempo/network/base.hpp>

#include <glm/glm.hpp>

#include <SFML/Network.hpp>

namespace tempo
{
class Mask
{
   public:
	///< \brief The center of the mask
	glm::ivec2              ctr;

	///< \brief The size of the mask
	glm::ivec2              sz;

	///< \brief Floating point value associated with each tile of the mask
	std::vector<float>      mask;

	///< \brief List of positions covered by the mask
	std::vector<glm::ivec2> positions;

	Mask();
	Mask(glm::ivec2 centre, float *data, glm::ivec2 size);

	float get_value(glm::ivec2 offset);
	float get_value(int x, int y);
};

sf::Packet &operator<<(sf::Packet &packet, const tempo::Mask &m);
sf::Packet &operator>>(sf::Packet &packet, tempo::Mask &m);
}

#endif
