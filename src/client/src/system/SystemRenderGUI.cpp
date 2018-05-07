#include <client/system/SystemRenderGUI.hpp>
#include <client/misc/RGBtoHSV.hpp>

namespace client
{

	void SystemRenderGUI::init(irr::IrrlichtDevice *device,
		                         irr::video::IVideoDriver* driver,
		                         bool enable_hud) {

		if(enable_hud) {
			texture_HUD = driver->getTexture("resources/materials/textures/hud/HUD.png");

			irr::core::stringw active_path = L"resources/materials/textures/hud/HUD-Active-";
			irr::core::stringw semi_path = L"resources/materials/textures/hud/HUD-SemiActive-";

			char buffer[5];

			irr::core::stringw str = L"";
			str += buffer;
			for(int i = 0; i < 10; i++)
			{
				sprintf(buffer, "%d", i);
				texture_HUD_Active[i] = driver->getTexture(active_path + buffer + ".png");
				texture_HUD_Semi_Active[i] = driver->getTexture(semi_path + buffer + ".png");
			}

		}

	}


void SystemRenderGUI::setup(irr::IrrlichtDevice *device,
	                          irr::video::IVideoDriver* driver,
	                          bool enable_hud) {

	if(enable_hud) {
		HUD = device->getGUIEnvironment()->addImage(
		                texture_HUD,
		                irr::core::position2d<irr::s32>(0,0), true);
	}

	timer_nudge_picker  = std::clock();
	timer_missed = std::clock();
	timer_HUD_transition = std::clock();
	HUD_transition_state = 0;

	prev_combo = 0;
	missed_combos = 0;
}

void SystemRenderGUI::update(irr::video::IVideoDriver * driver,
	                           irr::gui::IGUIEnvironment *gui_env,
	                           tempo::Clock &             clock,
	                           int                        combo,
	                           tempo::ComponentHealth     comp_health,
                             client::ComponentKeyInput  comp_player_input,
	                           int                        colour_index,
	                           bool                       enable_hud)
{

	std::clock_t time_now = std::clock();

	// HUD transition animation
	if(enable_hud) {
		updateHUD(time_now, combo, colour_index);
	}
	// Get the screen size to adjust the position and size of UI elements
	const irr::core::dimension2du &screenSize = driver->getScreenSize();

	if(prev_combo > combo) {
		missed_combos++;
	} else if(prev_combo < combo) {
		missed_combos = 0;
	}

	if(missed_combos == 4) {
		missed_combos = 0;
		timer_missed = std::clock();
	}
	prev_combo = combo;

	irr::gui::IGUIFont *font = gui_env->getFont("resources/fonts/joystix72/myfont.xml");
	if (font) {
		updateNudge(font, time_now, clock, screenSize, comp_player_input);
		updateComboCounter(font, combo, screenSize);
	}

	// Display combo bar
	updateComboBar(driver, clock, combo, comp_player_input, screenSize);
	// Display health bar
	updateHealthBar(driver, comp_health, screenSize);
}


void SystemRenderGUI::updateComboCounter(irr::gui::IGUIFont *font,
	                                       int combo,
	                                       const irr::core::dimension2du screenSize) {
	char buffer[5];
	sprintf(buffer, "%3d", combo);

	// Display Combo text
	irr::core::stringw str = L"";
	str += buffer;
	font->draw(
		str.c_str(),
		irr::core::rect<irr::s32>(40, 0.84 * screenSize.Height, 0.2 * screenSize.Width, 300),
		irr::video::SColor(255, 255, 255, 255));
}

void SystemRenderGUI::updateNudge(irr::gui::IGUIFont *font,
	                                std::clock_t time_now,
                                  tempo::Clock& clock,
                                  const irr::core::dimension2du screenSize,
                                  client::ComponentKeyInput comp_input) {

	if(comp_input.actions.size() < 2){
		// Then tell the player to move

		if((time_now - timer_nudge_picker ) / (double) CLOCKS_PER_SEC > 3.0f) {
			timer_nudge_picker = time_now;
			message = (message + 1) % 2;
		}

		font->draw(move_str[message].c_str(),
		           irr::core::rect<irr::s32>( 0.2 * screenSize.Width,
		                                      0.70 * screenSize.Height,
		                                      0.2 * screenSize.Width, 300),
		           irr::video::SColor(255, 255, 255, 255));

		return;
	}
	// If still running then we have 2 or more data points in the history

	////////////////////////////////////////////////////////////////////////////
	// Compute some stats about the history of keypresses

	float window_size = clock.get_beat_window_delta().asSeconds();
	float beat_length = clock.get_beat_length().asSeconds();

	// Out of those beats represented in the history, on how many...
	int   beats_hit      = 0; // did the first key press occur within  the window
	int   beats_missed   = 0; // did the first key press occur outside the window
	int   beats_repeated = 0; // did multiple key presses occur

	float avg_beat_delta      = 0; // average delta to the beat for each key press

	for(unsigned int i = 1; i < comp_input.actions.size(); ++i){
		avg_beat_delta += comp_input.actions[i].delta;

		if(comp_input.actions[i].beat == comp_input.actions[i-1].beat){
			++beats_repeated;
		} else {
			if(abs(comp_input.actions[i].delta) < window_size){
				++beats_hit;
			} else {
				++beats_missed;
			}
		}
	}
	avg_beat_delta /= (float)comp_input.actions.size()-1;

	float avg_key_delta = 0; // average delta between key presses
	for(unsigned int i = 1; i < comp_input.actions.size(); ++i){
		float delta = beat_length * (comp_input.actions[i].beat - comp_input.actions[i-1].beat);
		delta -= comp_input.actions[i-1].delta;
		delta += comp_input.actions[i+0].delta;
		avg_key_delta += delta;
	}
	avg_key_delta /= (float)comp_input.actions.size()-1;

	//printf("Avg delta: %8f, hit: %2i, missed %2i, repeated: %2i, avg_key_delta: %8f\n",
	//       avg_beat_delta, beats_hit, beats_missed, beats_repeated, avg_key_delta);

	const char* msg = nullptr;


	// This is how much we expect the user's keypresses to change
	// by in relation to the beat per beat
	float drift = beat_length - avg_key_delta;

	if(beats_missed > 0){
		if(drift > 0 && avg_beat_delta + drift * 5 > window_size){
			msg = "Too fast!";
		}
		if(drift < 0 && avg_beat_delta + drift * 5 < -window_size){
			msg = "Too slow!";
		}
	}

	if(msg != nullptr){
		font->draw(msg,
		           irr::core::rect<irr::s32>( 0.35 * screenSize.Width,
		                                      0.70 * screenSize.Height,
		                                      0.20 * screenSize.Width, 300),
		           irr::video::SColor(200, 255, 127, 127));
	}
}

void SystemRenderGUI::updateHUD(std::clock_t time_now, int combo, int colour_index)
{
	if((time_now - timer_HUD_transition ) / (double) CLOCKS_PER_SEC > 0.4 ) {
		if(combo > 20 && HUD_transition_state == 1) {
			HUD_transition_state = 2;
			timer_HUD_transition = time_now;
			HUD->setImage(texture_HUD_Active[colour_index]);
		} else if((combo > 20 && HUD_transition_state == 0) ||
					(combo < 20 && HUD_transition_state == 2) ) {
					HUD_transition_state = 1;
					HUD->setImage(texture_HUD_Semi_Active[colour_index]);
					timer_HUD_transition = time_now;
		} else if(combo < 20 && HUD_transition_state == 1) {
			HUD_transition_state = 0;
			HUD->setImage(texture_HUD);
			timer_HUD_transition = time_now;
		}
	}

	if(HUD_transition_state == 2) {
		HUD->setImage(texture_HUD_Active[colour_index]);
	} else if(HUD_transition_state == 1) {
		HUD->setImage(texture_HUD_Semi_Active[colour_index]);
	}

}

void SystemRenderGUI::updateComboBar(irr::video::IVideoDriver * driver,
	                                   tempo::Clock &             clock,
	                                   int                        combo,
                                     client::ComponentKeyInput  comp_input,
	                                   const irr::core::dimension2du screenSize)
{
	int combo_width_center  = screenSize.Width / 2;
	int combo_width_left    = combo_width_center - (screenSize.Width * 0.25);
	int combo_width_right   = combo_width_center + (screenSize.Width * 0.25);
	int combo_width         = combo_width_right - combo_width_left;
	int combo_height_top    = (1.f - 3.f / 36) * screenSize.Height;
	int combo_height_center = (1.f - 2.f / 36) * screenSize.Height;
	int combo_height_bottom = (1.f - 1.f / 36) * screenSize.Height;


	float beat_size   = clock.get_beat_length      ().asSeconds();
	float window_size = clock.get_beat_window_delta().asSeconds();

	float window_proportion = window_size / beat_size;

	int window_left   = combo_width_center - (int)(combo_width * window_proportion);
	int window_right  = combo_width_center + (int)(combo_width * window_proportion);

	int indicator_center = combo_width_left + (int)(combo_width * (clock.beat_progress() > 0.5 ?
	                                                               clock.beat_progress() - 0.5 :
	                                                               clock.beat_progress() + 0.5));
	int indicator_left   = indicator_center - 6;
	int indicator_right  = indicator_center + 6;
	if(indicator_left  < combo_width_left ){ indicator_left  = combo_width_left;  }
	if(indicator_right > combo_width_right){ indicator_right = combo_width_right; }

	bool last_key_was_this_beat = false;
	if(comp_input.actions.size() > 0){
		if(comp_input.actions.back().delta < 0){
			// Then the key was pressed before start of this beat
			last_key_was_this_beat = (clock.get_beat_number() + 1 ==
			                          comp_input.actions.back().beat
			                         );
		} else {
			// then key was pressed after start of this beat
			last_key_was_this_beat = (clock.get_beat_number() ==
			                          comp_input.actions.back().beat
			                         );
		}
	}

	////////////////////////////////////////////////////////
	// Highlight the window keys can be pressed in
	driver->draw2DRectangle(irr::video::SColor(150, 0, 255, 0),
	                        irr::core::rect<irr::s32>(window_left,  combo_height_top,
	                                                  window_right, combo_height_bottom));


	////////////////////////////////////////////////////////
	// Bars at the side of the screen
	irr::video::SColor colour_combo_bar = irr::video::SColor(255, 50, 50, 50);

	// If last key press was on this beat then flash the bar
	if(last_key_was_this_beat){
		if(comp_input.actions.back().outside_window){
			colour_combo_bar = irr::video::SColor(255, 255,   0,   0);
		} else  {
			colour_combo_bar = irr::video::SColor(255, 204, 223, 255);
		}
	}
	driver->draw2DRectangle(colour_combo_bar,
	                        irr::core::rect<irr::s32>(combo_width_left,  combo_height_top,
	                                                  window_left, combo_height_bottom));

	driver->draw2DRectangle(colour_combo_bar,
	                        irr::core::rect<irr::s32>(window_right,  combo_height_top,
	                                                  combo_width_right, combo_height_bottom));

	////////////////////////////////////////////////////////
	// Last key press indicator
	if(last_key_was_this_beat){

		int last_indicator_center = (combo_width_left +
		                             (int)(
		                                   ((comp_input.actions.back().delta + (beat_size / 2.0f)) /
		                                    beat_size
		                                   ) * combo_width
		                                   )
		                             );

		driver->draw2DRectangle(irr::video::SColor(255, 20, 20, 20),
	                        irr::core::rect<irr::s32>(last_indicator_center-7, combo_height_center-12,
	                                                  last_indicator_center+7, combo_height_center+12));


	}

	////////////////////////////////////////////////////////
	// Beat progress indicator
	driver->draw2DRectangle(irr::video::SColor(255, 20, 20, 20),
	                        irr::core::rect<irr::s32>(indicator_left, combo_height_top,
	                                                  indicator_right, combo_height_bottom));



}

void SystemRenderGUI::updateHealthBar(irr::video::IVideoDriver * driver,
	                                    tempo::ComponentHealth     comp_health,
	                                    const irr::core::dimension2du screenSize)
{

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
