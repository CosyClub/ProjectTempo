#ifndef SERVER_SYSTEM_MOVEMENT_HPP
#define SERVER_SYSTEM_MOVEMENT_HPP

#include <tempo/component/ComponentStage.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>

#include <anax/System.hpp>

namespace server
{
struct SystemMovement
    : anax::System<anax::Requires<tempo::ComponentStage,
                                  tempo::ComponentStageTranslation,
                                  tempo::ComponentStagePosition>> {
	// Receive Movement Intents and Broadcasts them to everyone else
	void receiveTranslations(anax::World &world);

	// Processes Intents and Broadcasts Corrections
	void processTranslation();
};

}  // namespace server

#endif
