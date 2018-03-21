#include <client/system/SystemRenderAttack.hpp>

namespace client
{
	SystemRenderAttack::SystemRenderAttack(irr::scene::ISceneManager* smgr)
		: smgr(smgr)
	{
		// no-op
	}

	void SystemRenderAttack::update()
	{

		auto& entities = this->getEntities();
		for(auto entity : entities){
			auto& attack = entity.getComponent<tempo::ComponentAttack>();
			if(!attack.isAttacking()){
				continue;
			}
			printf("Rendering attack intents\n");
			printf("Found an attacking entity, center: %i, %i, size: {%i, %i}\n",
			       attack.damage.ctr.x, attack.damage.ctr.y,
			       attack.damage.positions.size(),
			       attack.damage.mask.size());
			printf("\n\n\n");
			for(int i = 0; i < attack.damage.positions.size(); ++i){
				printf("  %i, %i\n",
				       attack.damage.positions[i].x, attack.damage.positions[i].y);
			}
		}

	}


}
