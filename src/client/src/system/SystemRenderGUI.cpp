#include <client/system/SystemRenderGUI.hpp>

namespace client
{
void SystemRenderGUI::update(irr::video::IVideoDriver * driver,
                             irr::gui::IGUIEnvironment *gui_env,
                             tempo::Clock &             clock,
                             int                        combo)
{
	const irr::core::dimension2du &screenSize = driver->getScreenSize();

	irr::gui::IGUIFont *font = gui_env->getFont("resources/fonts/joystix/Joystix.xml");
	if (font) {
		irr::core::stringw str = L"Combo:";
		str += combo;
		// font->setKerningWidth(6);
		// font->setKerningHeight(6);
		font->draw(
		  str.c_str(),
		  irr::core::rect<irr::s32>(10, 0.9 * screenSize.Height, 0.1 * screenSize.Width, 300),
		  irr::video::SColor(255, 255, 255, 255));
	}

	irr::video::SColor colour_white(255, 255, 255, 255);
	irr::video::SColor colour_blue(255, 135, 206, 250);

	float scale = clock.beat_progress_desc();

  int bar_width_left    = screenSize.Width / 2 - (screenSize.Width * 0.22) * scale;
  int bar_width_right   = screenSize.Width / 2 + (screenSize.Width * 0.22) * scale;
  int bar_height_top    = 0.91 * screenSize.Height;
  int bar_height_bottom = 0.95 * screenSize.Height;

	if (scale >= 0.3) {
		driver->draw2DRectangle(
		  colour_white,
		  irr::core::rect<irr::s32>(bar_width_left, bar_height_top,
		                            bar_width_right, bar_height_bottom));
	} else {
		driver->draw2DRectangle(
		  colour_blue,
		  irr::core::rect<irr::s32>(bar_width_left, bar_height_top,
		                            bar_width_right, bar_height_bottom));
	}
}
}
