#include <client/system/SystemRenderHealthBars.hpp>

namespace client {


	void SystemRenderHealthBars::setup(irr::scene::ISceneManager* smgr) {
		irr::core::dimension2d<irr::f32> size(0.7f, 0.15f);
		irr::core::vector3df pos(-0.5f, 1.6f + size.Height / 2, 0.0f);
		irr::video::SColor color(255, 0, 255, 0);

			auto entities = getEntities();

			for(auto& entity : entities) {
				auto& health      = entity.getComponent<tempo::ComponentHealth>();
				auto& healthbar   = entity.getComponent<client::ComponentHealthBar>().healthBarnode;
				auto& sn = entity.getComponent<client::ComponentRenderSceneNode>();

				healthbar = smgr->addBillboardSceneNode(
				   sn.node,
				   size,
				   pos, // fix alignment
				   -1,
				   color,
				   color);
				   healthbar->setMaterialFlag(irr::video::EMF_LIGHTING, false);

			}
	}

	void SystemRenderHealthBars::update() {

		irr::video::SColor colour_green(255, 0, 255, 0);
		irr::video::SColor colour_red(255, 255, 0, 0);

		auto entities = getEntities();
		double scale;

		for(auto& entity : entities) {
			auto& health      = entity.getComponent<tempo::ComponentHealth>();
			auto& healthbar   = entity.getComponent<client::ComponentHealthBar>().healthBarnode;

			irr::core::dimension2d<irr::f32> prev_size = healthbar->getSize();

			// Fraction of health left
			scale = (double) health.current_health / health.max_health;

			float new_width = prev_size.Width * scale;

			healthbar->setSize(irr::core::dimension2d<irr::f32>(new_width, prev_size.Height));
			// Set colour of healthbar (Green = full health, Red = No Health)
			if(scale <= 0.5){
				healthbar->setColor(colour_red);
			}

			else{
				healthbar->setColor(colour_green);
			}

		}
	}
}
