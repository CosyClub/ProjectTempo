#ifndef TEMPO_ENTITY_ENTITYCREATIONSERVER_HPP
#define TEMPO_ENTITY_ENTITYCREATIONSERVER_HPP

#include <anax/World.hpp>

#include <tempo/entity/EntityCreation.hpp>
#include <tempo/entity/ID.hpp>
#include <tempo/entity/LevelManager.hpp>
#include <tempo/entity/GridAi.hpp>
#include <tempo/entity/PlayerRemote.hpp>

namespace tempo
{

EntityCreationData dumpEntity(anax::Entity e);

anax::Entity newPlayer(anax::World& world, EID tid, SystemLevelManager system_grid_motion);
anax::Entity newAI(anax::World& world, EID tid, int x, int y);
anax::Entity newDestroyable(anax::World& world, EID tid, int x, int y, std::string mesh_name);
anax::Entity newNonDestroyable(anax::World& world, EID tid, int x, int y, std::string mesh_name);

}

#endif


