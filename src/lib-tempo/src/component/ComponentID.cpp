#include <tempo/component/ComponentID.hpp>


namespace tempo {

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

	ComponentID::~ComponentID() {
	}

}
