#ifndef TEMPO_MASK_HPP
#define TEMPO_MASK_HPP

#include <tempo/config.hpp>
#include <glm/glm.hpp>

#include <string.h>

namespace tempo
{

class Mask
{
	public:

	Mask(glm::ivec2 centre, float* data, int w, int h);
	Mask(int x, int y, float* data, int w, int h);
	Mask(const Mask& m);
	Mask();

	~Mask();

	Mask& operator=(const Mask& m);

	float get_value(glm::ivec2 offset);
	float get_value(int x, int y);

	glm::ivec2 centre;
	glm::ivec2 size;

	float* mask;
};

}

#endif
