#ifndef TEMPO_COMPONENT_STAGE_HPP
#define TEMPO_COMPONENT_STAGE_HPP

#include <anax/Component.hpp>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

#include <tuple>

namespace tempo {

	// This implementation may change without notice
	typedef std::vector<std::tuple<glm::ivec2, float>> stage_tiles;

	// An entity that is bound by the world stage
	struct ComponentStage : anax::Component {
		private:
			stage_tiles tiles;
		public:
			// Constructor requires stage file
			ComponentStage(const char* stage_file);
	
			// Gets a list of 
			stage_tiles getHeights();

			// TODO: better matrix for ZZ, ZZ, float 
			// Get height at
			float getHeight(glm::ivec2 position) {
				for (int i = 0; i < tiles.size(); i++) {
					std::tuple<glm::ivec2, float> tile = tiles[i];
					glm::ivec2 pos = std::get<0>(tile);
					if (pos == position) {
						return std::get<1>(tile);
					}
				}

				return -1.0f;
			}
	};

} // namespace tempo

#endif
