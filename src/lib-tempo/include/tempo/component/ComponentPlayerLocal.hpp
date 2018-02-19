#ifndef TEMPO_COMPONENT_PLAYERLOCAL_HPP
#define TEMPO_COMPONENT_PLAYERLOCAL_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/NetworkedComponent.hpp>
#include <tempo/network/client.hpp>
#include <tempo/network/QueueID.hpp>
#include <tempo/time.hpp>

#include <iostream>
#include <cstdio>
#include <SDL.h>

#undef main // SDL likes to define main

namespace tempo{

struct ComponentPlayerLocal : anax::Component, NetworkedComponent {
	ComponentID id;

	bool moved_this_beat;
	uint32_t counter_combo;
	uint32_t level_combo;

	inline ComponentPlayerLocal() : moved_this_beat(false) {}

	//Required for inital network sync
	ComponentPlayerLocal(sf::Packet p);
	sf::Packet dumpComponent();
};

}

#endif
