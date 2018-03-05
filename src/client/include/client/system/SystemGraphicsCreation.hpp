#ifndef CLIENT_SYSTEM_SYSTEM_GRAPHICS_CREATION_HPP
#define CLIENT_SYSTEM_SYSTEM_GRAPHICS_CREATION_HPP

#include <irrlicht.h>
#include <anax/System.hpp>

#include <client/component/ComponentRenderSceneNode.hpp>
#include <tempo/component/ComponentModel.hpp>
#include <tempo/component/ComponentStagePosition.hpp>

namespace client
{
struct SystemGraphicsCreation
    : anax::System<anax::Requires<tempo::ComponentStagePosition, tempo::ComponentModel>> {
	SystemGraphicsCreation();
	void addEntities(irr::video::IVideoDriver *driver, irr::scene::ISceneManager *manager);
};

}  // namespace tempo

#endif
