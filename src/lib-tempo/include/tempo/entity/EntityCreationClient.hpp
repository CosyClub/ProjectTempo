#ifndef TEMPO_ENTITY_ENTITYCREATIONCLIENT_HPP
#define TEMPO_ENTITY_ENTITYCREATIONCLIENT_HPP

#include <anax/World.hpp>

#include <Ogre.h>

#include <tempo/component/ComponentGridAi.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentID.hpp>
#include <tempo/component/ComponentRender.hpp>
#include <tempo/entity/EntityCreation.hpp>
#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/entity/PlayerRemote.hpp>
#include <tempo/system/SystemLevelManager.hpp>

namespace tempo
{

EntityCreationData* newEntity(int type_id, Vec2s pos);

anax::Entity newEntity(EntityCreationData data,
                       anax::World& world,
                       Ogre::SceneManager* scene,
                       tempo::SystemLevelManager system_gm);

anax::Entity newPlayer(anax::World& world, Ogre::SceneManager* scene, int iid, EID tid, int x, int y, tempo::SystemLevelManager system_grid_motion);
anax::Entity newAI(anax::World& world, Ogre::SceneManager* scene, int iid, EID tid, int x, int y);
anax::Entity newDestroyable(anax::World& world, Ogre::SceneManager* scene, int iid, EID tid, int x, int y, std::string mesh_name);
anax::Entity newNonDestroyable(anax::World& world, Ogre::SceneManager* scene, int iid, EID tid, int x, int y, std::string mesh_name);

EntityCreationData dumpEntity(anax::Entity e);
}

#endif
