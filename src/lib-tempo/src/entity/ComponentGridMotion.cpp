#include <tempo/entity/ComponentGridMotion.hpp>

namespace tempo{

	/////////////////////////////////////////////////////////////
	// ComponentGridMotion
	ComponentGridMotion::ComponentGridMotion(float max_jump_distance,
	                                         float movement_speed,
	                                         float max_jump_height){
		this->delta             = {0,0};
		this->motion_progress   = 0;
		this->max_jump_height   = max_jump_height;
		this->max_jump_distance = max_jump_distance;
		this->movement_speed    = movement_speed;
		this->target_locked     = false;
	}

	bool ComponentGridMotion::beginMovement(int dx, int dy){
		if(this->motion_progress != 0){ return false; }

		if((dx*dx + dy*dy) > (this->max_jump_distance * this->max_jump_distance)){
			return false;
		}

		this->delta.x = dx;
		this->delta.y = dy;
		return true;
	}

	const Vec2s& ComponentGridMotion::getCurrentMovement(){
		return this->delta;
	}

	float ComponentGridMotion::getMotionProgress(){
		return this->motion_progress;
	}

	bool ComponentGridMotion::isMoving(){
		return this->motion_progress >= 0.0f;
	}

	bool ComponentGridMotion::isMovementLocked(){
		return this->target_locked;
	}

	void ComponentGridMotion::setDelta(Vec2s delta) {
		this->delta = delta;
	}

	void ComponentGridMotion::setMotionProgress(float motion_progress) {
		this->motion_progress = motion_progress;
	}

	void ComponentGridMotion::setTargetLocked(bool target_locked) {
		this->target_locked = target_locked;
	}
}
