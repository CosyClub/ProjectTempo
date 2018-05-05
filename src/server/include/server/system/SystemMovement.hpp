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

	struct vec2eq
	{
		size_t operator()(const glm::ivec2& k)const
		{
		    return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
		}
		
		bool operator()(const glm::ivec2& a, const glm::ivec2& b)const
		{
		        return a.x == b.x && a.y == b.y;
		}
	};


	typedef std::unordered_map<glm::ivec2,
	                           bool,
	                           vec2eq,
	                           vec2eq,
	                           std::allocator<std::pair<const glm::ivec2, bool>>> collMap;

	collMap collisionMap;

	// Receive Movement Intents and Broadcasts them to everyone else
	void receiveTranslations(anax::World &world);

	// Processes Intents and Broadcasts Corrections
	void processTranslation();
};

}  // namespace server

#endif
