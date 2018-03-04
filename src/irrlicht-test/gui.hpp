////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \brief Contains test gui code
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_IRRLICHT_TEST_GUI_HPP
#define TEMPO_IRRLICHT_TEST_GUI_HPP

#include <irrlicht.h>

void setupGui(irr::gui::IGUIEnvironment *gui_env)
{
	irr::gui::IGUISkin *gui_skin = gui_env->getSkin();
	gui_skin->setColor(irr::gui::EGDC_BUTTON_TEXT, irr::video::SColor(255, 255, 255, 255));
	gui_skin->setFont(gui_env->getFont("resources/fonts/liberation_sans.xml"));

	gui_env->addStaticText(L"Hello World! This is Irrlicht",
	                       irr::core::rect<irr::s32>(10, 10, 500, 50), false);
}

#endif
