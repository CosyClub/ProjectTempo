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
			auto& p = entity.getComponent<ComponentPosition>();
			auto& g = entity.getComponent<ComponentGridMotion>();

			if(g.current != g.target){
				g.motion_progress += dt / 0.1f;
				printf("dt: %7f, Motion progress: %f\n", dt, g.motion_progress);
				if(g.motion_progress >= 1){
					g.current = g.target;
					g.motion_progress = 0;
				}
			}

			p.position.x = Ogre::Math::lerp(g.current.x, g.target.x, g.motion_progress);
			p.position.z = Ogre::Math::lerp(g.current.y, g.target.y, g.motion_progress);;

			float a = g.motion_progress - 0.5;
			p.position.y = (-(a*a) + 0.25f) * 2.0f;


			if(p.position.x < this->min_x){ p.position.x = this->min_x; }
			if(p.position.x > this->max_x){ p.position.x = this->max_x; }
			if(p.position.z < this->min_y){ p.position.z = this->min_y; }
			if(p.position.z > this->max_y){ p.position.z = this->max_y; }
		}
	}
}
