#include <client/system/SystemTranslationAnimation.hpp>

#include <anax/Entity.hpp>

namespace client
{


void SystemTranslationAnimation::updateAnimations()
{

	//printf("Doing update of animations\n");
	auto& entities = getEntities();

	for(auto& entity : entities){
		tempo::ComponentStageTranslation& trans = entity.getComponent<tempo::ComponentStageTranslation>();
		client::ComponentRenderSceneNode& node  = entity.getComponent<client::ComponentRenderSceneNode>();

		if(trans.delta.x == 0 && trans.delta.y == 0){ continue; }

		printf("Entity delta: (%i, %i)\n", trans.delta.x, trans.delta.y);

	}
}

} // end of namespace client
