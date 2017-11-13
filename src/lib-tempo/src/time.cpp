#include <tempo/config.hpp>
#include <tempo/time.hpp>
#include <tempo/network.hpp>

#include <iostream>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

namespace tempo
{
        //public
        Clock::Clock(sf::Time first_beat, sf::Time offset)
        {
                next_beat = first_beat;
                delta = offset;
        }
        
        sf::Time Clock::get_time()
        {
                return time + timer.getElapsedTime();
        }

        void Clock::set_time(sf::Time t, tempo::Song *song)
        {
                cache_time();
                sf::Time delta = t - time;
                std::cout << "Time delta was " << delta.asMicroseconds() << std::endl;
                time = t;
                song->skip(delta);
        }

        void Clock::sync_time(tempo::Song *song)
        {
                set_time(ntp_get_time(this), song);
        }

        bool Clock::passed_beat()
        {
                cache_time();
                if (time > next_beat)
                {
                        update_beat();
                        return true;
                }
                return false;
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

        float Clock::beat_progress()
        {
                return since_beat() / (since_beat() + until_beat());
        }

        sf::Time Clock::until_beat()
        {
                return next_beat - time;
        }

        sf::Time Clock::since_beat()
        {
                return time - last_beat;
        }

        //private
        void Clock::cache_time()
        {
                sf::Time delta = timer.getElapsedTime();
                timer.restart();
                time = time + delta;
        }
        
        void Clock::update_beat()
        {       
                sf::Time delta = next_beat - last_beat;
                last_beat = next_beat;
                next_beat = last_beat + delta;
        }
}
