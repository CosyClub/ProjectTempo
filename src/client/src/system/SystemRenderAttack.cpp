#include <client/system/SystemRenderAttack.hpp>

#include <tempo/component/ComponentPlayerLocal.hpp>

namespace client
{
void SystemRenderAttack::setup(irr::scene::ISceneManager *smgr)
{
	original_size = irr::core::dimension2d<irr::f32>(0.7f, 0.15f);
	irr::core::dimension2d<irr::f32> size(0.7f, 0.15f);
	irr::core::vector3df             pos(-0.3f, 1.6f + size.Height / 2, 0.0f);

	auto entities = getEntities();

	for (auto &entity : entities) {

		auto &attack = entity.getComponent<tempo::ComponentAttack>();
		auto &rend   = entity.getComponent<client::ComponentRenderAttack>();
		auto &sn     = entity.getComponent<client::ComponentRenderSceneNode>();

		if(rend.node != nullptr){
			continue;
		}

		irr::video::SColor colour = (255, 255, 255, 0);
		rend.node = smgr->addBillboardSceneNode(sn.node, size, pos, -1, colour, colour);
		rend.node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		rend.node->grab();
	}
}

void SystemRenderAttack::update()
{

	auto   entities = getEntities();
	double scale;

	for (auto &entity : entities) {

		auto &attack = entity.getComponent<tempo::ComponentAttack>();
		auto &rend   = entity.getComponent<client::ComponentRenderAttack>();
		auto &sn     = entity.getComponent<client::ComponentRenderSceneNode>();

		if(entity.hasComponent<tempo::ComponentPlayerLocal>()){
			if(!attack.isAttacking()){
				printf("NOT ATTACKING\n");
			}
			else{
				printf("ATTACKING\n");
			}
		}
	}
}
}
