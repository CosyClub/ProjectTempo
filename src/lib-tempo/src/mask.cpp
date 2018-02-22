#include <tempo/mask.hpp>

#include <iostream>

namespace tempo
{

Mask::Mask(int x, int y, float* data, int w, int h)
{
	mask = (float*) malloc(w * h * sizeof(float));
	memcpy(mask, data, w * h * sizeof(float));

	centre.x = x;
	centre.y = y;

	for (int I = 0; I < w; I++)
	{
		for(int J = 0; J < h; J++)
		{
			std::cout << mask[I + J * w] << std::endl;
		}
	}

	size.x = w;
	size.y = h;
}

Mask::Mask(glm::ivec2 centre, float* data, int w, int h)
{
	Mask(centre.x, centre.y, data, w, h);
}

Mask::Mask(const Mask& m)
{
	centre = m.centre;
	size = m.size;

	mask = (float*) malloc(m.size.x * m.size.y * sizeof(float));
	memcpy(mask, m.mask, m.size.x * m.size.y * sizeof(float));
}

Mask::Mask()
{
	centre = glm::ivec2(0,0);
	size = glm::ivec2(0,0);
	int* mask = NULL;
}

Mask::~Mask()
{
	free(mask);
}

// Mask& Mask::operator=(const Mask& m)
// {
// 	centre = m.centre;
// 	size = m.size;

// 	mask = (float*) malloc(m.size.x * m.size.y * sizeof(float));
// 	memcpy(mask, m.mask, m.size.x * m.size.y * sizeof(float));
// }

float Mask::get_value(glm::ivec2 offset)
{
	return Mask::get_value(offset.x, offset.y);
}

float Mask::get_value(int x, int y)
{
	glm::ivec2 pos = centre;
	pos.x += x;
	pos.y += y;
	std::cout << pos.x << ":" << pos.y << std::endl; //TODO remove
	if (pos.x <  0      ||
	    pos.y <  0      ||
	    pos.x >= size.x ||
	    pos.y >= size.y   )
	{
		return 0; // out of bounds
	}
	else
	{
		return mask[size.x * pos.y + pos.x]; //Maybe works?
	}
}

}
