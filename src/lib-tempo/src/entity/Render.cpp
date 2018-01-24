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
	ComponentRender::ComponentRender(Ogre::SceneManager* scene, std::string path){
		this->node = scene->getRootSceneNode()->createChildSceneNode();
		this->path = path;
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
			auto& trans = entity.getComponent<ComponentTransform>();
			auto& rend  = entity.getComponent<ComponentRender  >();

			rend.node->setPosition   (trans.position);
			rend.node->setOrientation(trans.rotation);
		}

		app.ogre->renderOneFrame();
	}
}
