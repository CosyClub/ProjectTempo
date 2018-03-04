#ifndef TEMPO_COMPONENT_STAGE_HPP
#define TEMPO_COMPONENT_STAGE_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <vector>

#include <tuple>

namespace tempo
{
// This implementation may change without notice
typedef std::vector<std::tuple<glm::ivec2, float>> stage_tiles;

extern stage_tiles _global_stage
extern bool        _global_stage_loaded

// An entity that is bound by the world stage
struct ComponentStage
    : anax::Component
    , NetworkedComponent {
   private:
	stage_tiles* tiles;
	std::string stage_file;

	void loadLevel(const char *stage_file);

  // Finds the index for position
  inline int findIndex(glm::ivec2 position);

   public:
	// Constructor requires stage file
	ComponentStage(const char *stage_file);

	// Gets a list of
	stage_tiles getHeights();

	// Get height at
	float getHeight(glm::ivec2 position);

  void setHeight(glm::ivec2 position, int height);

  void setHeight(std::vector<glm::ivec2> positions, int height);

	// Returns true if there is a tile at position
	bool existstTile(glm::ivec2 position);

	/////
	// Required for networking
	/////
	ComponentStage(sf::Packet p);
	sf::Packet  dumpComponent();
	ComponentID getId();
};

}  // namespace tempo

#endif
