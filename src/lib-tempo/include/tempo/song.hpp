////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Application.hpp
/// \author Raef Coles
/// \date 2017/11/13
/// \Functions and classes related to timing
//////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_SONG_HPP
#define TEMPO_SONG_HPP

#include <SFML/System/Time.hpp>
#include <SFML/Audio.hpp>

#undef main // SDL likes to define main

namespace tempo
{
        class Song
        {
                public:
                Song(std::string path);
                void start();
                void pause();
                void stop();
                void skip(sf::Time delta);

                private:
                sf::SoundBuffer buffer;
                sf::Sound sound;
        };
}

#endif
