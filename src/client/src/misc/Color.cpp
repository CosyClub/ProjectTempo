#include <client/misc/Color.hpp>

namespace client {

const std::vector<palette> palettes =
	{
		{
			irr::video::SColor(255, 212,   0, 120), // floor1
			irr::video::SColor(255,  61, 241,   0), // floor2
			irr::video::SColor(255,   0, 152,  61), // attack
			irr::video::SColor(255, 255,  49,   0)  // light
		},
		{
			irr::video::SColor(255, 106,  31, 148), // floor1
			irr::video::SColor(255, 221, 221,  36), // floor2
			irr::video::SColor(255, 114, 199,  32), // attack
			irr::video::SColor(255, 195,  32,  87)  // light
		},
		{
			irr::video::SColor(255,  24,  24, 124), // floor1
			irr::video::SColor(255, 220, 171,  26), // floor2
			irr::video::SColor(255, 211,  74,   0), // attack
			irr::video::SColor(255,   0, 181, 126)  // light
		},
		{
			irr::video::SColor(255,  17,  90, 233), // floor1
			irr::video::SColor(255, 255, 140,   0), // floor2
			irr::video::SColor(255, 255,  19,   0), // attack
			irr::video::SColor(255,   0, 239,  42)  // light
		},
		{
			irr::video::SColor(255,   2, 215, 215), // floor1
			irr::video::SColor(255, 255, 117,   3), // floor2
			irr::video::SColor(255, 242,   3,  91), // attack
			irr::video::SColor(255, 129, 245,   3)  // light
		},
		{
			irr::video::SColor(255, 103, 235,  18), // floor1
			irr::video::SColor(255, 241,  18,  74), // floor2
			irr::video::SColor(255, 255, 140,  20), // attack
			irr::video::SColor(255,  21, 150, 193)  // light
		},
		{
			irr::video::SColor(255, 197, 237,   1), // floor1
			irr::video::SColor(255, 160,   1, 160), // floor2
			irr::video::SColor(255,   1, 160, 120), // attack
			irr::video::SColor(255, 246,  92,   1)  // light
		},
		{
			irr::video::SColor(255, 255, 232,   0), // floor1
			irr::video::SColor(255, 108,  14, 234), // floor2
			irr::video::SColor(255,  12, 124, 232), // attack
			irr::video::SColor(255, 255, 151,   0)  // light
		},
		{
			irr::video::SColor(255, 255, 129,   0), // floor1
			irr::video::SColor(255,   5, 180, 230), // floor2
			irr::video::SColor(255, 245,   0, 103), // attack
			irr::video::SColor(255, 141, 249,   0)  // light
		},
		{
			irr::video::SColor(255, 221,  45,  36), // floor1
			irr::video::SColor(255,  28, 172,  46), // floor2
			irr::video::SColor(255, 112,  30, 147), // attack
			irr::video::SColor(255, 255, 129,   0)  // light
		}
	};
palette curr_pallette = palettes[0];

int next_palette(int old_colour)
{
	int colour_index = rand() % 10;
	while (colour_index == old_colour)
		colour_index = rand() % 10;
	curr_pallette = palettes[colour_index];
	return colour_index;
}

} // namespace client
