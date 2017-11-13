#include <tempo/config.hpp>
#include <tempo/time.hpp>
#include <tempo/network.hpp>

#include <iostream>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>

namespace tempo
{
        sf::Time ntp_get_time(tempo::Clock *clock)
        {
                sf::TcpSocket socket;
                sf::Socket::Status status = socket.connect(NET_ADDR, NTP_PORT);
                if (status != sf::Socket::Done)
                {
                        std::cout << "Error binding socket" << std::endl;
                }

                sf::Int64 t0 = clock->get_time().asMicroseconds();
                sf::Int64 t1;
                sf::Int64 t2;
                sf::Int64 t3;

                sf::Packet packet;
                packet << t0;
                socket.send(packet);

                socket.receive(packet);
                t3 = clock->get_time().asMicroseconds();
                packet >> t1 >> t2;

                sf::Int64 delay  = ((t3 - t0) - (t2 - t1)) / 2;
                return sf::microseconds(t2 + delay);
        }
}
