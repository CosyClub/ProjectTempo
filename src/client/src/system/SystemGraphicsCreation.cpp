#include <client/system/SystemGraphicsCreation.hpp>

#include <client/component/ComponentHealthBar.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>
#include <iostream>
#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentStage.hpp>

namespace client
{
SystemGraphicsCreation::SystemGraphicsCreation() {}

void SystemGraphicsCreation::addEntities(irr::video::IVideoDriver * driver,
                                         irr::scene::ISceneManager *manager)
{
	auto entities = getEntities();
	for (auto &entity : entities) {
		if (!entity.hasComponent<client::ComponentRenderSceneNode>()) {
			entity.addComponent<ComponentRenderSceneNode>(nullptr);
			if (entity.hasComponent<tempo::ComponentHealth>()) {
				entity.addComponent<ComponentHealthBar>(nullptr);
			}
			entity.activate();
		}
	}
}

}  // namespace tempo
