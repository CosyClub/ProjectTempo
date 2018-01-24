////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Render.hpp
/// \author Jamie Terry
/// \date 2017/11/14
/// \brief Declares the the Render component and system
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_ID_HPP
#define TEMPO_ENTITY_ID_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

namespace tempo{

int globalIDCounter = 0;

struct ComponentID : anax::Component {
	int ID;

	//only for server
	ComponentID();

	//client
	ComponentID(int id);

	//possibly unnecessary
	~ComponentID();
};

}

#endif
