#include <tempo/entity/Render.hpp>

namespace tempo{
	ComponentRender::ComponentRender(Ogre::SceneManager* scene, std::string path){
		this->path = path;
		this->scene = scene;
		this->node  = scene->getRootSceneNode()->createChildSceneNode();
	}


	ComponentRender::~ComponentRender(){
		this->node->getCreator()->destroySceneNode(this->node);
	}


	void ComponentRender::AddHealthBar(){

		// Create Billboard using Ogre Billboards
		Ogre::BillboardSet* Healthset = this->scene->createBillboardSet();
		Healthset->setMaterialName("rectangleSprite");
		Healthset->setDefaultDimensions(0.5, 0.2);
		Healthset->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
		Healthset->setCommonDirection(Ogre::Vector3(0, 1, 0));
		Ogre::Billboard* health = Healthset->createBillboard(0.5, 2, 0);

		this->healthBillboard = health;

		this->healthBillboard->setColour(Ogre::ColourValue(0,1,0));

		this->healthBarnode = this->node->createChildSceneNode();

		this->healthBarnode->attachObject(Healthset);
	}

	SystemRender::SystemRender(Application& new_app) : app(new_app) {
	  scene = app.ogre->createSceneManager(Ogre::ST_GENERIC);
	}

	SystemRender::~SystemRender(){
		app.ogre->destroySceneManager(scene);
	}

	void SystemRender::render(float dt){
		auto entities = getEntities();

		for(auto& entity : entities){
			auto& trans = entity.getComponent<ComponentTransform>();
			auto& rend  = entity.getComponent<ComponentRender  >();

			rend.node->setPosition   (trans.position);
			rend.node->setOrientation(trans.rotation);
		}

		app.ogre->renderOneFrame();
	}
}