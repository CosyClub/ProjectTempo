#include <tempo/config.hpp>
#include <tempo/time.hpp>
#include <tempo/song.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Audio.hpp>

namespace tempo
{
        class Song
        {
                public:
                Song(std::string path);
                void start();
                void pause();
                void stop();
                void skip(sf::time delta);

                private:
                sf::time start_time;
        }
}
