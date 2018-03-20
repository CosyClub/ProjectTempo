#ifndef CLIENT_ENTITY_RENDER_GUI_HPP
#define CLIENT_ENTITY_RENDER_GUI_HPP

#include <irrlicht.h>
#include <tempo/time.hpp>
#include <tempo/component/ComponentHealth.hpp>

namespace client
{
struct SystemRenderGUI {
	void update(irr::video::IVideoDriver * driver,
	            irr::gui::IGUIEnvironment *gui_env,
	            tempo::Clock &             clock,
	            int                        combo,
						  tempo::ComponentHealth     comp_health);
};
}

#endif
