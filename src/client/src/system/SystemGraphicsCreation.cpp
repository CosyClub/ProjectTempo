#include <client/system/SystemGraphicsCreation.hpp>

#include <tempo/component/ComponentStage.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>
#include <client/component/ComponentHealthBar.hpp>
#include <iostream>

namespace client
{

SystemGraphicsCreation::SystemGraphicsCreation()
{
}

void SystemGraphicsCreation::addEntities(irr::video::IVideoDriver *driver,
                                         irr::scene::ISceneManager *manager)
{
	auto entities = getEntities();
	for (auto& entity : entities)
	{
		if (!entity.hasComponent<client::ComponentRenderSceneNode>())
		{
			entity.addComponent<ComponentRenderSceneNode>(nullptr);
      if(entity.hasComponent<tempo::ComponentHealth>()) {
        entity.addComponent<ComponentHealthBar>();
      }
			entity.activate();
		}
	}
}

} // namespace tempo
