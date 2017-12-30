#ifndef TEMPO_ENTITY_ENTITYCREATION_HPP
#define TEMPO_ENTITY_ENTITYCREATION_HPP

#include <anax/World.hpp>

#include <Ogre.h>

#include <tempo/entity/Render.hpp>
#include <tempo/entity/LevelManager.hpp>
#include <tempo/entity/GridAi.hpp>
#include <tempo/entity/PlayerInput.hpp>

anax::Entity newPlayer(anax::World& world, Ogre::SceneManager* scene, tempo::SystemLevelManager system_grid_motion);
anax::Entity newAI(anax::World& world, Ogre::SceneManager* scene, int x, int y);
anax::Entity newDestroyable(anax::World& world, Ogre::SceneManager* scene, int x, int y, std::string mesh_name);

#endif
