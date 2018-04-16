#include <client/system/SystemRenderAttack.hpp>
#include <IParticleEmitter.h>

namespace client
{

SystemRenderAttack::SystemRenderAttack(irr::video::IVideoDriver * driver,
                                       irr::scene::ISceneManager* smgr)
	: smgr(smgr), driver(driver)
{
	// no-op
}

void SystemRenderAttack::update(SystemStageRenderer& sr)
{

	auto& entities = this->getEntities();
	for(auto entity : entities){
		auto& attack = entity.getComponent<tempo::ComponentAttack>();
		if(!attack.isAttacking()){ continue; }

		auto& pos   = entity.getComponent<tempo::ComponentStagePosition>();
		auto& rot   = entity.getComponent<tempo::ComponentStageRotation>();
		auto& stage = entity.getComponent<tempo::ComponentStage        >();

		auto targets = attack.getAbsolutePositions(pos.getOrigin(), rot.facing);
		

		for (auto& target : targets)
		{
			printf("%d\n", 255/ (attack.beats_until_attack + 1));
			irr::video::SColor color = (255, 0, 0, 255 / (attack.beats_until_attack + 1));
			sr.setTileColor(target, color);
		}
	}

}

void SystemRenderAttack::endBeat()
{
}


}
