#ifndef TEMPO_ENTITY_ENTITYCREATIONSERVER_HPP
#define TEMPO_ENTITY_ENTITYCREATIONSERVER_HPP

#include <anax/World.hpp>

#include <tempo/entity/EntityCreation.hpp>

#include <tempo/system/SystemLevelManager.hpp>

#include <tempo/component/ComponentAI.hpp>
#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentStage.hpp>
#include <tempo/component/ComponentAOEIndicator.hpp>
#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentModel.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentWeapon.hpp>

#include <tempo/mask.hpp>

#include <string.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace tempo
{
anax::Entity newPlayer(anax::World &world);

// anax::Entity newAI(anax::World &world, int x, int y);

anax::Entity createMobStill(anax::World &world, glm::ivec2 pos);

anax::Entity newDestroyable(anax::World &world, int x, int y, std::string mesh_name);

anax::Entity newNonDestroyable(anax::World &world, int x, int y, std::string mesh_name);

}  // namespace tempo

#endif
