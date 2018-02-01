#include <tempo/entity/ID.hpp>


namespace tempo{

int globalIDCounter = 0;

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
	if (id_map.find(instance_id) != id_map.end())
	{
		return id_map.find(instance_id)->second;
	}
	else
	{
		std::cout << "Entity with ID " << instance_id 
		          << " not found in map" << std::endl;
		throw std::out_of_range("Entity not found in map");
	}
}

void SystemID::onEntityAdded(anax::Entity& e)
{
	int instance_id = e.getComponent<ComponentID>().instance_id;
	id_map[instance_id] = e;
}
void SystemID::onEntityRemoved(anax::Entity& e)
{
	id_map.erase(id_map.find(e.getComponent<ComponentID>().instance_id));
}

}
