#include <client/system/SystemRenderHealthBars.hpp>
#include <client/misc/YAlignedBillboardSceneNode.hpp>
#include <client/misc/RGBtoHSV.hpp>

namespace client
{
void SystemRenderHealthBars::setup(irr::scene::ISceneManager *smgr)
{


	original_size = irr::core::dimension2d<irr::f32>(0.7f, 0.15f);
	irr::core::dimension2d<irr::f32> size(0.7f, 0.15f);
	irr::video::SColor               color(255, 0, 255, 0);

	auto entities = getEntities();

	for (auto &entity : entities) {
		// auto &health    = entity.getComponent<tempo::ComponentHealth>();
		auto &healthbar = entity.getComponent<client::ComponentHealthBar>();
		auto &sn        = entity.getComponent<client::ComponentRenderSceneNode>();

		if (healthbar.node != nullptr)
			continue;

		irr::core::dimension2d<float> bbsize = sn.billboard->getSize();
		irr::core::vector3df pos(0.0f, bbsize.Height / 1.5, 0.0f);

		healthbar.node = new irr::scene::YAlignedBillboardSceneNode(sn.billboard, smgr, -1, pos, size, color, color);
		healthbar.node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		healthbar.node->grab();
	}
}

void SystemRenderHealthBars::update()
{
	irr::video::SColor colour_green(255, 0, 255, 0);
	irr::video::SColor colour_red(255, 255, 0, 0);

	auto   entities = getEntities();
	double scale;

	for (auto &entity : entities) {
		auto &health    = entity.getComponent<tempo::ComponentHealth>();
		auto &healthbar = entity.getComponent<client::ComponentHealthBar>().node;
		auto &sn = entity.getComponent<client::ComponentRenderSceneNode>();

		// Fraction of health left
		scale = (double) health.current_health / health.max_health;

		float new_width = original_size.Width * scale;
		irr::core::dimension2d<irr::f32> newSize(new_width, original_size.Height);
		healthbar->setSize(newSize);
		
		// TODO: remove this when player growth is handeled better
		irr::core::dimension2d<float> bbsize = sn.billboard->getSize();
		irr::core::vector3df pos(0.0f, bbsize.Height / 2.0f + original_size.Height, 0.0f);
		healthbar->setPosition(pos);

		irr::core::vector3df c1 = RGBtoHSV(colour_green);
		irr::core::vector3df c2 = RGBtoHSV(colour_red);
		c1.X = c1.X * scale + c2.X * (1.f - scale);
		irr::video::SColor colour_health = HSVtoRGB(c1);
		healthbar->setColor(colour_health);
	}
}
}
