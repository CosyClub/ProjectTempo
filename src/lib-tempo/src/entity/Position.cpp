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
	ComponentPosition::ComponentPosition() : ComponentPosition(0, 0, 0){
		// empty body
	}

	ComponentPosition::ComponentPosition(Ogre::Vector3 pos) : ComponentPosition(pos.x, pos.y, pos.z){
		// empty body
	}

	ComponentPosition::ComponentPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
		this->position.x = x;
		this->position.y = y;
		this->position.z = z;
		this->velocity = Ogre::Vector3(0,0,0);

		//this->rotation = Ogre::Quaternion();
		//this->rotational_velocity = Ogre::Quaternion();
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
