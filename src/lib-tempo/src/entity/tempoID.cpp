////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Render.cpp
/// \author Jamie Terry
/// \date 2017/11/14
/// \brief Contains definition of Render system functions
////////////////////////////////////////////////////////////////////////////

#include <tempo/entity/Render.hpp>

namespace tempo{

ComponentID::ComponentID()
{
	ID = globalIDCounter;
	globalIDCounter++;
}

ComponentID::ComponentID(int id)
{
	ID = id;
}

ComponentID::~ComponentID(){
}

}
