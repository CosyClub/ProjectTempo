#include <client/system/SystemButtonRenderer.hpp>

#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>
#include <irrlicht.h> // :TODO: sort out includes

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <iostream>

namespace {

}
namespace client {
	void SystemButtonRenderer::setup(irr::scene::ISceneManager* smgr,
	                                irr::video::IVideoDriver*  driver) {

		printf("\n\n\nSYSTEM BUTTON RENDERER IS BEING SET UP\n\n\n");

		auto entities = getEntities();

		if(entities.size() == 0){
			printf("\nThats a problem\n");
		}

		for (auto& entity : entities) {

			auto& group = entity.getComponent<tempo::ComponentButtonGroup>();
			auto& rend = entity.getComponent<client::ComponentRenderButtonGroup>();
			printf("\n\n\nButtonRend Line 25\n\n\n");

			//button data
			auto& buttons = group.buttons;
			printf("\n\n\nButtonRend Line 29\n\n\n");
			//button render data
			auto& buttonRend = rend.buttonsRender;
			printf("\n\n\nButtonRend Line 32\n\n\n");

			for (int i=0; i < buttons.size(); i++){
				irr::scene::IMesh* mesh_button = smgr->getMesh("resources/meshes/button.obj");
				irr::scene::IMesh* mesh_button_housing= smgr->getMesh("resources/meshes/button_housing.obj");

				//create a new struct to place meshes
				buttonRender buttonRend;
				buttonRend.button_housing = smgr->addMeshSceneNode(mesh_button_housing, 0);
				buttonRend.button         = smgr->addMeshSceneNode(mesh_button, 0);

				buttonRend.button_housing->setPosition(irr::core::vector3df(buttons[i].pos.x, 0.0, buttons[i].pos.y));
				buttonRend.button->setPosition(irr::core::vector3df(buttons[i].pos.x, 0.0, buttons[i].pos.y));

				irr::video::SMaterial& material_button_housing = buttonRend.button_housing->getMaterial(0);
				material_button_housing.Shininess = 0.5f;
				material_button_housing.EmissiveColor.set(255, 100, 100, 100);

				irr::video::SMaterial& material_button  = buttonRend.button->getMaterial(0);
				material_button.Shininess = 0.0f;
				material_button.EmissiveColor.set(255, 255, 0, 0);

				rend.buttonsRender.push_front(buttonRend);
			}
			printf("\n\n\nButtonRend Line 56\n\n\n");
		}
	}

	void SystemButtonRenderer::updateButtons(irr::video::IVideoDriver*  driver) {

		printf("\n\n\nButtonRend Line 62\n\n\n");
		auto entities = getEntities();
		printf("\n\n\nButtonRend Line 64\n\n\n");
		for (auto entity : entities) {

			auto& group = entity.getComponent<tempo::ComponentButtonGroup>();
			printf("\n\n\nButtonRend Line 68\n\n\n");
			auto& rend = entity.getComponent<client::ComponentRenderButtonGroup>();
			printf("\n\n\nButtonRend Line 70\n\n\n");

			//button data
			auto& buttons = group.buttons;
			//button render data
			auto& buttonRend = rend.buttonsRender;

			if(buttons.size() != buttonRend.size()){
				printf("\n\n\n\n FUCKFUCKFUCK buttons=%d,buttonRend = %d\n\n\n\n",buttons.size(),buttonRend.size());
			}
			else{
				printf("\n\n\n\n Sizes are fine \n\n\n\n");
			}

			if(!group.groupTriggered){

			bool tempTriggered = true;

			printf("\n\n\nButtonRend Line 81\n\n\n");

			for (int i=0; i < buttons.size(); i++){

				printf("\n\n\nButtonRend Line 85\n\n\n");

				if(buttons[i].triggered == true){
					printf("\n\n\nButtonRend Line 88\n\n\n");
					irr::scene::IMeshSceneNode* button = buttonRend[i].button;
					printf("\n\n\nWe have the button\n\n\n");
					int x = buttons[i].pos.x;
					int y = buttons[i].pos.y;
					printf("\n\n\nWe have the positions: {%d,%d}\n\n\n",x,y);
					button->setPosition(irr::core::vector3df(x, -0.1, y));
					printf("\n\n\nButtonRend Line 90\n\n\n");
					irr::video::SMaterial& material_button  = buttonRend[i].button->getMaterial(0);
					printf("\n\n\nButtonRend Line 92\n\n\n");
					material_button.Shininess = 0.0f;
					printf("\n\n\nButtonRend Line 94\n\n\n");
					material_button.EmissiveColor.set(255, 0, 255, 0);
					printf("\n\n\nButtonRend Line 96\n\n\n");
				}
				else{
					printf("\n\n\nButtonRend Line 99\n\n\n");
					tempTriggered = false;
					buttonRend[i].button->setPosition(irr::core::vector3df(buttons[i].pos.x, 0, buttons[i].pos.y));
					irr::video::SMaterial& material_button  = buttonRend[i].button->getMaterial(0);
					material_button.Shininess = 0.0f;
					material_button.EmissiveColor.set(255, 255, 0, 0);
				}
			}
			printf("\n\n\nButtonRend Line 100\n\n\n");
			if(tempTriggered){
				group.groupTriggered = true;
			}
		}

		else{
			for (int i=0; i < buttons.size(); i++){
				buttons[i].triggered == true;
				buttonRend[i].button->setPosition(irr::core::vector3df(buttons[i].pos.x, -0.1, buttons[i].pos.y));
				irr::video::SMaterial& material_button  = buttonRend[i].button->getMaterial(0);
				material_button.Shininess = 0.0f;
				material_button.EmissiveColor.set(255, 0, 255, 0);
			}
		}

		}

	}
} // namespace client
