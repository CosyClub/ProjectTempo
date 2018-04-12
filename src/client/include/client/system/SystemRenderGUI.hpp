#ifndef CLIENT_ENTITY_RENDER_GUI_HPP
#define CLIENT_ENTITY_RENDER_GUI_HPP

#include <irrlicht.h>
#include <tempo/time.hpp>
#include <tempo/component/ComponentHealth.hpp>

namespace client
{
struct SystemRenderGUI {

	private:
		irr::gui::IGUIImage*      HUD;
		irr::video::ITexture*     texture_HUD;
		irr::video::ITexture*     texture_HUD_Active;
		irr::video::ITexture*     texture_HUD_Semi_Active;

	public:
		void setup(irr::IrrlichtDevice* device,
		           irr::video::IVideoDriver* driver);

		void update(irr::video::IVideoDriver * driver,
		            irr::gui::IGUIEnvironment *gui_env,
		            tempo::Clock &             clock,
		            int                        combo,
							  tempo::ComponentHealth     comp_health);

};
}

#endif
