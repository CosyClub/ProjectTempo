#ifndef TEMPO_COMPONENT_STAGE_TRANSLATION_HPP
#define TEMPO_COMPONENT_STAGE_TRANSLATION_HPP
#include <anax/Component.hpp>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

namespace tempo
{
	
// Represents a translation on the stage
struct ComponentStageTranslation : anax::Component 
{
	// The position translation
	glm::ivec2 delta;
	// Creates the stage translation with no delta
	ComponentStageTranslation();
};

} // namespace tempo

#endif
