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
void SystemButtonRenderer::setup(irr::scene::ISceneManager* smgr, irr::video::IVideoDriver* driver)
{
	this->buttonBlocked = driver->getTexture("resources/materials/buttonBlocked.png");
	this->buttonArrow = driver->getTexture("resources/materials/buttonArrow.png");

	auto& entities = getEntities();

	if (entities.size() == 0) {
		printf("\nThere does not seem to be any buttons\n");
	}

	int j = 0; //used in setRotation
	for (auto &entity : entities) {
		auto &group = entity.getComponent<tempo::ComponentButtonGroup>();
		auto &rend  = entity.getComponent<client::ComponentRenderButtonGroup>();

		if (rend.setup)
			continue;

		if (group.rhythmID == 0) { // Rhythm-less buttons
			// button data
			auto &buttons = group.buttons;

			for (uint32_t i = 0; i < buttons.size(); i++) {
				irr::scene::IMesh *mesh_button = smgr->getMesh("resources/meshes/button.obj");
				irr::scene::IMesh *mesh_button_housing =
					smgr->getMesh("resources/meshes/button_housing.obj");

				// create a new struct to place meshes
				buttonRender buttonRend;
				buttonRend.button_housing = smgr->addMeshSceneNode(mesh_button_housing, 0);
				buttonRend.button = smgr->addMeshSceneNode(mesh_button, 0);

				buttonRend.button_housing->setPosition(
					irr::core::vector3df(buttons[i].pos.x, 0.0, buttons[i].pos.y));
				buttonRend.button->setPosition(
					irr::core::vector3df(buttons[i].pos.x, 0.0, buttons[i].pos.y));

				irr::video::SMaterial &material_button_housing =
					buttonRend.button_housing->getMaterial(0);
				material_button_housing.Shininess = 0.5f;
				material_button_housing.EmissiveColor.set(255, 100, 100, 100);

				irr::video::SMaterial &material_button = buttonRend.button->getMaterial(0);
				material_button.Shininess = 0.0f;
				material_button.EmissiveColor.set(255, 255, 0, 0);

				rend.buttonsRender.push_front(buttonRend);
			}

		}

		else { // Rhythm Based buttons

			// button data
			auto &buttons = group.buttons;

			for (uint32_t i = 0; i < buttons.size(); i++) {

				irr::scene::IMesh *mesh_button = smgr->getMesh("resources/meshes/button.obj");
				irr::scene::IMesh *mesh_button_housing =
					smgr->getMesh("resources/meshes/button_housing.obj");

				// create a new struct to place meshes
				buttonRender buttonRend;
				buttonRend.button_housing = smgr->addMeshSceneNode(mesh_button_housing, 0);
				buttonRend.button = smgr->addMeshSceneNode(mesh_button, 0);

				buttonRend.button_housing->setPosition(
					irr::core::vector3df(buttons[i].pos.x, 0.0, buttons[i].pos.y));
				buttonRend.button->setPosition(
					irr::core::vector3df(buttons[i].pos.x, 0.0, buttons[i].pos.y));


				irr::video::SMaterial &material_button_housing =
						buttonRend.button_housing->getMaterial(0);
				material_button_housing.Shininess = 0.5f;
				material_button_housing.EmissiveColor.set(255, 100, 100, 100);

				//Rotate each button so that the arows are facing the correct direction
				setRotation(entities, j, i, buttonRend.button, buttons[i].pos);

				if (!group.groupTriggerable) {
					irr::video::SMaterial &material_button = buttonRend.button->getMaterial(0);
					material_button.Shininess = 0.0f;
					material_button.EmissiveColor.set(255, 255, 0, 0);
					material_button.setTexture(0, this->buttonArrow);
				}

				else {
					irr::video::SMaterial &material_button = buttonRend.button->getMaterial(0);
					material_button.Shininess = 0.0f;
					material_button.EmissiveColor.set(255, 255, 255, 255);
					material_button.setTexture(0, this->buttonArrow);
				}

				rend.buttonsRender.push_front(buttonRend);
			}
		}

		rend.setup = true;
		j++;
	}
}

void SystemButtonRenderer::setRotation(std::vector<anax::Entity> entities, uint32_t j, uint32_t i, irr::scene::IMeshSceneNode *button, glm::ivec2 pos) {
	//Last button group in level
	if (j == entities.size() - 1) {
		return;
	}

	//if last button in sequence
	if (entities[j].getComponent<tempo::ComponentButtonGroup>().next.x == -1 && entities[j].getComponent<tempo::ComponentButtonGroup>().next.y == -1) {
		return;
	}

	glm::ivec2 newPos = entities[j + 1].getComponent<tempo::ComponentButtonGroup>().buttons[i].pos;

	int dx, dy;

	dx = newPos.x - pos.x;

	dy = newPos.y - pos.y;

	double n = NAN; // for error checking

	if (dx == 1) {
		n = 180;
	}

	else if (dx == -1) {
		n = 0;
	}

	else if (dy == 1) {
		n = 90;
	}

	else if (dy == -1) {
		n = 270;
	}

	button->setRotation(irr::core::vector3df(0, n, 0));
}

