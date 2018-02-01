#ifndef TEMPO_ENTITY_ID_HPP
#define TEMPO_ENTITY_ID_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>
#include <map>

namespace tempo{

extern int globalIDCounter;
extern std::map<int, anax::Entity> id_map;
	
struct ComponentID : anax::Component {
	int instance_id;
	int type_id;

	//only for server
	ComponentID(int eid);

	//client
	ComponentID(int tid, int eid);

	//possibly unnecessary
	~ComponentID();
};

struct SystemID : anax::System<anax::Requires<ComponentID>> {

	SystemID();
	~SystemID();

	anax::Entity get(int instance_id);
	void onEntityAdded(anax::Entity& e);
	void onEntityRemoved(anax::Entity& e);
};

}

#endif
