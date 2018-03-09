#ifndef CLIENT_SYSTEM_TRANSLATION_ANIMATION_HPP
#define CLIENT_SYSTEM_TRANSLATION_ANIMATION_HPP

#include <anax/System.hpp>
#include <anax/Entity.hpp>

#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>

#include <IrrlichtDevice.h>
#include <ISceneManager.h>
#include <ISceneNodeAnimator.h>
#include <IVideoDriver.h>

#include <map>

namespace client
{

class SystemTranslationAnimation
	: public anax::System<anax::Requires<tempo::ComponentStageTranslation,
	                                     tempo::ComponentStagePosition,
	                                     client::ComponentRenderSceneNode>
	                      >
{
private:
	irr::IrrlichtDevice* device;
	anax::World*         world;
	std::map<anax::Entity::Id::int_type, irr::scene::ISceneNodeAnimator*> animators;
public:
	SystemTranslationAnimation(anax::World* world, irr::IrrlichtDevice* device);

	void updateAnimations();
	void endBeat();
};

}  // namespace client

#endif
