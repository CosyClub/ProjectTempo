#include <tempo/entity/ComponentGridPosition.hpp>

namespace tempo{
	/////////////////////////////////////////////////////////////
	// ComponentGridPosition
	ComponentGridPosition::ComponentGridPosition(Vec2s pos, TileMask mask, bool is_ethereal) :
		ComponentGridPosition(pos.x, pos.y, mask, is_ethereal){
		// empty body
	}

	ComponentGridPosition::ComponentGridPosition(int x, int y, TileMask mask, bool is_ethereal){
		// level not currently used -> but when we want to do more efficient lookup
		// structure we need to insert entity in it based off of its position
		this->position = {x,y};
		this->mask     = mask;
		this->ethereal = is_ethereal;
	}

	void ComponentGridPosition::setPosition(Vec2s position) {
		this->position = position;
	}

	TileMask ComponentGridPosition::isCollidingWith(const ComponentGridPosition& other){
		return this->mask.isCollidingWith(other.mask, other.position - this->position);
	}

}
