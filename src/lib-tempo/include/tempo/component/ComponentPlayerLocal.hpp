#ifndef TEMPO_COMPONENT_PLAYERLOCAL_HPP
#define TEMPO_COMPONENT_PLAYERLOCAL_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentID.hpp>
#include <tempo/network/client.hpp>
#include <tempo/network/QueueID.hpp>
#include <tempo/time.hpp>

#include <iostream>
#include <cstdio>
#include <SDL.h>

#undef main // SDL likes to define main

namespace tempo{

	struct ComponentPlayerLocal : anax::Component {
		bool moved_this_beat;
		unsigned int counter_combo;
		unsigned int level_combo;

		inline ComponentPlayerLocal() : moved_this_beat(false) {}
	};
}

#endif
