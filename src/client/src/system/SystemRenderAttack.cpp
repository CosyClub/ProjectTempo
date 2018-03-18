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
		printf("Rendering attack intents\n");

		auto& entities = this->getEntities();
		for(auto entity : entities){
			auto& attack = entity.getComponent<tempo::ComponentAttack>();
			if(!attack.isAttacking()){
				continue;
			}
			printf("Found an attacking entity, center: %i, %i\n",
			       attack.damage.ctr.x, attack.damage.ctr.y);
			for(int i = 0; i < attack.damage.positions.size(); ++i){
				printf("  %i, %i\n",
				       attack.damage.positions[i].x, attack.damage.positions[i].y);
			}
		}

		printf("\n\n\n");
	}


}
