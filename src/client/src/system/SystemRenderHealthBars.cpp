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
	// void SystemRenderHealthBars::HealthBarUpdate() {
	//
	// 	auto entities = getEntities();
	// 	double scale;
	//
	// 	for(auto& entity : entities){
	// 		auto& h = entity.getComponent<ComponentHealth>();
	// 		auto& rend  = entity.getComponent<ComponentRender>();
	//
	// 		// Fraction of health left
	// 		scale = (double) h.current_health / h.max_health;
	//
	// 		// Set scale of healthbar based on current health value
	// 		rend.healthBarnode->setScale(scale,1,1);
	//
	// 		// Set colour of healthbar (Green = full health, Red = No Health)
	// 		if(scale <= 0.5){
	// 			rend.healthBillboard->setColour(Ogre::ColourValue(1,(scale/0.5),0));
	// 		}
	//
	// 		else{
	// 			rend.healthBillboard->setColour(Ogre::ColourValue(((1-scale)/0.5),1,0));
	// 		}
	//
	// 	}
	// }
}
