#include <client/system/SystemButtonRenderer.hpp>

#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>
#include <irrlicht.h>  // :TODO: sort out includes

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <iostream>

namespace
{
}
namespace client
{
void SystemButtonRenderer::setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver)
{
	auto entities = getEntities();

	if (entities.size() == 0) {
		printf("\nThere does not seem to be any buttons\n");
	}

	for (auto &entity : entities) {
		auto &group = entity.getComponent<tempo::ComponentButtonGroup>();
		auto &rend  = entity.getComponent<client::ComponentRenderButtonGroup>();

		if (rend.setup) continue;

		// button data
		auto &buttons = group.buttons;

		for (int i = 0; i < buttons.size(); i++) {
			irr::scene::IMesh *mesh_button = smgr->getMesh("resources/meshes/button.obj");
			irr::scene::IMesh *mesh_button_housing =
			  smgr->getMesh("resources/meshes/button_housing.obj");

			// create a new struct to place meshes
			buttonRender buttonRend;
			buttonRend.button_housing = smgr->addMeshSceneNode(mesh_button_housing, 0);
			buttonRend.button         = smgr->addMeshSceneNode(mesh_button, 0);

			buttonRend.button_housing->setPosition(
			  irr::core::vector3df(buttons[i].pos.x, 0.0, buttons[i].pos.y));
			buttonRend.button->setPosition(
			  irr::core::vector3df(buttons[i].pos.x, 0.0, buttons[i].pos.y));

			irr::video::SMaterial &material_button_housing =
			  buttonRend.button_housing->getMaterial(0);
			material_button_housing.Shininess = 0.5f;
			material_button_housing.EmissiveColor.set(255, 100, 100, 100);

			irr::video::SMaterial &material_button = buttonRend.button->getMaterial(0);
			material_button.Shininess              = 0.0f;
			material_button.EmissiveColor.set(255, 255, 0, 0);

			rend.buttonsRender.push_front(buttonRend);
		}
		rend.setup = true;
	}
}

void SystemButtonRenderer::updateButtons(irr::video::IVideoDriver *driver)
{
	auto entities = getEntities();
	for (auto entity : entities) {
		auto &group = entity.getComponent<tempo::ComponentButtonGroup>();
		auto &rend  = entity.getComponent<client::ComponentRenderButtonGroup>();

		// button data
		auto &buttons = group.buttons;
		// button render data
		auto &buttonRend = rend.buttonsRender;

		if (buttons.size() != buttonRend.size()) {
			printf("\n\n\n\n This is a significant problem buttons=%d,buttonRend = %d\n\n\n\n",
			       buttons.size(), buttonRend.size());
		}

		if (!group.groupTriggered) {
			for (int i = 0; i < buttons.size(); i++) {
				if (buttons[i].triggered == true) {
					buttonRend[i].button->setPosition(
					  irr::core::vector3df(buttons[i].pos.x, -0.1, buttons[i].pos.y));
					irr::video::SMaterial &material_button = buttonRend[i].button->getMaterial(0);
					material_button.EmissiveColor.set(255, 0, 255, 0);
				} else {
					buttonRend[i].button->setPosition(
					  irr::core::vector3df(buttons[i].pos.x, 0, buttons[i].pos.y));
					irr::video::SMaterial &material_button = buttonRend[i].button->getMaterial(0);
					material_button.EmissiveColor.set(255, 255, 0, 0);
				}
			}
		} else {
			for (int i = 0; i < buttons.size(); i++) {
				buttons[i].triggered == true;
				buttonRend[i].button->setPosition(
				  irr::core::vector3df(buttons[i].pos.x, -0.1, buttons[i].pos.y));
				irr::video::SMaterial &material_button = buttonRend[i].button->getMaterial(0);
				material_button.EmissiveColor.set(255, 0, 255, 0);
			}
		}
	}
}
}  // namespace client
