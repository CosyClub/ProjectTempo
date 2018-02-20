#ifndef TEMPO_COMPONENT_STAGE_HPP
#define TEMPO_COMPONENT_STAGE_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

#include <glm/fwd.hpp>

#include <tuple>

namespace tempo 
{

// This implementation may change without notice
typedef std::vector<std::tuple<glm::ivec2, float>> stage_tiles;

// An entity that is bound by the world stage
struct ComponentStage : anax::Component, NetworkedComponent 
{
	private:
		stage_tiles tiles;
		std::string stage_file;

		void loadLevel(const char * stage_file);
	public:
		// Constructor requires stage file
		ComponentStage(const char* stage_file);

		// Gets a list of 
		stage_tiles getHeights();
	
		/////
		// Required for networking
		/////
		ComponentStage(sf::Packet p);
		sf::Packet dumpComponent();
		ComponentID getId();
};

} // namespace tempo

#endif
