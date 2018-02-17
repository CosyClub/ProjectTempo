#ifndef TEMPO_ENTITY_ENTITYCREATIONCLIENT_HPP
#define TEMPO_ENTITY_ENTITYCREATIONCLIENT_HPP

#include <anax/World.hpp>

#include <Ogre.h>

#include <tempo/entity/EntityCreation.hpp>
#include <tempo/system/SystemLevelManager.hpp>

#include <glm/fwd.hpp>

namespace tempo
{

EntityCreationData* newEntity(int type_id, glm::vec2 pos);

anax::Entity newEntity(EntityCreationData data,
                       anax::World& world,
                       Ogre::SceneManager* scene,
                       tempo::SystemLevelManager system_gm);

anax::Entity newPlayer(anax::World& world, Ogre::SceneManager* scene, 
                       int iid, 
                       EID tid,
                       unsigned char playerLocal,
                       int x, 
                       int y, 
                       int health,
                       tempo::SystemLevelManager system_grid_motion);

anax::Entity newAI(anax::World& world, 
                   Ogre::SceneManager* scene, 
                   int iid, 
                   EID tid, 
                   int x, 
                   int y);

anax::Entity newDestroyable(anax::World& world, 
                            Ogre::SceneManager* scene, 
                            int iid, 
                            EID tid, 
                            int x, 
                            int y, 
                            std::string mesh_name);

anax::Entity newNonDestroyable(anax::World& world, 
                               Ogre::SceneManager* scene, 
                               int iid, 
                               EID tid, 
                               int x, 
                               int y, 
                               std::string mesh_name);

} // namespace tempo

#endif
