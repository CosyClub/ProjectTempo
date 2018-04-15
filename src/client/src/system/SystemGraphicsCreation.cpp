#include <client/system/SystemGraphicsCreation.hpp>

#include <client/component/ComponentHealthBar.hpp>
#include <client/component/ComponentRenderAttack.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>
#include <client/component/ComponentRenderSpikes.hpp>
#include <client/component/ComponentRenderButtonGroup.hpp>

#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentButtonGroup.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentModel.hpp>
#include <tempo/component/ComponentSpikes.hpp>
#include <tempo/component/ComponentStage.hpp>

#include <iostream>

namespace client
{
SystemGraphicsCreation::SystemGraphicsCreation() {}

void SystemGraphicsCreation::addEntities(irr::video::IVideoDriver * driver,
                                         irr::scene::ISceneManager *manager,
                                         anax::World &              world)
{
	bool touched  = false;
	auto entities = getEntities();
	for (auto &entity : entities) {
		if (entity.hasComponent<tempo::ComponentModel>() &&
		   !entity.hasComponent<client::ComponentRenderSceneNode>()) {
			touched = true;
			entity.addComponent<ComponentRenderSceneNode>(nullptr);
			std::cout << "Added Null Scene Node\n";

			if (entity.hasComponent<tempo::ComponentHealth>()) {
				entity.addComponent<ComponentHealthBar>(nullptr);
			}

			if(entity.hasComponent<tempo::ComponentAttack>()){
				entity.addComponent<ComponentRenderAttack>(nullptr);
			}
		}

		if (entity.hasComponent<tempo::ComponentButtonGroup>() &&
		    !entity.hasComponent<ComponentRenderButtonGroup>()) {
			touched = true;
			std::cout << "Added Rendering to a Button" << std::endl;
			entity.addComponent<ComponentRenderButtonGroup>();
		}

		if (entity.hasComponent<tempo::ComponentSpikes>() &&
			!entity.hasComponent<ComponentRenderSpikes>()) {
			touched = true;
			std::cout << "Added Rendering to a Spike" << std::endl;
			entity.addComponent<ComponentRenderSpikes>();
		}

		if (touched) entity.activate();
	}

	if (touched)
		world.refresh();
}

}  // namespace tempo
