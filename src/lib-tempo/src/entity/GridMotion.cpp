////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file GridMotion.cpp
/// \author Jamie Terry
/// \date 2017/11/15
/// \brief Contains definition of GridMotion functions
////////////////////////////////////////////////////////////////////////////

#include <OgreMath.h>

#include <tempo/entity/GridMotion.hpp>

namespace tempo{
	ComponentGridMotion::ComponentGridMotion() : ComponentGridMotion(0,0){
		// empty body
	}

	ComponentGridMotion::ComponentGridMotion(Ogre::Vector2 pos) : ComponentGridMotion(pos.x, pos.y){
		// empty body
	}

	ComponentGridMotion::ComponentGridMotion(Ogre::Real x, Ogre::Real y){
		this->current = Ogre::Vector2(x,y);
		this->target  = Ogre::Vector2(x,y);
		this->motion_progress = 0;
	}

	bool ComponentGridMotion::moveBy(Ogre::Real x, Ogre::Real y){
		if(this->motion_progress != 0){
			return false;
		}

		this->target.x = this->current.x + x;
		this->target.y = this->current.y + y;
		return true;
	}

	SystemGridMotion::SystemGridMotion(int nmin_x, int nmin_y, int nmax_x, int nmax_y)
		: min_x(nmin_x), min_y(nmin_y), max_x(nmax_x), max_y(nmax_y) {
		// empty body
	}

	void SystemGridMotion::update(float dt){
		auto entities = getEntities();

		for(auto& entity : entities){
			auto& trans = entity.getComponent<ComponentTransform>();
			auto& gm    = entity.getComponent<ComponentGridMotion>();

			if(gm.current != gm.target){
				gm.motion_progress += dt / 0.1f;
				if(gm.motion_progress >= 1){
					gm.current = gm.target;
					gm.motion_progress = 0;
				}
			}

			trans.position.x = Ogre::Math::lerp(gm.current.x, gm.target.x, gm.motion_progress);
			trans.position.z = Ogre::Math::lerp(gm.current.y, gm.target.y, gm.motion_progress);;

			// hop effect
			float a = gm.motion_progress - 0.5;
			trans.position.y = (-(a*a) + 0.25f) * 2.0f;

			if(trans.position.x < this->min_x){ trans.position.x = this->min_x; }
			if(trans.position.x > this->max_x){ trans.position.x = this->max_x; }
			if(trans.position.z < this->min_y){ trans.position.z = this->min_y; }
			if(trans.position.z > this->max_y){ trans.position.z = this->max_y; }
		}
	}
}