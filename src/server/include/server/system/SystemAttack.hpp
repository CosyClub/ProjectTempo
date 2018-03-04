#ifndef SERVER_SYSTEM_ATTACK_HPP
#define SERVER_SYSTEM_ATTACK_HPP

#include <anax/System.hpp>

#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentStagePosition.hpp>

namespace server
{

struct SubSystemAttack : anax::System<anax::Requires<tempo::ComponentHealth, tempo::ComponentStagePosition>> 
{
	// Attack
	// If attack is made, all attackable entities (have ComonentHealth)
	// are checked to see if they are inside the attack range
	//
	// Arguments:
	//          anax::Entity attacker - The entity that has chosen to attack
	// Returns:
	//          void
	void Attack(anax::Entity attacker);

	void Broadcast(anax::World &w);
	void Recieve(anax::World &w);

	enum Messages {
		ATTACK,
		DELAYED_ATTACK,
	};
};

struct SystemAttack : anax::System<anax::Requires<tempo::ComponentAttack>> 
{
	SubSystemAttack subSystem;
	
	// Sets up the SystemAttack along with it's necessary subsystem
	SystemAttack(anax::World &world);
	
	// Processes all of the attack intents. To be run at Delta End.
	void processAttacks();
};

} // namespace server

#endif
