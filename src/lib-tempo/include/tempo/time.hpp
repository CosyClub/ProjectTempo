#ifndef TEMPO_TIME_HPP
#define TEMPO_TIME_HPP

#include <tempo/config.hpp>
#include <tempo/song.hpp>

#include <SFML/Network.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

namespace tempo
{
//
// =========
// Figure 1:
// =========
//
//        Delta   Delta       Delta   Delta
//        Start    End        Start    End
//          V       V           V       V
// ....-----"---|---"-----+-----"---|---"-----+-----....
//              ^         ^         ^         ^
//             Beat       ^        Beat       ^
//            Passed      ^       Passed      ^
//                        ^                   ^
//                     Midpoint            Midpoint
//                    of 2 beats          of 2 beats
//                    (antibeat)          (antibeat)
//
// Where:
//   "Delta End" - "Beat Passed" == "Beat Passed" - "Delta Start"

class Clock
{
   public:
	// When created the clock will have time 0.
	// first_beat is the absolute time for the first beat.
	// Future beats will progress at this rate unless set_next_beat
	// is used.
	// 2 * delta = "Delta End" - "Delta Start" as per Figure 1.
	Clock(sf::Time first_beat, sf::Time delta);

	// Gets/Sets the current absolute time
	sf::Time get_time();
	void     set_time(sf::Time t);

	// Checks to see if we have passed the beat as marked by "Beat Passed",
	// "Midpoint of 2 beats", "Delta Start" and "Delta End" in Figure 1.
	// Returns true if so. Will return false after returning true
	// (i.e. a beat passing), until the next beat passes.
	// Thus this should only be called from the main loop.
	bool passed_beat();
	bool passed_antibeat();
	bool passed_delta_start();
	bool passed_delta_end();

	// Returns how far into a beat we are.
	float beat_progress();

	// Returns how far from a beat we are.
	float beat_progress_desc();

	// Returns the amount of time until the next/since the previous
	// "Beat Passed" event as in Figure 1.
	sf::Time until_beat();
	sf::Time since_beat();

	// Returns time until the next delta start event as in Figure 1
	sf::Time until_delta_start();

	// Retrieves the size of the window to either side of the beat within which
	// actions may be performed
	// Note that it is this much time before AND after the beat within which
	// actions can be performed
	sf::Time get_beat_window_delta();

	// Sets when the next beat should be. The time given is an
	// absolute time. From this point on the beats will continue to
	// progress with a time between beats of (next_beat - last_beat)
	void set_next_beat(sf::Time t);

	// Checks if we are between the "Delta Start" and "Delta End"
	// period as per Figure 1.
	bool within_delta();

	// Retrieves the number of beats passed since the creation of this clock
	unsigned int get_beat_number();

   private:
	// Cached time and our underlying clock
	sf::Time  time;
	sf::Clock timer;

	// The time that the last and next "Beat Passed" event are as
	// per Figure 1.
	sf::Time last_beat;
	sf::Time next_beat;

	// 2 * delta = "Delta End" - "Delta Start"
	// as per Figure 1.
	sf::Time delta;

	// Used to stop the passed_delta_XXX() functions from repeat
	// firing.
	bool passed_ab;
	bool passed_ds;
	bool passed_de;

	// The number of beats that have passed so far since the creation of this clock
	unsigned int beat_number;

	// Caches the internal time
	void cache_time();

	// Updates the beat variables. Next beat is updated to be the
	// current (next_beat - last_beat) away from the current
	// next_beat.
	void update_beat();
};
}

#endif
