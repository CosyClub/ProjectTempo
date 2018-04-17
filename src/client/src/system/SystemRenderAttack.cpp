#include <client/system/SystemRenderAttack.hpp>

#include <tempo/component/ComponentPlayerLocal.hpp>

namespace client
{
void SystemRenderAttack::setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver * driver, irr::IrrlichtDevice *device)
{
	original_size = irr::core::dimension2d<irr::f32>(0.7f, 0.15f);
	irr::core::dimension2d<irr::f32> size(3.0f, 3.0f);
	irr::core::vector3df             pos(0.0f, 0.3f, 0.0f);

	std::string aoePath = "resources/materials/textures/aoe-animation.png";
	irr::video::ITexture *spritesheet = driver->getTexture(aoePath.c_str());
	driver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT,true);
	auto entities = getEntities();

	for (auto &entity : entities) {

		auto &rend   = entity.getComponent<client::ComponentRenderAttack>();
		auto &sn     = entity.getComponent<client::ComponentRenderSceneNode>();

		if(rend.node != nullptr){
			continue;
		}

		irr::video::SColor colour;
		rend.posx = 1.f / 3;

		rend.node = smgr->addBillboardSceneNode(sn.node, size, pos, -1, colour, colour);
		rend.node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		rend.node->setMaterialType( irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL );
		rend.node->setMaterialTexture(0, spritesheet);
		rend.node->getMaterial(0).getTextureMatrix(0).setTextureScale(rend.posx, 1);
		rend.node->getMaterial(0).getTextureMatrix(0).setTextureTranslate(rend.posx, 1);

		irr::core::vector3df rotation = irr::core::vector3df(0.25f,0,0.25f);

		rend.node->setRotation(rotation);
		rend.node->grab();
	}

	this->oldTime = device->getTimer()->getRealTime();
}

void SystemRenderAttack::update(irr::IrrlichtDevice *device)
{

	auto   entities = getEntities();
	bool timeUpdated = false;
	this->now = device->getTimer()->getRealTime();
	this->elapsed = this->now - this->oldTime;

	for (auto &entity : entities) {

		auto &attack = entity.getComponent<tempo::ComponentAttack>();
		auto &rend   = entity.getComponent<client::ComponentRenderAttack>();

		if(entity.hasComponent<tempo::ComponentPlayerLocal>()){
			if(attack.isAttacking()){
				rend.node->getMaterial(0).getTextureMatrix(0).setTextureTranslate(rend.posx, 1);
				if(this->elapsed > 20){
					rend.posx = rend.posx + (1.f/ 3);
					timeUpdated = true;
				}
			}
			else{
				rend.posx = 1.f / 3;
			}
		}
	}

	if(timeUpdated){
		this->oldTime = device->getTimer()->getRealTime();
		timeUpdated = false;
	}

}
}
