#ifndef CLIENT_SYSTEM_SYSTEM_GRAPHICS_CREATION_HPP
#define CLIENT_SYSTEM_SYSTEM_GRAPHICS_CREATION_HPP

#include <irrlicht.h>
#include <anax/System.hpp>
#include <anax/World.hpp>

namespace client
{
struct SystemGraphicsCreation
    : anax::System<anax::Requires<>> {
	SystemGraphicsCreation();
	void addEntities(irr::video::IVideoDriver * driver,
	                 irr::scene::ISceneManager *manager,
	                 anax::World &              world);
};

}  // namespace tempo

#endif
