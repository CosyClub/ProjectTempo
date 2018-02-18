#ifndef TEMPO_ENTITY_ENTITYCREATIONSERVER_HPP
#define TEMPO_ENTITY_ENTITYCREATIONSERVER_HPP

#include <anax/World.hpp>

#include <tempo/component/ComponentGridAi.hpp>
#include <tempo/entity/EntityCreation.hpp>
#include <tempo/component/ComponentPlayerRemoteServer.hpp>
#include <tempo/system/SystemLevelManager.hpp>

#include <string.h>

namespace tempo
{

anax::Entity newPlayer(anax::World& world, SystemLevelManager system_grid_motion);
anax::Entity newAI(anax::World& world, int x, int y);
anax::Entity newDestroyable(anax::World& world, int x, int y, std::string mesh_name);
anax::Entity newNonDestroyable(anax::World& world, int x, int y, std::string mesh_name);

}

#endif


