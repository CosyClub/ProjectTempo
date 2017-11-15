////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Position.cpp
/// \author Jamie Terry
/// \date 2017/11/14
/// \brief Contains definition of Position system functions
////////////////////////////////////////////////////////////////////////////

#include <tempo/entity/Position.hpp>

namespace tempo{
	ComponentPosition::ComponentPosition() {
		// empty body
	}

	ComponentPosition::ComponentPosition(Ogre::Vector3 pos) {
		this->position = pos;
	}

	ComponentPosition::ComponentPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
		this->position.x = x;
		this->position.y = y;
		this->position.z = z;
	}


	void SystemPosition::update(double dt){
		auto entities = getEntities();
		for(auto& entity : entities){
			//printf("SystemPosition is updating entity %i\n", entity.getId());
			auto& p = entity.getComponent<ComponentPosition>();

			p.position += (p.velocity            * dt);
			//p.rotation *= (p.rotational_velocity * dt);
		}
	}
}
