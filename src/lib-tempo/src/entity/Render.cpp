////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Render.cpp
/// \author Jamie Terry
/// \date 2017/11/14
/// \brief Contains definition of Render system functions
////////////////////////////////////////////////////////////////////////////

#include <tempo/entity/Render.hpp>

namespace tempo{
	ComponentRender::ComponentRender(Ogre::SceneManager* scene){
		this->node = scene->getRootSceneNode()->createChildSceneNode();
	}

	ComponentRender::~ComponentRender(){
		this->node->getCreator()->destroySceneNode(this->node);
	}

	SystemRender::SystemRender(Application& new_app) : app(new_app) {
	  scene = app.ogre->createSceneManager(Ogre::ST_GENERIC);
	}

	SystemRender::~SystemRender(){
		app.ogre->destroySceneManager(scene);
	}

	void SystemRender::render(float dt){
		auto entities = getEntities();

		for(auto& entity : entities){
			//printf("SystemRender   is updating entity %i\n", entity.getId());
			auto& pos  = entity.getComponent<ComponentPosition>();
			auto& rend = entity.getComponent<ComponentRender  >();

			//printf("Setting position to: %f, %f, %f\n", pos.position.x, pos.position.y, pos.position.z);

			rend.node->setPosition(pos.position);
		}

		app.ogre->renderOneFrame();
	}
}
