#include <client/system/SystemRenderHealthBars.hpp>
#include <client/misc/RGBtoHSV.hpp>

namespace client
{
void SystemRenderHealthBars::setup(irr::scene::ISceneManager *smgr)
{
	original_size = irr::core::dimension2d<irr::f32>(0.7f, 0.15f);
	irr::core::dimension2d<irr::f32> size(0.7f, 0.15f);
	irr::core::vector3df             pos(-0.3f, 1.6f + size.Height / 2, 0.0f);
	irr::video::SColor               color(255, 0, 255, 0);

	auto entities = getEntities();

	for (auto &entity : entities) {
		// auto &health    = entity.getComponent<tempo::ComponentHealth>();
		auto &healthbar = entity.getComponent<client::ComponentHealthBar>();
		auto &sn        = entity.getComponent<client::ComponentRenderSceneNode>();

		if (healthbar.node != nullptr)
			continue;

		healthbar.node = smgr->addBillboardSceneNode(sn.node, size,
		                                             pos,  // fix alignment
		                                             -1, color, color);
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

		// Fraction of health left
		scale = (double) health.current_health / health.max_health;

		float new_width = original_size.Width * scale;
		irr::core::dimension2d<irr::f32> newSize(new_width, original_size.Height);
		healthbar->setSize(newSize);

		irr::core::vector3df c1 = RGBtoHSV(colour_green);
		irr::core::vector3df c2 = RGBtoHSV(colour_red);
		c1.X = c1.X * scale + c2.X * (1.f - scale);
		irr::video::SColor colour_health = HSVtoRGB(c1);
		healthbar->setColor(colour_health);


		if (entity.hasComponent<tempo::ComponentCombo>())
		{
			tempo::ComponentCombo& c = entity.getComponent<tempo::ComponentCombo>();
			float scale = c.comboCounter / 20.f;
			scale = fmin(scale, 0.5);
			newSize += newSize * scale;
			irr::core::vector3df          pos(-0.3f, 1.6f + newSize.Height / 2, 0.0f);
			pos += pos * scale;
			healthbar->setSize(newSize);
			healthbar->setPosition(pos);

		}
	}
}
}
