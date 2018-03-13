#include <tempo/time.hpp>

namespace tempo
{
/////
// Public
/////
Clock::Clock(sf::Time first_beat, sf::Time delta)
{
	passed_ds   = true;
	passed_de   = false;
	next_beat   = first_beat;
	last_beat   = sf::Time::Zero;
	this->delta = delta;
}

sf::Time Clock::get_time()
{
	return time + timer.getElapsedTime();
}

void Clock::set_time(sf::Time t, tempo::Song *song)
{
	cache_time();
	sf::Time time_to_skip = t - time;
	time                  = t;
	song->skip(time_to_skip);
	update_beat();
}

bool Clock::passed_beat()
{
	cache_time();
	if (time > next_beat) {
		update_beat();
		return true;
	}
	return false;
}

bool Clock::passed_delta_start()
{
	cache_time();
	if (!passed_ds && (time > (next_beat - delta))) {
		passed_ds = true;
		return true;
	}
	return false;
}

bool Clock::passed_delta_end()
{
	cache_time();
	if (!passed_de && (time > (last_beat + delta))) {
		passed_de = true;
		return true;
	}
	return false;
}

float Clock::beat_progress()
{
	return since_beat() / (since_beat() + until_beat());
}

float Clock::beat_progress_desc()
{
	return until_beat() / (since_beat() + until_beat());
}

sf::Time Clock::until_beat()
{
	return next_beat - time;
}

sf::Time Clock::until_delta_start()
{
	return until_beat() - delta;
}

sf::Time Clock::since_beat()
{
	return time - last_beat;
}

void Clock::set_next_beat(sf::Time t)
{
	cache_time();
	next_beat = t;
}

bool Clock::within_delta()
{
	cache_time();
	return (until_beat() < delta || since_beat() < delta);
}

// Private
void Clock::cache_time()
{
	time = time + timer.restart();
}

void Clock::update_beat()
{
	cache_time();
	while (next_beat < time) {
		sf::Time beat_length = next_beat - last_beat;
		last_beat            = next_beat;
		next_beat            = last_beat + beat_length;
	}
	passed_ds = false;
	passed_de = false;
}
}
