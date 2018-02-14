#ifndef TEMPO_COMPONENT_RENDER_HPP
#define TEMPO_COMPONENT_RENDER_HPP

#include <OgreRoot.h>
#include <OgreSceneNode.h>

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/Application.hpp>
#include <tempo/component/ComponentTransform.hpp>

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
}

#endif
