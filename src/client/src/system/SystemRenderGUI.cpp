#include <client/system/SystemRenderGUI.hpp>
#include <client/misc/RGBtoHSV.hpp>

namespace client
{

void SystemRenderGUI::setup(irr::IrrlichtDevice *device,
                             irr::video::IVideoDriver* driver) {

  device->getGUIEnvironment()->addImage(
                  driver->getTexture("resources/materials/textures/HUD.png"),
                  irr::core::position2d<irr::s32>(0,0), true);

}

void SystemRenderGUI::update(irr::video::IVideoDriver * driver,
                             irr::gui::IGUIEnvironment *gui_env,
                             tempo::Clock &             clock,
                             int                        combo,
                             tempo::ComponentHealth     comp_health)
{
  // Get the screen size to adjust the position and size of UI elements
	const irr::core::dimension2du &screenSize = driver->getScreenSize();

  // Misc Colours
  irr::video::SColor colour_white(255, 255, 255, 255);
  irr::video::SColor colour_red(255, 255, 0, 0);
  irr::video::SColor colour_green(255, 0, 255, 0);
  irr::video::SColor colour_blue(255, 135, 206, 250);

  // Display Combo text
	irr::gui::IGUIFont *font = gui_env->getFont("resources/fonts/joystix/Joystix.xml");
	if (font) {
    char buffer[5];
    sprintf(buffer, "%3d", combo);

		irr::core::stringw str = L"";
		str += buffer;
		font->draw(
		  str.c_str(),
		  irr::core::rect<irr::s32>(60, 0.85 * screenSize.Height, 0.2 * screenSize.Width, 300),
		  irr::video::SColor(255, 255, 255, 255));
	}


  // Display combo bar
	float combo_scale = clock.beat_progress_desc();

  int combo_width_left    = screenSize.Width / 2 - (screenSize.Width * 0.26) * combo_scale;
  int combo_width_right   = screenSize.Width / 2 + (screenSize.Width * 0.26) * combo_scale;
  int combo_height_top    = (1.f - 3.f / 36) * screenSize.Height;
  int combo_height_bottom = (1.f - 1.f / 36) * screenSize.Height;

	if (combo_scale >= 0.3) {
		driver->draw2DRectangle(
		  colour_white,
		  irr::core::rect<irr::s32>(combo_width_left, combo_height_top,
		                            combo_width_right, combo_height_bottom));
	} else {
		driver->draw2DRectangle(
		  colour_blue,
		  irr::core::rect<irr::s32>(combo_width_left, combo_height_top,
		                            combo_width_right, combo_height_bottom));
	}


  // Display health bar
  float health_scale =  (float) comp_health.current_health /  (float) comp_health.max_health;

  int health_width_left    = (1.f - 3.f / 64) * screenSize.Width;
  int health_width_right   = (1.f - 1.f / 64) * screenSize.Width;
  int health_height_top    = (1.f - 1.f / 36 - 0.499 * health_scale  ) * screenSize.Height;
  int health_height_bottom = (1.f - 1.f / 36) * screenSize.Height;

  irr::core::vector3df c1 = RGBtoHSV(colour_green);
  irr::core::vector3df c2 = RGBtoHSV(colour_red);

  c1.X = c1.X * health_scale + c2.X * (1.f - health_scale);

  irr::video::SColor colour_health = HSVtoRGB(c1);

  driver->draw2DRectangle(
    colour_health,
    irr::core::rect<irr::s32>(health_width_left, health_height_top,
                              health_width_right, health_height_bottom));
}
}