void SystemButtonRenderer::updateButtons(const glm::ivec2 playerpos)
{
	auto& entities = getEntities();
	for (int i = entities.size() - 1; i >= 0; i--) {
		auto &entity = entities[i];
		auto &group = entity.getComponent<tempo::ComponentButtonGroup>();
		auto &rend = entity.getComponent<client::ComponentRenderButtonGroup>();

		// button data
		auto& buttons = group.buttons;
		// button render data
		auto& buttonRend = rend.buttonsRender;

		if (group.rhythmID == 0) { //Rhythm-less buttons

			if (buttons.size() != buttonRend.size()) {
				printf("\n\n\n\n This is a significant problem buttons=%ld,buttonRend = %ld\n\n\n\n",
					buttons.size(), buttonRend.size());
			}

			if (!group.groupTriggered) {
				for (uint32_t j = 0; j < buttons.size(); j++) {
					glm::ivec2 pos = buttons[j].pos;
					if (pos.x < playerpos.x - 24 || pos.x > playerpos.x + 7 ||
						pos.y < playerpos.y - 33 || pos.y > playerpos.y + 33) {

						buttonRend[j].button->setVisible(false);
						buttonRend[j].button_housing->setVisible(false);
						continue;
					}
					buttonRend[j].button->setVisible(true);
					buttonRend[j].button_housing->setVisible(true);

					if (buttons[j].triggered == true) {
						buttonRend[j].button->setPosition(
							irr::core::vector3df(buttons[j].pos.x, -0.1, buttons[j].pos.y));
						irr::video::SMaterial &material_button = buttonRend[j].button->getMaterial(0);
						material_button.EmissiveColor.set(255, 0, 255, 0);
					}
					else {
						buttonRend[j].button->setPosition(
							irr::core::vector3df(buttons[j].pos.x, 0, buttons[j].pos.y));
						irr::video::SMaterial &material_button = buttonRend[j].button->getMaterial(0);
						material_button.EmissiveColor.set(255, 255, 0, 0);
					}
				}
			}
			else {
				for (uint32_t j = 0; j < buttons.size(); j++) {
					glm::ivec2 pos = buttons[j].pos;
					if (pos.x < playerpos.x - 24 || pos.x > playerpos.x + 7 ||
						pos.y < playerpos.y - 33 || pos.y > playerpos.y + 33) {

						buttonRend[j].button->setVisible(false);
						buttonRend[j].button_housing->setVisible(false);
						continue;
					}
					buttonRend[j].button->setVisible(true);
					buttonRend[j].button_housing->setVisible(true);

					buttonRend[j].button->setPosition(
						irr::core::vector3df(buttons[j].pos.x, -0.1, buttons[j].pos.y));
					irr::video::SMaterial &material_button = buttonRend[j].button->getMaterial(0);
					material_button.EmissiveColor.set(255, 0, 255, 0);
				}
			}
		}

		else { // Rhythm-based buttons

			if (buttons.size() != buttonRend.size()) {
				printf("\n\n\n\n This is a significant problem Rhythmbuttons=%ld,buttonRend = %ld\n\n\n\n",
					buttons.size(), buttonRend.size());
			}

			if (group.groupTriggerable) {
				for (uint32_t j = 0; j < buttons.size(); j++) {
					glm::ivec2 pos = buttons[j].pos;
					if (pos.x < playerpos.x - 24 || pos.x > playerpos.x + 7 ||
						pos.y < playerpos.y - 33 || pos.y > playerpos.y + 33) {

						buttonRend[j].button->setVisible(false);
						buttonRend[j].button_housing->setVisible(false);
						continue;
					}
					buttonRend[j].button->setVisible(true);
					buttonRend[j].button_housing->setVisible(true);

					buttonRend[j].button->setPosition(
						irr::core::vector3df(buttons[j].pos.x, 0, buttons[j].pos.y));
					irr::video::SMaterial &material_button = buttonRend[j].button->getMaterial(0);
					material_button.setTexture(0, this->buttonArrow);
					material_button.EmissiveColor.set(255, 255, 255, 255);

					//Dont display arrow on last button
					if (group.next.x == -1 && group.next.y == -1) {
						irr::video::SMaterial &material_button = buttonRend[j].button->getMaterial(0);
						material_button.setTexture(0, nullptr);
					}
				}
			}

			else {
				for (uint32_t j = 0; j < buttons.size(); j++) {
					glm::ivec2 pos = buttons[j].pos;
					if (pos.x < playerpos.x - 24 || pos.x > playerpos.x + 7 ||
						pos.y < playerpos.y - 33 || pos.y > playerpos.y + 33) {

						buttonRend[j].button->setVisible(false);
						buttonRend[j].button_housing->setVisible(false);
						continue;
					}
					buttonRend[j].button->setVisible(true);
					buttonRend[j].button_housing->setVisible(true);

					if (group.groupTriggered) {
						buttonRend[j].button->setPosition(
							irr::core::vector3df(buttons[j].pos.x, -0.1, buttons[j].pos.y));
						irr::video::SMaterial &material_button = buttonRend[j].button->getMaterial(0);
						material_button.setTexture(0, this->buttonArrow);
						material_button.EmissiveColor.set(255, 0, 255, 0);
					}

					else if (group.blocked) {
						buttonRend[j].button->setPosition(
							irr::core::vector3df(buttons[j].pos.x, 0, buttons[j].pos.y));
						irr::video::SMaterial &material_button = buttonRend[j].button->getMaterial(0);
						material_button.setTexture(0, this->buttonBlocked);
						material_button.EmissiveColor.set(255, 200, 0, 0);
					}

					else {
						buttonRend[j].button->setPosition(
							irr::core::vector3df(buttons[j].pos.x, 0, buttons[j].pos.y));
						irr::video::SMaterial &material_button = buttonRend[j].button->getMaterial(0);
						material_button.setTexture(0, this->buttonArrow);
						material_button.EmissiveColor.set(255, 255, 0, 0);
					}

					//Dont display arrow on last button
					if (group.next.x == -1 && group.next.y == -1 && !group.blocked) {
						irr::video::SMaterial &material_button = buttonRend[j].button->getMaterial(0);
						material_button.setTexture(0, nullptr);
					}

				}
			}

		}
	}
}
}  // namespace client
