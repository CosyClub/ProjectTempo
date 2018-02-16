#include <tempo/network/QueueID.hpp>

namespace tempo
{

void (*restore_pointer[Component_ID::CID_RESERVED_TOP])(anax::Entity& e, sf::Packet p);

sf::Packet (*dump_pointer[Component_ID::CID_RESERVED_TOP])(anax::Entity& e);

}
