#include <tempo/entity/RenderHealth.hpp>

namespace tempo {
	void RenderHealth::HealthBarUpdate() {

		auto entities = getEntities();
		double scale;

		for(auto& entity : entities){
			auto& h = entity.getComponent<ComponentHealth>();
			auto& rend  = entity.getComponent<ComponentRender>();

			printf("\n\n RenderHealth 12\n\n");
			// Fraction of health left
			scale = (double) h.current_health / h.max_health;
			printf("\n\n RenderHealth 15\n\n");
			// Set scale of healthbar based on current health value
			printf("\nScale %f\n", scale);
			printf("\current health %d\n", h.current_health);
			printf("\nmax health %d\n", h.max_health);
			if (h.max_health != 0) {
				rend.healthBarnode->setScale(scale, 1, 1);
			}
			printf("\n\n RenderHealth 18\n\n");
			// Set colour of healthbar (Green = full health, Red = No Health)
			if(scale <= 0.5){
				printf("\n\n RenderHealth 21\n\n");
				rend.healthBillboard->setColour(Ogre::ColourValue(1,(scale/0.5),0));
				printf("\n\n RenderHealth 23\n\n");
			}

			else{
				printf("\n\n RenderHealth 27\n\n");
				rend.healthBillboard->setColour(Ogre::ColourValue(((1-scale)/0.5),1,0));
				printf("\n\n RenderHealth 29\n\n");
			}

		}
	}
}
