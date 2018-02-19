#ifndef TEMPO_ENTITY_ENTITYCREATION_HPP
#define TEMPO_ENTITY_ENTITYCREATION_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/network/base.hpp>
#include <tempo/network/QueueID.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>

#include <iostream>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <SFML/Network.hpp>

namespace tempo
{

anax::Entity addComponent(anax::World& w, sf::Packet p);

}

#endif
