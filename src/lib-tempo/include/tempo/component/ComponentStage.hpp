#ifndef TEMPO_COMPONENT_STAGE_HPP
#define TEMPO_COMPONENT_STAGE_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <unordered_map>
#include <tuple>

namespace tempo
{
// This implementation may change without notice

struct stage_tile {
	glm::ivec2 position;
	float      height;

	stage_tile(glm::ivec2 position, float height)
	    : position(position)
	    , height(height)
	{
	}
	stage_tile() {}
};

struct vec2less
{
	bool operator()(const glm::ivec2& a, const glm::ivec2& b)const
	{
		return (a.x < b.x) || ((a.x == b.x) && (a.y < b.y));
	}
};

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


template< typename tPair >
struct second_t {
	typename tPair::second_type operator()( const tPair& p ) const { return p.second; }
};

typedef std::unordered_map<glm::ivec2,
                           stage_tile,
                           vec2eq,
                           vec2eq,
                           std::allocator<std::pair<const glm::ivec2, stage_tile>>> tileMap;

typedef std::unordered_map<glm::ivec2,
                          bool,
                          vec2eq,
                          vec2eq,
                          std::allocator<std::pair<const glm::ivec2, stage_tile>>> heightDeltaMap;

extern tileMap _global_stage;
extern heightDeltaMap _global_heightDelta;
extern std::string             _global_stage_loaded;

// An entity that is bound by the world stage
struct ComponentStage
    : anax::Component
    , NetworkedComponent {
   private:
	tileMap *tiles;
	std::string              stage_file;

	void loadLevel(const char *stage_file);

   public:
	heightDeltaMap *heightDelta;

	// Constructor requires stage file
	ComponentStage(const char *stage_file);

	// Gets a list of
	std::vector<tempo::stage_tile> getHeights();

	// Get height at
	float getHeight(glm::ivec2 position);

	void setHeight(glm::ivec2 position, int height);

	void setHeight(std::vector<glm::ivec2> positions, int height);

	// Returns true if there is a tile at position
	bool existstTile(glm::ivec2 position);

	bool isNavigable(glm::ivec2 position, float height);

	/////
	// Required for networking
	/////
	ComponentStage(sf::Packet p);
	sf::Packet  dumpComponent();
	ComponentID getId();
};

}  // namespace tempo

#endif
