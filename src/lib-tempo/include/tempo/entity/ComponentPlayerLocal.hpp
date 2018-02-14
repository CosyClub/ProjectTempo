#ifndef TEMPO_COMPONENT_PLAYERLOCAL_HPP
#define TEMPO_COMPONENT_PLAYERLOCAL_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentGridMotion.hpp>
#include <tempo/component/ComponentID.hpp>
#include <tempo/entity/SystemQID.hpp>
#include <tempo/network/client.hpp>
#include <tempo/time.hpp>

#include <iostream>
#include <cstdio>
#include <SDL.h>

#undef main // SDL likes to define main

namespace tempo{

	struct ComponentPlayerLocal : anax::Component {
		bool moved_this_beat;
		uint counter_combo;
		uint level_combo;

		inline ComponentPlayerLocal() : moved_this_beat(false) {}
	};
}

#endif
