#include <tempo/entity/EntityCreation.hpp>

anax::Entity newPlayer(anax::World& world, Ogre::SceneManager* scene, tempo::SystemLevelManager system_grid_motion){

  anax::Entity entity_player = world.createEntity();
  Ogre::BillboardSet* Pset = scene->createBillboardSet();
  Pset->setMaterialName("rectangleSprite");
  Pset->setDefaultDimensions(0.5, 1.5);
  Pset->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
  Pset->setCommonDirection(Ogre::Vector3(0, 1, 0));
  Ogre::Billboard* player = Pset->createBillboard(0, 0.75, 0);
  player->setColour(Ogre::ColourValue::Red);
  entity_player.addComponent<tempo::ComponentTransform>();
  entity_player.addComponent<tempo::ComponentRender>(scene).node->attachObject(Pset);
  entity_player.addComponent<tempo::ComponentGridPosition>(system_grid_motion.spawn());
  entity_player.addComponent<tempo::ComponentGridMotion>();
  entity_player.addComponent<tempo::ComponentPlayerInput>();
  entity_player.activate();

  return entity_player;
}

anax::Entity newAI(anax::World& world, Ogre::SceneManager* scene){

  anax::Entity entity_ai = world.createEntity();
	Ogre::BillboardSet* Aset = scene->createBillboardSet();
	Aset->setMaterialName("rectangleSprite");
	Aset->setDefaultDimensions(0.4, 1.3);
	Aset->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
	Aset->setCommonDirection(Ogre::Vector3(0, 1, 0));
	Ogre::Billboard* ai = Aset->createBillboard(0, 0.75, 0);
	ai->setColour(Ogre::ColourValue::Blue);
	entity_ai.addComponent<tempo::ComponentTransform>();
	entity_ai.addComponent<tempo::ComponentRender>(scene).node->attachObject(Aset);
	entity_ai.addComponent<tempo::ComponentRender>(scene).node->attachObject(Aset);
	entity_ai.addComponent<tempo::ComponentGridPosition>(3, 3);
	entity_ai.addComponent<tempo::ComponentGridMotion>();
	entity_ai.addComponent<tempo::ComponentGridAi>();
	entity_ai.activate();

  return entity_ai;
}
