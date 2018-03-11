#include <client/system/SystemGraphicsCreation.hpp>

#include <client/component/ComponentHealthBar.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>
#include <iostream>
#include <tempo/component/ComponentGridAi.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/component/ComponentStage.hpp>

namespace client
{
SystemGraphicsCreation::SystemGraphicsCreation() {}

void SystemGraphicsCreation::addEntities(irr::video::IVideoDriver * driver,
                                         irr::scene::ISceneManager *manager,
                                         anax::World &world)
{
	bool touched = false;
	auto entities = getEntities();
	for (auto &entity : entities) {
		if (!entity.hasComponent<client::ComponentRenderSceneNode>()) {
			touched = true;
			if (entity.hasComponent<tempo::ComponentPlayerLocal>() || entity.hasComponent<tempo::ComponentPlayerRemote>()) {
				entity.addComponent<ComponentRenderSceneNode>(nullptr, "rogue.png", 4, 4);
			}
			else if (entity.hasComponent<tempo::ComponentGridAi>()) {
				entity.addComponent<ComponentRenderSceneNode>(nullptr, "rogue.png", 4, 4);
			}
			std::cout << "Added Null Scene Node\n";
			if (entity.hasComponent<tempo::ComponentHealth>()) {
				entity.addComponent<ComponentHealthBar>(nullptr);
			}
			entity.activate();
		}
	}

	if (touched) world.refresh();
}

}  // namespace tempo
