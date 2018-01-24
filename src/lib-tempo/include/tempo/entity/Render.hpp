////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// Render.hpp
///
/// Declares the the Render component and system
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
		std::string path;
		Ogre::SceneManager* scene;

		//Required for Entity health bars
		Ogre::SceneNode* healthBarnode;
		Ogre::Billboard* healthBillboard;

		ComponentRender(Ogre::SceneManager* scene, std::string path);
		~ComponentRender();

		// AddHealthBar
		// Add Health bar to entity to show current health
		//
		// Arguments:
		//          none
		// Returns:
		//          void
		void AddHealthBar();
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
