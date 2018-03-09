#ifndef CLIENT_SYSTEM_TRANSLATION_ANIMATION_HPP
#define CLIENT_SYSTEM_TRANSLATION_ANIMATION_HPP

#include <anax/System.hpp>

#include <tempo/component/ComponentStageTranslation.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>

#include <ISceneManager.h>
#include <IVideoDriver.h>

namespace client
{

class SystemTranslationAnimation
	: public anax::System<anax::Requires<tempo::ComponentStageTranslation,
	                                     client::ComponentRenderSceneNode>
	                      >
{
public:
	void updateAnimations();
};

}  // namespace client

#endif
