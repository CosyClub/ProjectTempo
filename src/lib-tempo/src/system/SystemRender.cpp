#include <tempo/entity/SystemRender.hpp>
#include <tempo/entity/ComponentRender.hpp>

namespace tempo{

	SystemRender::SystemRender(Application& new_app) : app(new_app) {
	  scene = app.ogre->createSceneManager(Ogre::ST_GENERIC);
	}

	SystemRender::~SystemRender(){
		app.ogre->destroySceneManager(scene);
	}

	void SystemRender::render(float dt){
		auto entities = getEntities();

		for(auto& entity : entities){
			auto& trans = entity.getComponent<ComponentTransform>();
			auto& rend  = entity.getComponent<ComponentRender  >();

			rend.node->setPosition   (trans.position);
			rend.node->setOrientation(trans.rotation);
		}

		app.ogre->renderOneFrame();
	}
}
