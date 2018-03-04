#ifndef CLIENT_SYSTEM_ATTACK_HPP
#define CLIENT_SYSTEM_ATTACK_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <cassert>
#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentStagePosition.hpp>

namespace tempo
{

struct SystemAttack : anax::System<anax::Requires<>> 
{
};

} // namespace tempo

#endif
