#ifndef TEMPO_COMPONENT_POSITION_HPP
#define TEMPO_COMPONENT_POSITION_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentStagePosition.hpp>

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace tempo{
	struct ComponentTransform : anax::Component {
		/// \brief Offset from world origin to entity's local origin
		glm::vec3 position;

		/// \brief Rotation of the entity about its local origin
		glm::quat rotation;

		ComponentTransform();
		ComponentTransform(glm::vec3 pos);
		ComponentTransform(float x, float y, float z);
	};
}

#endif
