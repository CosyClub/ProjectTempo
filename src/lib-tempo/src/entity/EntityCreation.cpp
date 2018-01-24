#include <tempo/entity/EntityCreation.hpp>

namespace tempo
{

/* EntityCreationData* newEntity(EID type_id, Vec2s pos){ */

/* 	EntityCreationData* entity = new EntityCreationData; */
/* 	entity->type_id = type_id; */
/* 	entity->position = pos; */

/* 	switch (entity->type_id){ */
/* 		case EID_PLAYER: */
/* 			entity->entity_type.player.some_data_for_player = EID_PLAYER; */
/* 			printf("\n\n\n\n%d\n\n\n\n\n", entity->entity_type.player.some_data_for_player); */
/* 			break; */
/* 		case EID_AI: */
/* 			entity->entity_type.ai.some_data_for_ai = EID_AI; */
/* 			printf("\n\n\n\n%d\n\n\n\n\n", entity->entity_type.ai.some_data_for_ai); */
/* 			break; */
/* 		case EID_DES: */
/* 			entity->entity_type.destroyable.some_data_for_destroyable = EID_DES; */
/* 			printf("\n\n\n\n%d\n\n\n\n\n", entity->entity_type.destroyable.some_data_for_destroyable); */
/* 			break; */
/* 		case EID_NONDES: */
/* 			entity->entity_type.nondestroyable.some_data_for_nondestroyable = EID_NONDES; */
/* 			printf("\n\n\n\n%d\n\n\n\n\n", entity->entity_type.nondestroyable.some_data_for_nondestroyable); */
/* 			break; */
/* 		default: printf("Missed all\n"); */
/* 	} */

/* 	return entity; */
/* } */

sf::Packet& operator <<(sf::Packet& packet, const EntityCreationData& data)
{
	packet << data.type_id;
	packet << data.position;
	packet << data.entity_type;
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const Entity_Type& type)
{
	uint8_t *data = (uint8_t*)(&type);

	for (int I = 0; I < sizeof(Entity_Type); I++)
	{
		packet << data[I];
	}

	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const Vec2s& vec)
{
	packet << vec.elements[0];
	packet << vec.elements[1];

	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, EntityCreationData& data)
{
	int tmp;

	packet >> tmp;
	data.type_id = (EID)tmp;
	packet >> data.position;
	packet >> data.entity_type;
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Entity_Type& type)
{
	uint8_t *data = (uint8_t*)(&type);

	for (int I = 0; I < sizeof(Entity_Type); I++)
	{
		packet >> data[I];
	}

	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Vec2s& vec)
{
	packet >> vec.elements[0];
	packet >> vec.elements[1];

	return packet;
}



EntityCreationData dumpEntty(anax::Entity e)
{
	EID type_id = (EID) e.getComponent<ComponentID>().type_id;
	Vec2s position;
	int instance_id   = e.getComponent<ComponentID>().instance_id;

	/* data.type_id = type_id; */
	/* data.instance_id = instance_id; */

	switch (type_id)
	{
		case EID_PLAYER:
			{
			position = e.getComponent<ComponentGridPosition>().getPosition();
			Player_t p;
			p.foo = 0;
			Entity_Type t = (Entity_Type) {.player = p};
			EntityCreationData data = {type_id, position, instance_id, t};
			return data;
			break;
			}
		case EID_AI:
			{
			position = e.getComponent<ComponentGridPosition>().getPosition();
			AI_t a;
			a.foo = 0;
			Entity_Type t = (Entity_Type) {.ai = a};
			EntityCreationData data = {type_id, position, instance_id, t};
			return data;
			break;
			}
		case EID_DES:
			{
			position = e.getComponent<ComponentGridPosition>().getPosition();
			Destroyable_t d;
			memset(&(d.mesh_name), 0, 100);
			memcpy((void*)e.getComponent<ComponentRender>().path.c_str(), &(d.mesh_name), 100);
			Entity_Type t = (Entity_Type) {.destroyable = d};
			EntityCreationData data = {type_id, position, instance_id, t};
			return data;
			break;
			}
		case EID_NONDES:
			{
			position = e.getComponent<ComponentGridPosition>().getPosition();
			NonDestroyable_t n;
			memset(&(n.mesh_name), 0, 100);
			memcpy((void*)e.getComponent<ComponentRender>().path.c_str(), &(n.mesh_name), 100);
			Entity_Type t = (Entity_Type) {.nondestroyable = n};
			EntityCreationData data = {type_id, position, instance_id, t};
			return data;
			break;
			}
	}

}

anax::Entity newEntity(EntityCreationData data, anax::World& world,
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
			// int h = p.health;
			return_entity = newPlayer(world, scene, instance_id, type_id,
			                          system_gm
			                         );
			break;
			}
		case EID_AI:
			{
			AI_t a = data.entity_type.ai;
			// int h = a.health;
			return_entity = newAI(world, scene, instance_id, type_id,
			                      pos.x,
			                      pos.y
			                     );
			break;
			}
		case EID_DES:
			{
			Destroyable_t d = data.entity_type.destroyable;
			// int h = d.health;
			return_entity = newDestroyable(world, scene, instance_id, type_id,
			                               pos.x,
					               pos.y,
					               std::string(d.mesh_name)
			                              );
			break;
			}
		case EID_NONDES:
			{
			NonDestroyable_t n = data.entity_type.nondestroyable;
			return_entity = newNonDestroyable(world, scene, instance_id, type_id,
			                                  pos.x,
			                                  pos.y,
			                                  std::string(n.mesh_name)
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
	entity_player.addComponent<tempo::ComponentRender>(scene, "TODO").node->attachObject(Pset);
	entity_player.addComponent<tempo::ComponentGridPosition>(system_grid_motion.spawn());
	entity_player.addComponent<tempo::ComponentGridMotion>();
	entity_player.addComponent<tempo::ComponentPlayerInput>();
	// entity_player.getComponent<tempo::ComponentRender>().AddHealthBar();
	// entity_player.addComponent<tempo::ComponentHealth>(health);
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
	entity_ai.addComponent<tempo::ComponentRender>(scene, "TODO").node->attachObject(Aset);
	entity_ai.addComponent<tempo::ComponentGridPosition>(x, y, tempo::tileMask1by1, false);
	entity_ai.addComponent<tempo::ComponentGridMotion>();
	entity_ai.addComponent<tempo::ComponentGridAi>();
	// entity_ai.getComponent<tempo::ComponentRender>().AddHealthBar();
	// entity_ai.addComponent<tempo::ComponentHealth>(health);
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
	entity_object.addComponent<tempo::ComponentGridPosition>(x, y, tempo::tileMask1by1, false);
	entity_object.addComponent<tempo::ComponentGridMotion>();
	// entity_object.addComponent<tempo::ComponentHealth>(health);
	// entity_object.getComponent<tempo::ComponentRender>().AddHealthBar();

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
