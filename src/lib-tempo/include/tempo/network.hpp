////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Application.hpp
/// \author Raef Coles
/// \date 2017/11/13
/// \Functions and classes related to timing
//////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_NETWORK_HPP
#define TEMPO_NETWORK_HPP

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>

#undef main // SDL likes to define main

namespace tempo
{
        #define NTP_PORT 1337
        #define NET_ADDR "127.0.0.1"

        sf::Time ntp_get_time(tempo::Clock *clock);
}

#endif
