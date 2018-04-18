#include <client/system/SystemRenderAttack.hpp>
#include <IParticleEmitter.h>

namespace client
{

SystemRenderAttack::SystemRenderAttack()
{
	// no-op
}

void SystemRenderAttack::update(SystemStageRenderer& sr, irr::video::SColor colour)
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
			irr::video::SColor color(255,
					         colour.getRed()   / (attack.beats_until_attack + 1), 
					         colour.getGreen() / (attack.beats_until_attack + 1), 
					         colour.getBlue()  / (attack.beats_until_attack + 1));
			sr.setTileColor(target, color);
		}
	}

}


}
