#include <client/system/SystemRenderHealing.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>
#include <IParticleEmitter.h>

namespace client
{

SystemRenderHealing::SystemRenderHealing(irr::video::IVideoDriver * driver,
                                       irr::scene::ISceneManager* smgr)
	: smgr(smgr), driver(driver)
{
	// no-op
}

void SystemRenderHealing::update()
{

	auto& entities = this->getEntities();
	for(auto entity : entities) {

		auto& h     = entity.getComponent<tempo::ComponentHealth>();
		auto& c     = entity.getComponent<tempo::ComponentCombo>();
		auto& pos   = entity.getComponent<tempo::ComponentStagePosition>();
		auto& stage = entity.getComponent<tempo::ComponentStage        >();
		auto &sn    = entity.getComponent<client::ComponentRenderSceneNode>();

		if(c.comboCounter > 20 && h.current_health != h.max_health)
		 {
			irr::scene::IParticleSystemSceneNode* psystem = this->smgr->addParticleSystemSceneNode(false, sn.node);

			irr::scene::IParticleEmitter* emitter = psystem->createBoxEmitter
				(
				 irr::core::aabbox3d<irr::f32>(-0.5f, 0, -0.5f,   0.5f, 1.0f, 0.5f), // size of emitter
				 irr::core::vector3df(0.0f, 0.0001f, 0.0f), // direction
				 10, 25,                                    // emit rate
				 irr::video::SColor(0, 000, 200, 40),        // darkest color
				 irr::video::SColor(0, 000, 255, 70),        // brightest color
				 800, 1500,                                 // min and max age
				 50.0f,                                     // emit angle
				 irr::core::dimension2df(0.10f, 0.10f),     // min size
				 irr::core::dimension2df(0.35f, 0.35f)        // max size
				 );

			psystem->setEmitter(emitter);
			emitter->drop();

			irr::scene::IParticleAffector* affector_fade = psystem->createFadeOutParticleAffector();

			psystem->addAffector(affector_fade);
			affector_fade->drop();

			psystem->setPosition(psystem->getPosition() + irr::core::vector3df(0.1f,0.1f,0.f));
			psystem->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			psystem->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
			psystem->setMaterialTexture(0, driver->getTexture("resources/materials/textures/particlewhite.bmp"));
			psystem->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);

			this->particle_systems.push_back(std::make_pair(psystem,0));
		}
	}

}

void SystemRenderHealing::endBeat()
{

	for(auto it = particle_systems.begin(); it != particle_systems.end(); ++it)
	{
		if((*it).second == 3) {
			(*it).first->remove();
		}
		(*it).second +=1;
	}

	auto it = particle_systems.begin();

	while(it != particle_systems.end()) {

    if((*it).second == 4) {

        it = particle_systems.erase(it);
    }
    else ++it;
	}

}


}
