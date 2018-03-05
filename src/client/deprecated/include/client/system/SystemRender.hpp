#ifndef TEMPO_SYSTEM_RENDER_HPP
#define TEMPO_SYSTEM_RENDER_HPP

#include <OgreRoot.h>
#include <OgreSceneNode.h>

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <client/Application.hpp>
#include <client/component/ComponentRender.hpp>

#include <tempo/component/ComponentTransform.hpp>


namespace tempo
{
/////////////////////////////////////////////////////////////////////
/// \brief System that manages rendering entities, as well as managing
/// the Ogre scene
/////////////////////////////////////////////////////////////////////
struct SystemRender : anax::System<anax::Requires<ComponentTransform, ComponentRender>> {
	Application &       app;
	Ogre::SceneManager *scene;

	SystemRender(Application &app);
	~SystemRender();

	void render(float dt);
};
}

#endif
