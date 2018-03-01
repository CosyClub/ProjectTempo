#include <tempo/system/SystemAttack.hpp>

#include <tempo/component/ComponentAOEIndicator.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentWeapon.hpp>

#include <tempo/network/QueueID.hpp>
#include <tempo/network/base.hpp>
#include <tempo/mask.hpp>

#include <glm/glm.hpp>

namespace tempo
{

void SystemAttack::Attack(anax::Entity attacker)
{
	//Attacker
	glm::ivec2 attackerpos = attacker.getComponent<tempo::ComponentStagePosition>().getOrigin();
	glm::ivec2 rot = attacker.getComponent<tempo::ComponentStageRotation>().facing;
	auto& weapon = attacker.getComponent<tempo::ComponentWeapon>();
	Mask m = weapon.damage;

	if (weapon.isDelayed)
	{
		
	}

	//Victim
	auto entities = getEntities();

	for (auto& entity : entities) {

		//TODO some team system

		glm::ivec2 pos = entity.getComponent<tempo::ComponentStagePosition>().getOrigin();
		auto& health = entity.getComponent<tempo::ComponentHealth>();

		glm::vec2 forward = rot;
		glm::vec2 left = glm::ivec2(rot.y * -1, rot.x * -1); //Hacky cross product

		glm::vec2 diff = pos - attackerpos;
		glm::ivec2 relative_diff = glm::ivec2(glm::dot(diff, left), glm::dot(diff, forward));

		float damage = weapon.GetDamage(relative_diff);
		if ( damage != 0 )
		{
			std::cout << "hit entity " << entity.getId().index
			          << " for " << damage << std::endl;
		}
		health.HealthUpdate( -1 * damage );	
	}

	sf::Packet p;
	p << Messages::ATTACK;
	p << localtoserver[attacker.getId()]; 
	sendMessage(QueueID::SYSTEM_ATTACK, p);
}

void SystemAttack::Broadcast(anax::World& w)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(QueueID::SYSTEM_ATTACK);

	while( ! q->empty() )
	{
		sf::Packet p = q->front();
		q->pop();

		broadcastMessage(QueueID::SYSTEM_ATTACK, p);
	}
}

void SystemAttack::Recieve(anax::World& w)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(QueueID::SYSTEM_ATTACK);

	while( ! q->empty() )
	{
		sf::Packet p = q->front();
		q->pop();

		int code;
		p >> code;

		anax::Entity::Id id;
		p >> id;
		id = servertolocal[id];
		anax::Entity e(w, id);

		switch( code )
		{
			case Messages::ATTACK:
				{

				if ( ! e.hasComponent<ComponentAOEIndicator>() )
				{
					std::cout << "Entity does not contain ComponentAOEIndicator" << std::endl;
					assert(false);
				}
				ComponentAOEIndicator aoe = e.getComponent<ComponentAOEIndicator>();
				auto& weapon = e.getComponent<tempo::ComponentWeapon>();
				aoe.duration = sf::seconds( 0.1 );
				aoe.tiles = weapon.damage.positions;
				//Do some animating?	
				  
				}
				break;
			case Messages::DELAYED_ATTACK:
				//TODO
				break;
			default:
				std::cout << "ATTACK: Unhandled message" << std::endl;
		}
	}
}

}
