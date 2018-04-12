#include <tempo/entity/EntityCreation.hpp>

#include <tempo/component/ComponentAOEIndicator.hpp>
#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentButtonGroup.hpp>
#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentAI.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentModel.hpp>
#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/component/ComponentSpikes.hpp>
#include <tempo/component/ComponentStage.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentTeam.hpp>
#include <tempo/component/ComponentWeapon.hpp>
#include <tempo/component/NetworkedComponent.hpp>

#include <tempo/network/base.hpp>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

#include <iostream>

#include <cassert>

#define CASE(NAME, CID)                                                                            \
	case ComponentID::CID:                                                                         \
		if (!e.hasComponent<NAME>()) {                                                             \
			e.addComponent<NAME>(part);                                                            \
		} else {                                                                                   \
			std::cout << "Warning: Reinstanciation of "                                            \
			          << "" #NAME << std::endl;                                                    \
		}                                                                                          \
		break;

namespace tempo
{
anax::Entity addComponent(anax::World &w, sf::Packet p)
{
	anax::Entity::Id   id;
	anax::Entity::Id   localid;
	anax::Entity       e;
	tempo::ComponentID component_id;

	p >> id;
	auto a = servertolocal.find(id);
	if (a == servertolocal.end()) {
		// Looks like it's a new one
		std::cout << "Received New Entity with server ID " << id.index << std::endl;
		e       = w.createEntity();
		localid = e.getId();
		servertolocal.emplace(id, localid);
		localtoserver.emplace(localid, id);
		e.activate();
	} else {
		std::cout << "Using current Entity with server ID " << id.index << std::endl;
		localid = a->second;
		e       = anax::Entity(w, localid);
		e.activate();
	}

	while (!p.endOfPacket()) {
		uint32_t size = 0;
		p >> size;
		std::cout << "Splitting packet to size " << size << std::endl;
		sf::Packet part = splitPacket(p, size);

		part >> component_id;
		std::cout << "Adding Component with ID " << component_id << std::endl;
		switch (component_id) {
			// Put new Components in here
			CASE(ComponentAOEIndicator, AOEINDICATOR)
			CASE(ComponentAttack, ATTACK)
			CASE(ComponentButtonGroup, BUTTON_GROUP)
			CASE(ComponentCombo, COMBO)
			CASE(ComponentAI, AI)
			CASE(ComponentHealth, HEALTH)
			CASE(ComponentModel, MODEL)
			CASE(ComponentPlayerLocal, PLAYER_LOCAL)
			CASE(ComponentPlayerRemote, PLAYER_REMOTE)
			CASE(ComponentSpikes, SPIKES)
			CASE(ComponentStage, STAGE)
			CASE(ComponentStagePosition, STAGE_POSITION)
			CASE(ComponentStageRotation, STAGE_ROTATION)
			CASE(ComponentTeam, TEAM)
			CASE(ComponentStageTranslation, STAGE_TRANSLATION)
			CASE(ComponentWeapon, WEAPON)

		// End of new Component Zone
		default:
			std::cout << "WARNING: Unimplemented deserialisation of"
			             " received component occured, ignoring."
			          << std::endl;
			assert(false);
		}
	}

	e.activate();
	std::cout << "Finished processing Entity with server ID " << id.index << " and "
	          << e.getComponentTypeList().count() << " components and validity " << e.isValid()
	          << std::endl;

	return e;
}

}  // namespace tempo
