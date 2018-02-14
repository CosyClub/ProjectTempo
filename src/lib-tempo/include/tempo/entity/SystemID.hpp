#ifndef TEMPO_SYSTEM_ID_HPP
#define TEMPO_SYSTEM_ID_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>
#include <map>
#include <iostream>
#include <stdexcept>

#include<tempo/entity/ComponentID.hpp>

namespace tempo{

struct SystemID : anax::System<anax::Requires<ComponentID>> {
	std::map<int, anax::Entity> id_map;

	SystemID();
	~SystemID();

	anax::Entity get(int instance_id);
	void onEntityAdded(anax::Entity& e);
	void onEntityRemoved(anax::Entity& e);
};

}

#endif
