#include <tempo/system/SystemPlayer.hpp>

namespace tempo
{

anax::Entity SystemPlayer::nearest_player(glm::ivec2 pos)
{
	float min = 9999999;
	anax::Entity out;
	for (auto entity : getEntities())
	{
		glm::vec2 diff = pos - entity.getComponent<ComponentStagePosition>().getOrigin();
		if (length(diff) < min)
		{
			min = length(diff);
			out = entity;
		}

	}

	return out;
}

}
