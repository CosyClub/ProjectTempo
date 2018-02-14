#ifndef TEMPO_COMPONENT_PLAYERREMOTE_HPP
#define TEMPO_COMPONENT_PLAYERREMOTE_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentGridMotion.hpp>
#include <tempo/system/SystemID.hpp>
#include <tempo/entity/SystemQID.hpp>
#include <tempo/network/queue.hpp>
#include <tempo/time.hpp>

#include <SDL.h>
#include <iostream>
#include <cstdio>
#include <stdexcept>

#undef main // SDL likes to define main

namespace tempo{

	struct ComponentPlayerRemote : anax::Component 
	{
		bool moved_this_beat;

		inline ComponentPlayerRemote() : moved_this_beat(false) {}
	};

}

#endif