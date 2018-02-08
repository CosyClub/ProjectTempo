#include <tempo/entity/RenderHealth.hpp>

namespace tempo {
	void RenderHealth::HealthBarUpdate() {

		auto entities = getEntities();
		double scale;

		for (auto& entity : entities) {
			auto& h = entity.getComponent<ComponentHealth>();
			auto& rend = entity.getComponent<ComponentRender>();
			bool hasHealthBar = rend.hasHealthBar();

			if (hasHealthBar) {
				// Fraction of health left
				scale = (double)h.getCurrentHealth() / h.getMaxHealth();
				// Set scale of healthbar based on current health value
				rend.healthBarnode->setScale(scale, 1, 1);
				// Set colour of healthbar (Green = full health, Red = No Health)
				if (scale <= 0.5) {
					rend.healthBillboard->setColour(Ogre::ColourValue(1, (scale / 0.5), 0));
				}

				else {
					rend.healthBillboard->setColour(Ogre::ColourValue(((1 - scale) / 0.5), 1, 0));
				}

			}
		}
	}
}
