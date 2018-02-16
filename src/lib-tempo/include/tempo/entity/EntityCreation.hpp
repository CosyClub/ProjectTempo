#ifndef TEMPO_ENTITY_ENTITYCREATION_HPP
#define TEMPO_ENTITY_ENTITYCREATION_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/network/QueueID.hpp>

#include <iostream>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <SFML/Network.hpp>

namespace tempo
{

anax::Entity addComponent(anax::World& w, sf::Packet p);

sf::Packet& operator <<(sf::Packet& packet, const anax::Entity::Id id);
sf::Packet& operator >>(sf::Packet& packet, anax::Entity::Id& id);

}

#endif
