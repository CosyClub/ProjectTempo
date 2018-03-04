#ifndef TEMPO_ENTITY_ENTITYCREATION_HPP
#define TEMPO_ENTITY_ENTITYCREATION_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <SFML/Network.hpp>

namespace tempo
{
anax::Entity addComponent(anax::World &w, sf::Packet p);

}  // namespace tempo

#endif
