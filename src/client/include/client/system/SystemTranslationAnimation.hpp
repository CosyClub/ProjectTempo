#ifndef CLIENT_SYSTEM_TRANSLATION_ANIMATION_HPP
#define CLIENT_SYSTEM_TRANSLATION_ANIMATION_HPP

#include <anax/Entity.hpp>
#include <anax/System.hpp>

#include <client/component/ComponentRenderSceneNode.hpp>
#include <tempo/component/ComponentStage.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>

#include <ISceneManager.h>
#include <ISceneNodeAnimator.h>
#include <IVideoDriver.h>
#include <IrrlichtDevice.h>

#include <unordered_map>
#include <utility>

namespace tempo
{
class Clock;
}

namespace client
{
class SystemTranslationAnimation
    : public anax::System<anax::Requires<tempo::ComponentStageTranslation,
                                         tempo::ComponentStagePosition,
                                         tempo::ComponentStage,
                                         client::ComponentRenderSceneNode>>
{
   private:
	anax::World* world;
	irr::IrrlichtDevice* device;
	tempo::Clock& clock;
	std::unordered_map<anax::Entity::Id::int_type, bool> animators;

   public:
	SystemTranslationAnimation(anax::World* world,
	                           irr::IrrlichtDevice* device,
	                           tempo::Clock& clock);

	void updateAnimations();
	void endBeat();
};

}  // namespace client

#endif
