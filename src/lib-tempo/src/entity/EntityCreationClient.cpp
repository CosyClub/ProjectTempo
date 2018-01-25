#include <tempo/entity/EntityCreationClient.hpp>

namespace tempo {

anax::Entity newEntity(EntityCreationData data, 
                       anax::World& world,
                       Ogre::SceneManager* scene,
                       tempo::SystemLevelManager system_gm)
{
	anax::Entity return_entity;

	EID type_id = data.type_id;
	Vec2s pos = data.position;
	int instance_id = data.instance_id;

	switch (type_id){
		case EID_PLAYER:
			{
			Player_t p = data.entity_type.player;
			return_entity = newPlayer(world, scene, instance_id, type_id,
			                          system_gm
			                         );
			break;
			}
		case EID_AI:
			{
			AI_t a = data.entity_type.ai;
			return_entity = newAI(world, scene, instance_id, type_id,
			                      pos.x,
			                      pos.y
			                     );
			break;
			}
		case EID_DES:
			{
			Destroyable_t d = data.entity_type.destroyable;
			return_entity = newDestroyable(world, scene, instance_id, type_id,
			                               pos.x,
					               pos.y,
					               /* std::string(d.mesh_name) */ // TODO FIX
					               "Cube"
			                              );
			break;
			}
		case EID_NONDES:
			{
			NonDestroyable_t n = data.entity_type.nondestroyable;
			return_entity = newNonDestroyable(world, scene, instance_id, type_id,
			                                  pos.x,
			                                  pos.y,
			                                  /* std::string(n.mesh_name) */ // TODO FIX
			                                  "Cube"
			                                 );
			break;
			}
	}

	return return_entity;
}

anax::Entity newPlayer(anax::World& world, Ogre::SceneManager* scene, int iid, EID tid, tempo::SystemLevelManager system_grid_motion) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_player = world.createEntity();
	Ogre::BillboardSet* Pset = scene->createBillboardSet();
	Pset->setMaterialName("rectangleSprite");
	Pset->setDefaultDimensions(0.5, 1.5);
	Pset->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
	Pset->setCommonDirection(Ogre::Vector3(0, 1, 0));
	Ogre::Billboard* player = Pset->createBillboard(0, 0.75, 0);
	player->setColour(Ogre::ColourValue::Red);
	
		entity_player.addComponent<tempo::ComponentID>(iid, (int)tid);
	entity_player.addComponent<tempo::ComponentTransform>();
	entity_player.addComponent<tempo::ComponentRender>(scene, "N/A").node->attachObject(Pset);
	entity_player.addComponent<tempo::ComponentGridPosition>(system_grid_motion.spawn());
	entity_player.addComponent<tempo::ComponentGridMotion>();
	entity_player.addComponent<tempo::ComponentPlayerLocal>();
	entity_player.activate();

	return entity_player;
}

anax::Entity newAI(anax::World& world, Ogre::SceneManager* scene, int iid, EID tid, int x, int y) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_ai = world.createEntity();
	Ogre::BillboardSet* Aset = scene->createBillboardSet();
	Aset->setMaterialName("rectangleSprite");
	Aset->setDefaultDimensions(0.4, 1.3);
	Aset->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
	Aset->setCommonDirection(Ogre::Vector3(0, 1, 0));
	Ogre::Billboard* ai = Aset->createBillboard(0, 0.75, 0);
	ai->setColour(Ogre::ColourValue::Blue);

	entity_ai.addComponent<tempo::ComponentID>(iid, (int)tid);
	entity_ai.addComponent<tempo::ComponentTransform>();
	entity_ai.addComponent<tempo::ComponentRender>(scene, "N/A").node->attachObject(Aset);
	entity_ai.addComponent<tempo::ComponentGridPosition>(x, y);
	entity_ai.addComponent<tempo::ComponentGridMotion>();
	entity_ai.addComponent<tempo::ComponentGridAi>();
	entity_ai.activate();

	return entity_ai;
}

anax::Entity newDestroyable(anax::World& world, Ogre::SceneManager* scene, int iid, EID tid, int x, int y, std::string mesh_name) {

	//TODO:: Add HealthComponent
	//TODO:: Add Entity to Specific Tile
	
	anax::Entity entity_object = world.createEntity();
	std::string filename = "meshes/" + mesh_name + ".mesh";
	Ogre::Entity* entity_mesh = scene->createEntity(filename);

	entity_object.addComponent<tempo::ComponentID>(iid, (int)tid);
	entity_object.addComponent<tempo::ComponentTransform>();
	entity_object.addComponent<tempo::ComponentRender>(scene, mesh_name).node->attachObject(entity_mesh);
	entity_object.addComponent<tempo::ComponentGridPosition>(x, y);
	entity_object.addComponent<tempo::ComponentGridMotion>();

	//TEMP:: Mesh used was too big
	Ogre::SceneNode* node_object = entity_object.getComponent<tempo::ComponentRender>().node;
	node_object->setScale(.5, .5, .5);

	entity_object.activate();

	return entity_object;

}

anax::Entity newNonDestroyable(anax::World& world, Ogre::SceneManager* scene, int iid, EID tid, int x, int y, std::string mesh_name) {

	//TODO:: Add Entity to Specific Tile

	anax::Entity entity_object = world.createEntity();
	std::string filename = "meshes/" + mesh_name + ".mesh";
	Ogre::Entity* entity_mesh = scene->createEntity(filename);

	entity_object.addComponent<tempo::ComponentID>(iid, (int)tid);
	entity_object.addComponent<tempo::ComponentTransform>();
	entity_object.addComponent<tempo::ComponentRender>(scene, mesh_name).node->attachObject(entity_mesh);
	entity_object.addComponent<tempo::ComponentGridPosition>(x, y);
	entity_object.addComponent<tempo::ComponentGridMotion>();

	//TEMP:: Mesh used was too big
	Ogre::SceneNode* node_object = entity_object.getComponent<tempo::ComponentRender>().node;
	node_object->setScale(.5, .5, .5);

	entity_object.activate();

	return entity_object;

}

}