#include <client/system/SystemTranslationAnimation.hpp>
#include <tempo/time.hpp>

#include <anax/World.hpp>

#include <ISceneNodeAnimator.h>

#include <glm/vec2.hpp>
#include <glm/glm.hpp>

namespace {
	class HopAnimator : public irr::scene::ISceneNodeAnimator {
	private:
		irr::u32   start_time;
		irr::u32   animation_time;
		glm::ivec2 start;
		glm::ivec2 delta;
		float      hop_height;
	public:
		/// \param time The irrlicht device time (in ms) when this animator is created
		/// \param animation_time The length of the animation in ms
		/// \param start The initial location of the entity
		/// \param delta The final location of the entity
		HopAnimator(irr::u32 time,
		            irr::u32 animation_time,
		            glm::ivec2 start,
		            glm::ivec2 delta,
		            float hop_height = 0.0f)
			: start_time(time), animation_time(animation_time),
			  start(start), delta(delta),
			  hop_height(hop_height)
		{
			// no-op
		}

		void animateNode(irr::scene::ISceneNode* node, irr::u32 time){
			// printf("Calling animate hop: %i\n", time);
			float progress = (float)(time - start_time) / (float)animation_time;
			printf("Hop progress: %f\n", progress);

			irr::core::vector3df old_pos = node->getPosition();
			printf("  old %f, %f, %f\n", old_pos.X, old_pos.Y, old_pos.Z);

			glm::fvec3 pos_start;
			pos_start.x = start.x;
			pos_start.z = start.y;
			pos_start.y = 0;
			printf("  sta %f, %f, %f\n", pos_start.x, pos_start.y, pos_start.z);

			glm::fvec3 pos_end;
			pos_end.x = start.x + delta.x;
			pos_end.z = start.y + delta.y;
			pos_end.y = 0;
			printf("  end %f, %f, %f\n", pos_end.x, pos_end.y, pos_end.z);

			glm::fvec3 pos = glm::mix(pos_start, pos_end, progress);
			printf("  new %f, %f, %f\n", pos.x, pos.y, pos.z);

			// Add sinosodial hop motion
			float a = progress - 0.5f;
			pos.y += (-(a*a) + 0.25f) * hop_height * 2.0f;

			node->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
		}

		irr::scene::ISceneNodeAnimator*
		createClone(irr::scene::ISceneNode* node,
		            irr::scene::ISceneManager* manager){
			return new HopAnimator(this->start_time, this->animation_time,
			                       this->start, this->delta, this->hop_height);
		}
	};
}

namespace client
{

SystemTranslationAnimation::SystemTranslationAnimation(anax::World* world,
                                                       irr::IrrlichtDevice* device,
                                                       tempo::Clock& clock)
	: world(world), device(device), clock(clock)
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
			new HopAnimator(now, clock.until_beat().asMilliseconds(),
			                pos.getOrigin(), trans.delta, 1.0f);

		printf("Add animator for entity %i, delta: (%i, %i), origin: (%i ,%i)\n",
		       entity_id, trans.delta.x, trans.delta.y, pos.getOrigin().x, pos.getOrigin().y
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
