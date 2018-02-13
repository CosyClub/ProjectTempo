#include <tempo/entity/Transform.hpp>

#include <glm/vec2.hpp>

namespace tempo{
	ComponentTransform::ComponentTransform() : ComponentTransform(0, 0, 0){
		// empty body
	}

	ComponentTransform::ComponentTransform(Ogre::Vector3 pos) : ComponentTransform(pos.x, pos.y, pos.z){
		// empty body
	}

	ComponentTransform::ComponentTransform(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
		this->position.x = x;
		this->position.y = y;
		this->position.z = z;
		this->rotation= Ogre::Quaternion();
	}


	void SystemUpdateTransforms::update(SystemLevelManager& level){
		auto entities = getEntities();

		for(auto& entity : entities){
			auto& trans = entity.getComponent<ComponentTransform>();
			auto& pos   = entity.getComponent<ComponentGridPosition>();
			auto& gm    = entity.getComponent<ComponentGridMotion>();

			float motion_progress = gm.getMotionProgress();
			glm::vec2 delta           = gm.getCurrentMovement();
			glm::vec2 position        = pos.getPosition();
			glm::vec2 target          = position + delta;

			float current_height = level.getHeight(position);
			float target_height  = level.getHeight(target  );

			// :TODO: vector lerps would be nice... (as would swizzles)
			trans.position.x = Ogre::Math::lerp<float>(position.x, target.x, motion_progress);
			trans.position.y = current_height;
			trans.position.z = Ogre::Math::lerp<float>(position.y, target.y, motion_progress);

			/////////////////////////////////
			// hop effect
			float a = motion_progress - 0.5;
			// Add sinosodial hop motion
			trans.position.y += (-(a*a) + 0.25f) * gm.max_jump_height * 2.0f;
			// Add motion to get between current tile height and target
			trans.position.y += Ogre::Math::lerp(0.0f, target_height - current_height, motion_progress);
		}
	}
}
