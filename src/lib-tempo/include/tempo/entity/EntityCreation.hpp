#ifndef TEMPO_ENTITY_ENTITYCREATION_HPP
#define TEMPO_ENTITY_ENTITYCREATION_HPP

#include <anax/World.hpp>

#include <Ogre.h>

#include <tempo/entity/Render.hpp>
#include <tempo/entity/LevelManager.hpp>
#include <tempo/entity/GridAi.hpp>
#include <tempo/entity/PlayerInput.hpp>

namespace tempo
{

enum EID {
	EID_PLAYER,
	EID_AI,
	EID_DES,
	EID_NONDES,
};

typedef struct {
	std::string name;
	float color[3];
} Player_t;

typedef struct {
	int some_data_for_ai;
} AI_t;

typedef struct {
	int some_data_for_destroyable;
} Destroyable_t;

typedef struct {
	int some_data_for_nondestroyable;
} NonDestroyable_t;

typedef union {
	Player_t player;
	AI_t ai;
	Destroyable_t destroyable;
	NonDestroyable_t nondestroyable;
} Entity_Type;

typedef struct {
	EID type_id;
	Vec2s position;
	Entity_Type entity_type;
} EntityCreationData;

EntityCreationData* newEntity(int type_id, Vec2s pos);

sf::Packet& operator <<(sf::Packet& packet, const EntityCreationData& data);
sf::Packet& operator <<(sf::Packet& packet, const Entity_Type& type);
sf::Packet& operator <<(sf::Packet& packet, const Vec2s& vec);

anax::Entity newPlayer(anax::World& world, Ogre::SceneManager* scene, tempo::SystemLevelManager system_grid_motion);
anax::Entity newAI(anax::World& world, Ogre::SceneManager* scene, int x, int y);
anax::Entity newDestroyable(anax::World& world, Ogre::SceneManager* scene, int x, int y, std::string mesh_name);
anax::Entity newNonDestroyable(anax::World& world, Ogre::SceneManager* scene, int x, int y, std::string mesh_name);

}

#endif
