#include <client/system/SystemLighting.hpp>
#include <client/misc/RGBtoHSV.hpp>
#include <IBillboardSceneNode.h>


namespace client {

	void SystemLighting::setup(irr::scene::ISceneManager *smgr,
							   irr::video::IVideoDriver * driver) {

		/// Create Main Colored LIGHT, does cast shadows:
		this->node_ML1 = 0;
		this->billboard_ML1 = smgr->addBillboardSceneNode(node_ML1);
		this->billboard_ML1->setPosition(irr::core::vector3df(0, 0, 0));
		this->billboard_ML1->setMaterialTexture(0, driver->getTexture("dummy.png")); /// Invisible: "dummy.png" - Visible: "estrela.png"
		this->billboard_ML1->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		this->billboard_ML1->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);

		// Add animator:
		irr::scene::ISceneNodeAnimator* anim_ML1 = smgr->createFlyCircleAnimator(irr::core::vector3df(0, 190, -420), 3.0f, 0.0030f);
		billboard_ML1->addAnimator(anim_ML1);
		anim_ML1->drop();

		/// Create the light: (This is the main light responsible for all the stage colors)
		this->light_ML1 = smgr->addLightSceneNode(billboard_ML1, irr::core::vector3df(0, 0, 0), irr::video::SColorf(1.0, 1.0, 1.0, 0.0), 3000);
		this->light_ML1->enableCastShadow(true);
		this->light_ML1->setVisible(true);
		this->data_ML1.Type = irr::video::ELT_POINT; // ELT_POINT | ELT_SPOT | ELT_DIRECTIONAL

		this->data_ML1.AmbientColor = irr::video::SColorf(0, 0, 0, 0);
		this->data_ML1.SpecularColor = irr::video::SColorf(0.5, 0.5, 0.5, 0);
		this->data_ML1.DiffuseColor = irr::video::SColorf(0.5, 0.5, 0.5, 0);
		this->light_ML1->setLightData(data_ML1);

	}

	void SystemLighting::update(const irr::video::SColor colour) {

		this->data_ML1.AmbientColor = colour;//irr::video::SColorf(red, green, blue, 0.0);
		//this->data_ML1.SpecularColor = colour;//irr::video::SColorf(red, green, blue, 0.0);
		//this->data_ML1.DiffuseColor = colour;//irr::video::SColorf(red, green, blue, 0.0);
		this->light_ML1->setLightData(data_ML1); // Apply data to the light
	}

}
