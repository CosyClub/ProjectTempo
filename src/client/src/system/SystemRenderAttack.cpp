#include <client/system/SystemRenderAttack.hpp>
#include <IParticleEmitter.h>

namespace client
{

SystemRenderAttack::SystemRenderAttack()
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

		auto targets = attack.getAbsolutePositions(pos.getOrigin(), rot.facing);

		for (auto& target : targets)
		{
			irr::video::SColor color(255, 0, 0, 255 / (attack.beats_until_attack + 1));
			sr.setTileColor(target, color);
		}
	}

}


}
