#include <server/system/SystemMovement.hpp>

#include <tempo/component/ComponentStageRotation.hpp>

#include <tempo/network/server.hpp>
#include <tempo/network/ID.hpp>

namespace server
{

using tempo::operator<<;
using tempo::operator>>;

void SystemMovement::processTranslation()
{
	for (auto &entity : getEntities()) {
		tempo::ComponentStageTranslation &st = entity.getComponent<tempo::ComponentStageTranslation>();
		tempo::ComponentStage &stage = entity.getComponent<tempo::ComponentStage>();
		if (st.delta.x == 0 && st.delta.y == 0)
			continue;

		bool  can_move  = true;
		auto &positions = entity.getComponent<tempo::ComponentStagePosition>().occupied;

		for (auto &position : positions) {
			if (!stage.existstTile(position + st.delta))
				can_move = false;
		}

		if (can_move) {
			for (auto &position : positions) {
				position += st.delta;
			}
		}

		st.delta = {0, 0};

		// Send update to everyone
		sf::Packet p;
		p << entity.getId();
		p << static_cast<uint32_t>(positions.size());
		for (auto &position : positions) {
			p << position.x << position.y;
		}
		
		// Add facing direction
		if (entity.hasComponent<tempo::ComponentStageRotation>()) {
			p << entity.getComponent<tempo::ComponentStageRotation>().facing.x;
			p << entity.getComponent<tempo::ComponentStageRotation>().facing.y;
		}
		tempo::broadcastMessage(tempo::QueueID::MOVEMENT_UPDATES, p);
	}
}

}  // namespace server
