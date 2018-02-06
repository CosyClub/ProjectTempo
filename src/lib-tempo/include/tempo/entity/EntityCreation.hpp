#ifndef TEMPO_ENTITY_ENTITYCREATION_HPP
#define TEMPO_ENTITY_ENTITYCREATION_HPP

#include <tempo/math/Vector.hpp>
#include <SFML/Network.hpp>

namespace tempo
{

enum EID {
	EID_PLAYER,
	EID_AI,
	EID_DES,
	EID_NONDES,
};

typedef struct {
	int current_health;
	int max_health;
	/* char  name[100]; */
	/* float color[3]; */
} Player_t;

typedef struct {
	int current_health;
	int max_health;
} AI_t;

typedef struct {
	char mesh_name[100];
	 int health;
} Destroyable_t;

typedef struct {
	char mesh_name[100];
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
	int instance_id;
	Entity_Type entity_type;
} EntityCreationData;

sf::Packet& operator <<(sf::Packet& packet, const EntityCreationData& data);
sf::Packet& operator <<(sf::Packet& packet, const Entity_Type& type);
sf::Packet& operator <<(sf::Packet& packet, const Vec2s& vec);

sf::Packet& operator >>(sf::Packet& packet, EntityCreationData& data);
sf::Packet& operator >>(sf::Packet& packet, Entity_Type& type);
sf::Packet& operator >>(sf::Packet& packet, Vec2s& vec);

}

#endif
