#include <tempo/config.hpp>
#include <tempo/config.hpp>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

namespace tempo
{
        //public
        clock::get_time()
        {
                cache_time();
                return time;
        }

        clock::set_time(sf::Time t)
        {
                cache_time();
                time = t;
        }

        clock::set_next_beat(sf::Time t)
        {
                cache_time();
                next_beat = t;
        }

        clock::until_beat()

        //private
        clock::cache_time()
        {
                sf::Time delta = timer.getElapsetTime();
                timer.restart();
                time = time + delta;
        }
        
        clock::update_beat()
        {       
                cache_time();
                if (time > next_beat)
                {
                        sf::Time delta = next_beat - last_beat;
                        last_beat = next_beat;
                        next_beat = last_beat + delta;
                }
        }
}
