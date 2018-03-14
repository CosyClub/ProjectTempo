#include <client/system/SystemRenderGUI.hpp>

namespace client
{
void SystemRenderGUI::update(irr::video::IVideoDriver * driver,
                             irr::gui::IGUIEnvironment *gui_env,
                             tempo::Clock &             clock,
                             int                        combo)
{
	const irr::core::dimension2du &screenSize = driver->getScreenSize();

	irr::gui::IGUIFont *font = gui_env->getFont("resources/fonts/liberation_sans.xml");
	if (font) {
		irr::core::stringw str = L"Combo: ";
		str += combo;
		font->setKerningWidth(6);
		font->setKerningHeight(6);
		font->draw(
		  str.c_str(),
		  irr::core::rect<irr::s32>(10, 0.9 * screenSize.Height, 0.1 * screenSize.Width, 300),
		  irr::video::SColor(255, 255, 255, 255));
	}

	irr::video::SColor colour_white(255, 255, 255, 255);
	irr::video::SColor colour_blue(255, 135, 206, 250);

	float scale = clock.beat_progress_desc();

	if (scale >= 0.3) {
		driver->draw2DRectangle(
		  colour_white,
		  irr::core::rect<irr::s32>(screenSize.Width / 2 - 200 * scale, 0.94 * screenSize.Height,
		                            screenSize.Width / 2 + 200 * scale, 0.96 * screenSize.Height));
	} else {
		driver->draw2DRectangle(
		  colour_blue,
		  irr::core::rect<irr::s32>(screenSize.Width / 2 - 200 * scale, 0.94 * screenSize.Height,
		                            screenSize.Width / 2 + 200 * scale, 0.96 * screenSize.Height));
	}
}
}
