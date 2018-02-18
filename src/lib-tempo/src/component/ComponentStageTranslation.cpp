#include <tempo/component/ComponentStageTranslation.hpp>

namespace tempo
{

ComponentStageTranslation::ComponentStageTranslation() 
: delta(0, 0) 
{
	id = Component_ID::CID_TRANSFORM;		
}
sf::Packet ComponentStageTranslation::dumpComponent()
{
	sf::Packet p;
	p << delta.x;
	p << delta.y;

	return p;
}

void ComponentStageTranslation::restoreComponent(sf::Packet p)
{
	p >> delta.x;
	p >> delta.y;
}

}
