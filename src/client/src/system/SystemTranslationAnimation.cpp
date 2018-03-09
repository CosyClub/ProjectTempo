#include <client/system/SystemTranslationAnimation.hpp>

#include <anax/World.hpp>

#include <ISceneNodeAnimator.h>

#include <glm/vec2.hpp>

namespace {
	class HopAnimator : public irr::scene::ISceneNodeAnimator {
	private:
		irr::u32   start_time;
		glm::ivec2 start;
		glm::ivec2 delta;
	public:
		HopAnimator(irr::u32 time, glm::ivec2 start, glm::ivec2 delta)
			: start_time(time), start(start), delta(delta) {
			// no-op
		}

		void animateNode(irr::scene::ISceneNode* node, irr::u32 time){
			// printf("Calling animate hop: %i\n", time);
		}

		irr::scene::ISceneNodeAnimator*
		createClone(irr::scene::ISceneNode* node,
		            irr::scene::ISceneManager* manager){
			return new HopAnimator(this->start_time, this->start, this->delta);
		}
	};
}

namespace client
{

SystemTranslationAnimation::SystemTranslationAnimation(anax::World* world,
                                                       irr::IrrlichtDevice* device)
	: world(world), device(device)
{
	// no-op
}


void SystemTranslationAnimation::updateAnimations()
{
	irr::u32 now = device->getTimer()->getTime();

	//printf("Doing update of animations\n");
	auto& entities = getEntities();

	for(auto& entity : entities){
		anax::Entity::Id::int_type entity_id = entity.getId().index;

		tempo::ComponentStageTranslation& trans =
			entity.getComponent<tempo::ComponentStageTranslation>();
		if(trans.delta.x == 0 && trans.delta.y == 0){ continue; }

		if(animators.count(entity_id) > 0){
			// Then we've already added an animator to this entity
			printf("Already have animator for entity: %i\n", entity_id);
			continue;
		}

		tempo::ComponentStagePosition&    pos    =
			entity.getComponent<tempo::ComponentStagePosition>();
		client::ComponentRenderSceneNode& node   =
			entity.getComponent<client::ComponentRenderSceneNode>();

		irr::scene::ISceneNodeAnimator* animator =
			new HopAnimator(now, pos.getOrigin(), trans.delta);

		printf("Add animator for entity %i, delta: (%i, %i)\n",
		       entity_id, trans.delta.x, trans.delta.y
		      );

		animators[entity_id] = animator;
		node.node->addAnimator(animator);
	}
}

void SystemTranslationAnimation::endBeat(){
	for(auto it = animators.begin(); it != animators.end(); ++it){
		anax::Entity entity = world->getEntity(it->first);

		printf("Removing hop animator for entity %i\n", entity.getId().index);

		auto& node = entity.getComponent<client::ComponentRenderSceneNode>();
		node.node->removeAnimator(it->second);
	}
	animators.clear();
}

} // end of namespace client
