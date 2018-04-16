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
			sr.setTileColor(target, irr::video::SColor(255, 0, 0, 255));
		}
	}

}

void SystemRenderAttack::endBeat()
{
}


}
