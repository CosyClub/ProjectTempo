////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_COMPONENT_GRIDPOSITION_HPP
#define TEMPO_COMPONENT_GRIDPOSITION_HPP

#include <anax/World.hpp>
#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/TileMask.hpp>
#include <tempo/math/Vector.hpp>

#include <time.h>
#include <limits>
#include <iostream> //TODO do we really need to print things here
#include <SDL.h>

#undef main // SDL likes to define main

namespace tempo{

	/////////////////////////////////////////////////////////////////////
	/// \brief Stores information regarding an entities position on the grid,
	/// as well as which tiles the entity occupies
	/// \note Cannot directly modify the values of this component, must instead
	/// call methods of SystemLevelManager which will update the fields of this
	/// component, or alternatively have them be modified via a ComponentGridMotion
	/////////////////////////////////////////////////////////////////////
	class ComponentGridPosition : public anax::Component {
	private:
		///< \brief The position of the center of this entity
		Vec2s position;

		///< \brief Mask of the tile's that this entity occupies
		TileMask mask;

		///< \brief A tile may be occupied by either:
		/// - A single non-ethereal entity
		/// - Many ethereal entities
		/// This allows, for example, players to move through each other,
		/// but not through collapsed walls/enemies etc.
		bool ethereal;
	public:
		inline const Vec2s&    getPosition(){ return this->position; }
		inline const TileMask& getTileMask(){ return this->mask;     }
		inline const bool      isEthereal (){ return this->ethereal; }

		void setPosition(Vec2s position);

		ComponentGridPosition(Vec2s pos        = {0,0},
		                      TileMask         = tempo::tileMask1by1,
		                      bool is_ethereal = true
		                     );

		ComponentGridPosition(int x, int y,
		                      TileMask         = tempo::tileMask1by1,
		                      bool is_ethereal = true
		                     );

		/////////////////////////////////////////////////////////////////////
		/// \brief Returns the mask of tiles that are occupied by both this and the
		/// other entity. Returned TileMask is centred on same position as
		/// this->mask
		/////////////////////////////////////////////////////////////////////
		TileMask isCollidingWith(const ComponentGridPosition& other);
	};
}

#endif
