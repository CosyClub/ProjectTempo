////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file GridAi.hpp
/// \author Jamie Terry
/// \date 2017/11/15
/// \brief Contains decerlation of grid ai component
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_GRIDAI_HPP
#define TEMPO_ENTITY_GRIDAI_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/GridMotion.hpp>

namespace tempo{

  struct ComponentGridAi : anax::Component {
	  // no state needed for AI (yet)
  };

	struct SystemGridAi : anax::System<anax::Requires<ComponentGridMotion, ComponentGridAi>> {
		void update();
	};
}

#endif
