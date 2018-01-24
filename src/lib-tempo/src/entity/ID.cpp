////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Render.cpp
/// \author Jamie Terry
/// \date 2017/11/14
/// \brief Contains definition of Render system functions
////////////////////////////////////////////////////////////////////////////

#include <tempo/entity/ID.hpp>

namespace tempo{

int globalIDCounter = 0;
std::map<int, anax::Entity> id_map;

ComponentID::ComponentID(int tid)
{
	this->type_id = tid;
	this->instance_id = globalIDCounter;
	
	globalIDCounter++;
}

ComponentID::ComponentID(int iid, int tid)
{
	type_id = tid;
	instance_id = iid;
}

ComponentID::~ComponentID(){
}

SystemID::SystemID()
{
}

SystemID::~SystemID()
{
}

anax::Entity SystemID::get(int instance_id){
	return id_map.find(instance_id)->second;
}

void OnEntityAdded(anax::Entity& e)
{
	int instance_id = e.getComponent<ComponentID>().instance_id;
	id_map[instance_id] = e;
}
void OnEntityRemoved(anax::Entity& e);

}
