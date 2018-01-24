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

extern int globalIDCounter;

struct ComponentID : anax::Component {
	int instance_id;
	int type_id;

	//only for server
	ComponentID(int eid);

	//client
	ComponentID(int tid, int eid);

	//possibly unnecessary
	~ComponentID();
};

}

#endif
