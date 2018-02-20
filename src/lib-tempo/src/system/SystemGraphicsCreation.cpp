#include <tempo/system/SystemGraphicsCreation.hpp>

#include <iostream>

namespace tempo {

SystemGraphicsCreation::SystemGraphicsCreation()
{
}

void SystemGraphicsCreation::addEntities(Ogre::SceneManager* scene)
{
	auto entities = getEntities();
	for (auto& entity : entities)
	{
		if (!entity.hasComponent<ComponentRender>())
		{
			std::cout << "Adding render component to entity with server ID " << entity.getId().index << std::endl;
			std::string path = entity.getComponent<ComponentModel>().path;
			entity.addComponent<ComponentRender>(scene, path);
			Ogre::SceneNode* node = entity.getComponent<ComponentRender>().node;

			Ogre::BillboardSet* Pset = scene->createBillboardSet();
			Pset->setMaterialName("rectangleSprite");
			Pset->setDefaultDimensions(0.5, 1.5);
			Pset->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
			Pset->setCommonDirection(Ogre::Vector3(0, 1, 0));
			Ogre::Billboard* player = Pset->createBillboard(0, 0.75, 0);
			player->setColour(Ogre::ColourValue::Red);

			node->attachObject(Pset);
		}

		Ogre::SceneNode* node = entity.getComponent<ComponentRender>().node;
		glm::ivec2 pos = entity.getComponent<ComponentStagePosition>().getOrigin();
		node->setPosition(Ogre::Vector3(pos.x, 0, pos.y));
	}
}

}
