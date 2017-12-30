#ifndef TEMPO_ENTITY_HEALTH_HPP
#define TEMPO_ENTITY_HEALTH_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>
#include <tempo/entity/Render.hpp>
#include <Ogre.h>

namespace tempo {

	struct ComponentHealth : anax::Component {

		//Starting Health for entity
		int max_health;
		int current_health;

		ComponentHealth(int entity_health);

		void HealthUpdate(int delta_health);

	};

	struct SystemHealth : anax::System<anax::Requires<ComponentHealth>> {

		void HealthUpdate(int delta_health);
		void CheckHealth();

	};

	struct ComponentRenderHealth : anax::Component {
		Ogre::SceneNode* node;


		ComponentRenderHealth(Ogre::SceneManager* scene);
		~ComponentRenderHealth();
	};


	struct RenderHealth : anax::System<anax::Requires<ComponentHealth, ComponentRenderHealth>> {
		void HealthBarUpdate();
	};

}

#endif
