#ifndef TEMPO_ENTITY_ENTITYCREATIONSERVER_HPP
#define TEMPO_ENTITY_ENTITYCREATIONSERVER_HPP

#include <anax/World.hpp>

#include <tempo/entity/EntityCreation.hpp>
#include <tempo/component/ComponentAI.hpp>
#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentStage.hpp>
#include <tempo/component/ComponentAOEIndicator.hpp>
#include <tempo/component/ComponentButtonGroup.hpp>
#include <tempo/component/ComponentSpikes.hpp>
#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentModel.hpp>
#include <tempo/component/ComponentParty.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/component/ComponentRespawn.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentTeam.hpp>
#include <tempo/component/ComponentWeapon.hpp>

#include <tempo/mask.hpp>

#include <string.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>



namespace tempo
{
anax::Entity newPlayer(anax::World &world, uint32_t party_number);

anax::Entity createMobStill(anax::World &world, glm::ivec2 pos);
anax::Entity createMobStillAOE(anax::World &world, glm::ivec2 pos);
anax::Entity createMobCreeper(anax::World &world, glm::ivec2 pos);
anax::Entity createMobPatroller(anax::World &world, glm::ivec2 pos, std::deque<glm::ivec2> path);
anax::Entity createMobAntiSnail(anax::World &world, glm::ivec2 pos);
anax::Entity createSnake(anax::World& world, glm::ivec2 pos, tempo::Facing f, int len);
void         createSpikes(anax::World &world, std::vector<glm::ivec2> spikes);
anax::Entity createButtonGroup(anax::World &           world,
                               std::vector<glm::ivec2> positions,
                               std::vector<glm::ivec2> tiles,
                               std::vector<glm::ivec2> spikes,
                               glm::ivec2              respawn_pos,
                               glm::ivec2              prev,
                               glm::ivec2              next,
                               bool                    triggerable,
                               int                     ID);

}  // namespace tempo

#endif
