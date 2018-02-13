#ifndef TEMPO_ENTITY_ENTITYCREATION_HPP
#define TEMPO_ENTITY_ENTITYCREATION_HPP

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
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
	int foo;
	// int health;
	/* char  name[100]; */
	/* float color[3]; */
} Player_t;

typedef struct {
	int foo;
	// int health;
} AI_t;

typedef struct {
	char mesh_name[100];
	// int health;
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
	glm::vec2 position;
	int instance_id;
	Entity_Type entity_type;
} EntityCreationData;

sf::Packet& operator <<(sf::Packet& packet, const EntityCreationData& data);
sf::Packet& operator <<(sf::Packet& packet, const Entity_Type& type);
sf::Packet& operator <<(sf::Packet& packet, const glm::vec2& vec);

sf::Packet& operator >>(sf::Packet& packet, EntityCreationData& data);
sf::Packet& operator >>(sf::Packet& packet, Entity_Type& type);
sf::Packet& operator >>(sf::Packet& packet, glm::vec2& vec);

}

#endif
