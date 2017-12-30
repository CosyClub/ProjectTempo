#include <tempo/entity/Health.hpp>

namespace tempo {

	ComponentHealth::ComponentHealth(int entity_health) {
		this->max_health = entity_health;
		this->current_health = entity_health;
	}

	void ComponentHealth::HealthUpdate(int delta_health){

		if ((this->current_health) + delta_health > (this->max_health)){
			this->current_health = this->max_health;
		}

		else{
			this->current_health += delta_health;
		}

		printf("Entity has %d Health Points.\n", this->current_health);

		if((this->current_health <= 0)){
			printf("\nEntity has just been killed.\n");
		}

	}

	void SystemHealth::HealthUpdate(int delta_health) {

		auto entities = getEntities();

		for (auto& entity : entities) {
			auto& h = entity.getComponent<ComponentHealth>();
			int id = entity.getId();

			if ((h.current_health) + delta_health > (h.max_health)) {
				h.current_health = h.max_health;
			}

			else {
				h.current_health += delta_health;
			}

			printf("Entity ID: %d has %d Health Points.\n", id, h.current_health);

			if ((h.current_health <= 0)) {
				printf("\nEntity ID: %d has just been killed.\n", id);
				entity.kill();
			}
		}

	}

	void SystemHealth::CheckHealth() {
		auto entities = getEntities();

		for (auto& entity : entities) {
			auto& h = entity.getComponent<ComponentHealth>();
			int id = entity.getId();

			if ((h.current_health <= 0)){
				printf("\nEntity ID: %d has just been killed. \n", id);
				entity.kill();
			}
		}
	}

	ComponentRenderHealth::ComponentRenderHealth(Ogre::SceneManager* scene){
		this->node = scene->getRootSceneNode()->createChildSceneNode();

		Ogre::BillboardSet* Healthset = scene->createBillboardSet();
		Healthset->setMaterialName("rectangleSprite");
		Healthset->setDefaultDimensions(0.5, 0.5);
		Healthset->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
		Healthset->setCommonDirection(Ogre::Vector3(0, 1, 0));
		Ogre::Billboard* health = Healthset->createBillboard(0.5, 2, 0);
		health->setColour(Ogre::ColourValue::Green);

		this->node->attachObject(Healthset);
	}

	ComponentRenderHealth::~ComponentRenderHealth(){
		this->node->getCreator()->destroySceneNode(this->node);
	}


	void RenderHealth::HealthBarUpdate() {
		auto entities = getEntities();

		for(auto& entity : entities){
			auto& h = entity.getComponent<ComponentHealth>();
			auto& rend  = entity.getComponent<ComponentRenderHealth>();

			rend.node->setScale(h.current_health/ (double) h.max_health,1,1);

			// h.max_health
			// h.current_health;
			//
			//rend.Healthset->setDefaultDimensions(0.5 , 0.5);
		}

	}

}
