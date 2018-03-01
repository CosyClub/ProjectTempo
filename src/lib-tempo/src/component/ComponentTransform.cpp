#include <tempo/component/ComponentTransform.hpp>

namespace tempo{
	ComponentTransform::ComponentTransform() : ComponentTransform(0, 0, 0){
		// empty body
	}

	ComponentTransform::ComponentTransform(glm::vec3 pos) : ComponentTransform(pos.x, pos.y, pos.z){
		// empty body
	}

	ComponentTransform::ComponentTransform(float x, float y, float z) {
		this->position.x = x;
		this->position.y = y;
		this->position.z = z;
		this->rotation   = glm::quat();
	}
}
