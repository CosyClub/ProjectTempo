#ifndef CLIENT_SYSTEM_BUTTON_RENDERER_HPP
#define CLIENT_SYSTEM_BUTTON_RENDERER_HPP


#include <anax/System.hpp>

#include <client/component/ComponentRenderButtonGroup.hpp>
#include <tempo/component/ComponentButtonGroup.hpp>

#include <ISceneManager.h>
#include <IVideoDriver.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace client
{
// Initalises all rendering for buttons
class SystemButtonRenderer
    : public anax::System<
        anax::Requires<tempo::ComponentButtonGroup, client::ComponentRenderButtonGroup>>
{
   public:
	void setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver);

	void updateButtons(const glm::ivec2 playerpos);

private:
	irr::video::ITexture *buttonBlocked;
	irr::video::ITexture *buttonArrow;

	//Used to rotate the button so that the arrow is facing the correct way
	void setRotation(std::vector<anax::Entity> entities, uint32_t i, uint32_t j, irr::scene::IMeshSceneNode *button, glm::ivec2 pos);
};
}  // namespace client

#endif
