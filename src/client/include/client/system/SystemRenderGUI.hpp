#ifndef CLIENT_ENTITY_RENDER_GUI_HPP
#define CLIENT_ENTITY_RENDER_GUI_HPP

#include <irrlicht.h>
#include <tempo/time.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <client/component/ComponentKeyInput.hpp>
#include <ctime>

namespace client
{
struct SystemRenderGUI {

	private:
		// Misc Colours
		irr::video::SColor colour_white = irr::video::SColor(255, 255, 255, 255);
		irr::video::SColor colour_red   = irr::video::SColor(255, 255, 0, 0);
		irr::video::SColor colour_green = irr::video::SColor(255, 0, 255, 0);
		irr::video::SColor colour_blue  = irr::video::SColor(255, 135, 206, 250);

		irr::gui::IGUIImage*      HUD;
		irr::video::ITexture*     texture_HUD;
		irr::video::ITexture*     texture_HUD_Active[10];
		irr::video::ITexture*     texture_HUD_Semi_Active[10];

		std::clock_t timer_HUD_transition;
		int          HUD_transition_state;

		std::clock_t timer_nudge_picker;
		std::clock_t timer_missed;
		int message = 0;

		irr::core::stringw move_str[2] = {L"  Move on the beat!",
		                                  L"  Press E to dance!"};

		int          prev_combo;
		int          missed_combos;

		void updateComboCounter(irr::gui::IGUIFont *font,
		                        int combo,
		                        const irr::core::dimension2du screenSize);

		void updateNudge(irr::gui::IGUIFont *font,
		                 std::clock_t time_now,
		                 const irr::core::dimension2du screenSize,
		                 client::ComponentKeyInput      comp_input);

		void updateHUD(std::clock_t time_now, int combo, int colour_index);

		void updateComboBar(irr::video::IVideoDriver * driver,
		                    tempo::Clock &             clock,
		                    int                        combo,
		                    const irr::core::dimension2du screenSize);

		void updateHealthBar(irr::video::IVideoDriver * driver,
		                     tempo::ComponentHealth     comp_health,
		                     const irr::core::dimension2du screenSize);

	public:
		void init(irr::IrrlichtDevice* device,
		          irr::video::IVideoDriver* driver,
		          bool enable_hud);

		void setup(irr::IrrlichtDevice* device,
		           irr::video::IVideoDriver* driver,
		           bool enable_hud);

		void update(irr::video::IVideoDriver * driver,
		            irr::gui::IGUIEnvironment *gui_env,
		            tempo::Clock &             clock,
		            int                        combo,
		            tempo::ComponentHealth     comp_health,
		            client::ComponentKeyInput  comp_key_input,
		            int                        colour_index,
		            bool                       enable_hud);

};
}

#endif
