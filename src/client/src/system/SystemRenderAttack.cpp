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

void SystemRenderAttack::update()
{

	printf("Rendering attack intents\n");

	auto& entities = this->getEntities();
	for(auto entity : entities){
		auto& attack = entity.getComponent<tempo::ComponentAttack>();
		if(!attack.isAttacking()){ continue; }

		auto& pos   = entity.getComponent<tempo::ComponentStagePosition>();
		auto& rot   = entity.getComponent<tempo::ComponentStageRotation>();
		auto& stage = entity.getComponent<tempo::ComponentStage        >();

		printf("Found an attacking entity, center: %i, %i, size: {%i, %i}\n",
		       attack.damage.ctr.x, attack.damage.ctr.y,
		       attack.damage.positions.size(),
		       attack.damage.mask.size());

		auto targets = attack.getAbsolutePositions(pos.getOrigin(), rot.facing);

		for(int i = 0; i < targets.size(); ++i){
			printf("  %i, %i\n", targets[i].x, targets[i].y);

			if(this->particle_systems.count(targets[i]) != 0){
				// Particle system already exists on this tile, skip it
				continue;
			}

			irr::scene::IParticleSystemSceneNode* psystem = this->smgr->addParticleSystemSceneNode(false);

			irr::scene::IParticleEmitter* emitter = psystem->createBoxEmitter
				(
				 irr::core::aabbox3d<irr::f32>(-0.5f, 0, -0.5f,   0.5f, 1.0f, 0.5f), // size of emitter
				 irr::core::vector3df(0.0f, 0.005f, 0.0f), // direction
				 80, 100,                                  // emit rate
				 irr::video::SColor(0, 200, 200, 200),     // darkest color
				 irr::video::SColor(0, 255, 255, 255),     // brightest color
				 800, 1500,                                // min and max age
				 3.0f,                                     // emit angle
				 irr::core::dimension2df(0.05f, 0.05f),      // min size
				 irr::core::dimension2df(0.2f, 0.2f)       // max size
				 );

			psystem->setEmitter(emitter);
			emitter->drop();

			irr::scene::IParticleAffector* affector_fade = psystem->createFadeOutParticleAffector();

			psystem->addAffector(affector_fade);
			affector_fade->drop();

			psystem->setPosition(irr::core::vector3df(targets[i].x + 0.5f,
			                                          stage.getHeight(pos.getOrigin()),
			                                          targets[i].y + 0.5f));
			psystem->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			psystem->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
			psystem->setMaterialTexture(0, driver->getTexture("resources/materials/textures/particlewhite.bmp"));
			psystem->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);

			this->particle_systems[targets[i]] = psystem;
		}
	}

	printf("\n\n\n");

}

void SystemRenderAttack::endBeat()
{
	printf("Removing all attack particle systems\n");
	for(auto it = particle_systems.begin(); it != particle_systems.end(); ++it)
	{
		it->second->remove();
	}
	particle_systems.clear();
}


}
