#ifndef TEMPO_COMPONENT_STAGE_HPP
#define TEMPO_COMPONENT_STAGE_HPP

#include <anax/Component.hpp>

#include <glm/fwd.hpp>

#include <tuple>

namespace tempo{
	// An entity that is bound by the world stage
	struct ComponentStage : anax::Component {
	private:
		//this implementation may change without notice
		std::vector<std::tuple<glm::ivec2, float>> tiles;
	public:
		ComponentStage(const char* stage_file);

		std::vector<std::tuple<glm::ivec2, float>> getHeights();
	};
}

#endif
