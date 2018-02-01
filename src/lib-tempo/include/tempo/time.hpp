#ifndef TEMPO_TIME_HPP
#define TEMPO_TIME_HPP

#include <tempo/song.hpp>
#include <tempo/config.hpp>

#include <SFML/Network.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

namespace tempo
{
        class Clock
        {
                public:
                //base stuff
                Clock(sf::Time first_beat, sf::Time offset);
                sf::Time get_time();
                void set_time(sf::Time t, tempo::Song *song);

                //beat relate
                bool passed_beat();
                void set_next_beat(sf::Time t);
                bool within_delta();
                float beat_progress();
                sf::Time until_beat();
                sf::Time since_beat();

                private:
                sf::Time time;
                sf::Clock timer;

                sf::Time last_beat;
                sf::Time next_beat;
                sf::Time delta;

                void cache_time();
                void update_beat();
        };
}

#endif
