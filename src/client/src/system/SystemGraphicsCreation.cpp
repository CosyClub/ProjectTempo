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
		// tempo::ComponentModel model = entity.getComponent<tempo::ComponentModel>();	
		// tempo::ComponentStagePosition pos = entity.getComponent<tempo::ComponentStagePosition>();	
		// glm::ivec2 origin = pos.getOrigin();

		if (!entity.hasComponent<client::ComponentRenderSceneNode>())
		{
			// irr::io::path path(model.path.c_str());

			// glm::vec3 c = model.color;
			// irr::video::SColor color(255, c.x, c.y, c.z);

			// irr::scene::ISceneNode *node;

			// if (model.isMesh)
			// {
			// 	irr::scene::IAnimatedMesh *mesh = manager->getMesh(path);
			// 	node = manager->addAnimatedMeshSceneNode(mesh);
			// }
			// else
			// {
			// 	irr::core::dimension2d<irr::f32> size(1, 2);
			// 	node = manager->addBillboardSceneNode(NULL,
			// 	                                       size);
			// 	// node->setMaterialTexture(0, driver->getTexture(path));
			// 	node->getMaterial(0).DiffuseColor = color;
			// }

			entity.addComponent<ComponentRenderSceneNode>(nullptr);
			entity.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
			entity.activate();
		}
	}
}

} // namespace tempo
