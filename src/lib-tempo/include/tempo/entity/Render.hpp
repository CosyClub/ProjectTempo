////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Render.hpp
/// \author Jamie Terry
/// \date 2017/11/14
/// \brief Declares the the Render component and system
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_RENDER_HPP
#define TEMPO_ENTITY_RENDER_HPP

#include <OgreRoot.h>
#include <OgreSceneNode.h>

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/Application.hpp>
#include <tempo/entity/Transform.hpp>

namespace tempo{
	/////////////////////////////////////////////////////////////////////
	/// \brief Component holding all data required to render an entity
	/////////////////////////////////////////////////////////////////////
	struct ComponentRender : anax::Component {
		Ogre::SceneNode* node;
		Ogre::SceneManager* scene;


		ComponentRender(Ogre::SceneManager* scene);
		void AddHealthBar(Ogre::SceneManager* scene);
		~ComponentRender();
	};

	/////////////////////////////////////////////////////////////////////
	/// \brief System that manages rendering entities, as well as managing
	/// the Ogre scene
	/////////////////////////////////////////////////////////////////////
	struct SystemRender : anax::System<anax::Requires<ComponentTransform, ComponentRender>> {
		Application&        app;
		Ogre::SceneManager* scene;

		SystemRender(Application& app);
		~SystemRender();

		void render(float dt);
	};
}

#endif
