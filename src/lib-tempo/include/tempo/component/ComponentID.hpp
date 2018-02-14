#ifndef TEMPO_COMPONENT_ID_HPP
#define TEMPO_COMPONENT_ID_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>
#include <map>
#include <iostream>
#include <stdexcept>

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

}

#endif
