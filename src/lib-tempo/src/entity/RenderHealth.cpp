////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// Health.cpp
///
/// Contains definition of Health Component & System functions
////////////////////////////////////////////////////////////////////////////

#include <tempo/entity/RenderHealth.hpp>

#include <Ogre.h>

namespace tempo {
	void RenderHealth::HealthBarUpdate() {

		auto entities = getEntities();
		double scale;

		for(auto& entity : entities){
			auto& h = entity.getComponent<ComponentHealth>();
			auto& rend  = entity.getComponent<ComponentRender>();

			// Fraction of health left
			scale = (double) h.current_health / h.max_health;

			// Set scale of healthbar based on current health value
			rend.healthBarnode->setScale(scale,1,1);

			// Set colour of healthbar (Green = full health, Red = No Health)
			if(scale <= 0.5){
				rend.healthBillboard->setColour(Ogre::ColourValue(1,(scale/0.5),0));
			}

			else{
				rend.healthBillboard->setColour(Ogre::ColourValue(((1-scale)/0.5),1,0));
			}

		}
	}
}