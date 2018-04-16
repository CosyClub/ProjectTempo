#ifndef CLIENT_SYSTEM_SYSTEMRENDERATTACK_HPP
#define CLIENT_SYSTEM_SYSTEMRENDERATTACK_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentStage.hpp>

#include <client/system/SystemStageRenderer.hpp>

#include <ISceneManager.h>
#include <IParticleSystemSceneNode.h>

#include <map>

namespace std
{

template<>
struct hash<glm::ivec2>
{
	std::size_t operator()(const glm::ivec2& k) const
	{
		static_assert(sizeof(std::size_t) == 2 * sizeof(int),
		              "Expected to be able to store all int bits"
		             );
		return (std::size_t)k.x | ((std::size_t)k.y) << (sizeof(std::size_t)*8);
	}

};

}

namespace client
{
class SystemRenderAttack
	: public anax::System<anax::Requires<tempo::ComponentAttack,
	                                     tempo::ComponentStagePosition,
	                                     tempo::ComponentStageRotation,
	                                     tempo::ComponentStage>
	                      > {

private:
	irr::scene::ISceneManager*                                smgr;
	irr::video::IVideoDriver*                                 driver;
	std::unordered_map<glm::ivec2,
	                   irr::scene::IParticleSystemSceneNode*> particle_systems;
public:
	SystemRenderAttack(irr::video::IVideoDriver * driver,
	                   irr::scene::ISceneManager* smgr);

	void update(SystemStageRenderer& sr);

	void endBeat();
};
}

#endif
