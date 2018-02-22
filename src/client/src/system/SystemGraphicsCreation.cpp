#include <client/system/SystemGraphicsCreation.hpp>

#include <tempo/component/ComponentStage.hpp>

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
			entity.activate();
		}
	}
}

} // namespace tempo
