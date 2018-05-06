#include <client/system/SystemTranslationAnimation.hpp>
#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/time.hpp>

#include <anax/World.hpp>

#include <ISceneNodeAnimator.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <iostream>

namespace
{
class TranslateAnimator : public irr::scene::ISceneNodeAnimator
{
   private:
	irr::u32 start_time;
	irr::u32 animation_time;
	glm::fvec3 initial;
	glm::fvec3 target;

   public:
	/// \param time The irrlicht device time (in ms) when this animator is created
	/// \param animation_time The length of the animation in ms
	/// \param initial The initial location of the entity
	/// \param target  The final location of the entity
	TranslateAnimator(irr::u32 time,
	                  irr::u32 animation_time,
	                  glm::fvec3 initial,
	                  glm::fvec3 target,
	                  float hop_height = 0.0f)
	    : start_time(time), animation_time(animation_time), initial(initial), target(target)
	{
	}

	void animateNode(irr::scene::ISceneNode* node, irr::u32 time)
	{
		// printf("Calling animate translate: %i\n", time);
		float progress = glm::clamp((float)(time - start_time) / (float)animation_time, 0.0f, 1.0f);
		glm::fvec3 pos = glm::mix(initial, target, progress);

		node->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
	}

	irr::scene::ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node,
	                                            irr::scene::ISceneManager* manager)
	{
		return new TranslateAnimator(this->start_time, this->animation_time, this->initial,
		                             this->target);
	}
};

class YHopAnimator : public irr::scene::ISceneNodeAnimator
{
   private:
	irr::u32 start_time;
	irr::u32 animation_time;
	glm::fvec3 initial;
	float hop_height;

   public:
	/// \param time The irrlicht device time (in ms) when this animator is created
	/// \param animation_time The length of the animation in ms
	/// \param initial The initial location of the entity
	/// \param target  The final location of the entity
	YHopAnimator(irr::u32 time,
	             irr::u32 animation_time,
	             glm::fvec3 initial,
	             float hop_height = 0.0f)
	    : start_time(time), animation_time(animation_time), initial(initial), hop_height(hop_height)
	{
	}

	void animateNode(irr::scene::ISceneNode* node, irr::u32 time)
	{
		// printf("Calling animate hop: %i\n", time);
		float progress = glm::clamp((float)(time - start_time) / (float)animation_time, 0.0f, 1.0f);
		glm::fvec3 pos = glm::vec3(initial);

		// Add sinosodial hop motion
		float a = progress - 0.5f;
		pos.y += (-(a * a) + 0.25f) * hop_height * 2.0f;

		node->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
	}

	irr::scene::ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node,
	                                            irr::scene::ISceneManager* manager)
	{
		return new YHopAnimator(this->start_time, this->animation_time, this->initial,
		                        this->hop_height);
	}
};
}  // namespace

namespace client
{
SystemTranslationAnimation::SystemTranslationAnimation(anax::World* world,
                                                       irr::IrrlichtDevice* device,
                                                       tempo::Clock& clock)
    : world(world), device(device), clock(clock)
{
}


void SystemTranslationAnimation::updateAnimations()
{
	irr::u32 now = device->getTimer()->getTime();

	// printf("Doing update of animations\n");
	auto& entities = getEntities();

	for (auto& entity : entities) {
		anax::Entity::Id::int_type entity_id = entity.getId().index;

		tempo::ComponentStageTranslation& trans =
		  entity.getComponent<tempo::ComponentStageTranslation>();
		if (!trans.moved) {
			continue;
		}

		client::ComponentRenderSceneNode& node =
			entity.getComponent<client::ComponentRenderSceneNode>();

		if (animators.count(entity_id) > 0) {
			// Then we've already added an animator to this entity
			node.node->removeAnimators();
			node.billboard->removeAnimators();
		}

		tempo::ComponentStagePosition& pos = entity.getComponent<tempo::ComponentStagePosition>();
		tempo::ComponentStage& stage = entity.getComponent<tempo::ComponentStage>();


		irr::core::vector3df bp = node.billboard->getPosition();
		irr::scene::ISceneNodeAnimator* hop =
			new YHopAnimator(now, clock.until_delta_start().asMilliseconds(),
				glm::fvec3(bp.X, bp.Y, bp.Z),
				1.0f);
		node.billboard->addAnimator(hop);

		glm::ivec2 origin = pos.getOrigin();
		glm::ivec2 target = origin + trans.delta;

		irr::scene::ISceneNodeAnimator* move = 
			new TranslateAnimator(now, clock.until_delta_start().asMilliseconds(),
				glm::fvec3(origin.x, stage.getHeight(origin), origin.y),
				glm::fvec3(target.x, stage.getHeight(target), target.y));
		node.node->addAnimator(move);

		animators[entity_id] = true;
	}
}

void SystemTranslationAnimation::endBeat()
{
	for (auto it = animators.begin(); it != animators.end(); ++it) {
		anax::Entity entity = world->getEntity(it->first);

		auto& node = entity.getComponent<client::ComponentRenderSceneNode>();
		// removeAnimators not used in case other animations added
		node.node->removeAnimators();
		node.billboard->removeAnimators();
	}
	animators.clear();
}

}  // end of namespace client
