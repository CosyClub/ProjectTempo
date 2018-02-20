#include <tempo/system/SystemGraphicsCreation.hpp>

#include <iostream>

namespace tempo {

SystemGraphicsCreation::SystemGraphicsCreation()
{
}

void SystemGraphicsCreation::addEntities(Ogre::SceneManager* scene)
{
	auto entities = getEntities();
	std::cout << "Adding graphics for " << entities.size() << " entities" << std::endl;
	for (auto& entity : entities)
	{
		if (!entity.hasComponent<ComponentRender>())
		{
			std::cout << "Adding render component to entity with server ID " << entity.getId().index << std::endl;
			std::string path = entity.getComponent<ComponentModel>().path;
			entity.addComponent<ComponentRender>(scene, path);
		}
	}
}

}
