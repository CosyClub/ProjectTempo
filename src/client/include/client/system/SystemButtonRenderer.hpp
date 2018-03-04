#ifndef CLIENT_SYSTEM_BUTTON_RENDERER_HPP
#define CLIENT_SYSTEM_BUTTON_RENDERER_HPP


#include <anax/System.hpp>

#include <client/component/ComponentRenderButtonGroup.hpp>
#include <tempo/component/ComponentButtonGroup.hpp>

#include <ISceneManager.h>
#include <IVideoDriver.h>

namespace client
{
// Initalises all rendering for buttons
class SystemButtonRenderer
    : public anax::System<
        anax::Requires<tempo::ComponentButtonGroup, client::ComponentRenderButtonGroup>>
{
   public:
	void setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver);

	void updateButtons(irr::video::IVideoDriver *driver);
};
}  // namespace client

#endif
