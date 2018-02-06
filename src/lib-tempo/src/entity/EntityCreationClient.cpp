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
			int current_health = data.entity_type.player.current_health;
			int max_health = data.entity_type.player.max_health;
			return_entity = newPlayer(world, scene, instance_id, type_id,
			                          pos.x,
			                          pos.y,
									  					  current_health,
																max_health,
			                          system_gm
			                         );
			break;
			}
		case EID_AI:
			{
			AI_t a = data.entity_type.ai;
			int current_health = data.entity_type.ai.current_health;
			int max_health = data.entity_type.ai.max_health;
			return_entity = newAI(world, scene, instance_id, type_id,
			                      pos.x,
			                      pos.y,
														current_health,
														max_health
			                     );
			break;
			}
		case EID_DES:
			{
			Destroyable_t d = data.entity_type.destroyable;
			// int health = data.entity_type.player.health;
			return_entity = newDestroyable(world, scene, instance_id, type_id,
			                               pos.x,
										   					 		 pos.y,
										   							 // health,
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

anax::Entity newPlayer(anax::World& world, Ogre::SceneManager* scene, int iid, EID tid, int x, int y, int current_health, int max_health, tempo::SystemLevelManager system_grid_motion) {

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
	entity_player.addComponent<tempo::ComponentHealth>(current_health,max_health);
	printf("\n\n ClientPlayer currently has health = %d\n\n", entity_player.getComponent<tempo::ComponentHealth>().current_health);
	entity_player.addComponent<tempo::ComponentRender>(scene, "N/A").node->attachObject(Pset);
	entity_player.getComponent<tempo::ComponentRender>().AddHealthBar();
	entity_player.addComponent<tempo::ComponentGridPosition>(x, y, tempo::tileMask1by1, false);
	entity_player.addComponent<tempo::ComponentGridMotion>();
	entity_player.addComponent<tempo::ComponentPlayerRemote>();
	entity_player.activate();

	return entity_player;
}

anax::Entity newAI(anax::World& world, Ogre::SceneManager* scene, int iid, EID tid, int x, int y, int current_health, int max_health) {

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
	entity_ai.addComponent<tempo::ComponentHealth>(current_health,max_health);
	entity_ai.addComponent<tempo::ComponentRender>(scene, "N/A").node->attachObject(Aset);
	entity_ai.getComponent<tempo::ComponentRender>().AddHealthBar();
	entity_ai.addComponent<tempo::ComponentGridPosition>(x, y, tempo::tileMask1by1, false);
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
	//entity_object.addComponent<tempo::ComponentHealth>(health);
	//printf("\n\n ClientDest currently has health = %d\n\n", entity_object.getComponent<tempo::ComponentHealth>().current_health);
	entity_object.addComponent<tempo::ComponentRender>(scene, mesh_name).node->attachObject(entity_mesh);
	//entity_object.addComponent<tempo::ComponentRender>().AddHealthBar();
	entity_object.addComponent<tempo::ComponentGridPosition>(x, y, tempo::tileMask1by1, false);
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
	entity_object.addComponent<tempo::ComponentGridPosition>(x, y, tempo::tileMask1by1, false);
	entity_object.addComponent<tempo::ComponentGridMotion>();

	//TEMP:: Mesh used was too big
	Ogre::SceneNode* node_object = entity_object.getComponent<tempo::ComponentRender>().node;
	node_object->setScale(.5, .5, .5);

	entity_object.activate();

	return entity_object;

}

}
