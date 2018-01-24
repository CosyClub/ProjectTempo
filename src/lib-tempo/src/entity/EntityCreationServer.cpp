#include <tempo/entity/EntityCreationServer.hpp>

namespace tempo
{

EntityCreationData dumpEntity(anax::Entity e)
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

}
