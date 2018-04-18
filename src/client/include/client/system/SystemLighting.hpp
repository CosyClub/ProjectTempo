#ifndef CLIENT_SYSTEM_LIGHTING_HPP
#define CLIENT_SYSTEM_LIGHTING_HPP

#include <anax/System.hpp>

#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <vector>

#include <irrlicht.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace client
{
	// Initalises all rendering for the static stage
	class SystemLighting : public anax::System<anax::Requires<>>
	{
	private:
		/// This is the main light responsible for all the stage colors:
		irr::scene::ILightSceneNode* light_ML1;
		irr::video::SLight data_ML1;

	public:
		void setup(irr::scene::ISceneManager *smgr,
				   irr::scene::ISceneNode *parent);

		void update(irr::video::SColor colour);
	};
}

#endif
