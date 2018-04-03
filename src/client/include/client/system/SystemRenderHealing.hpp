#ifndef CLIENT_SYSTEM_SYSTEMRENDERHEALING_HPP
#define CLIENT_SYSTEM_SYSTEMRENDERHEALING_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStage.hpp>
#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentHealth.hpp>

#include <ISceneManager.h>
#include <IParticleSystemSceneNode.h>

#include <unordered_map>
#include <vector>
#include <utility>

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
class SystemRenderHealing
	: public anax::System<anax::Requires<tempo::ComponentStagePosition,
	                                     tempo::ComponentStage,
																			 tempo::ComponentHealth,
																			 tempo::ComponentCombo>> {

private:
	irr::scene::ISceneManager*                                smgr;
	irr::video::IVideoDriver*                                 driver;
	//std::unordered_map<glm::ivec2,
	//                   irr::scene::IParticleSystemSceneNode*> particle_systems;
	std::vector<std::pair<irr::scene::IParticleSystemSceneNode*,int>> particle_systems;
public:
	SystemRenderHealing(irr::video::IVideoDriver * driver,
	                   irr::scene::ISceneManager* smgr);

	void update();

	void endBeat();
};
}

#endif
