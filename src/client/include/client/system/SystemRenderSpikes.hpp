#ifndef CLIENT_SYSTEM_RENDER_SPIKES_HPP
#define CLIENT_SYSTEM_RENDER_SPIKES_HPP


#include <anax/System.hpp>

#include <client/component/ComponentRenderSpikes.hpp>
#include <tempo/component/ComponentSpikes.hpp>

#include <ISceneManager.h>
#include <IVideoDriver.h>

namespace client
{
// Initalises all rendering for buttons
class SystemRenderSpikes
    : public anax::System<
        anax::Requires<tempo::ComponentSpikes, client::ComponentRenderSpikes>>
{
   public:
	void setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver);

	void updateSpikes(irr::video::IVideoDriver *driver, const glm::ivec2 playerpos);
};
}  // namespace client

#endif
