#ifndef CLIENT_MISC_LIGHTING
#define CLIENT_MISC_LIGHTING

#include <irrlicht.h>
#include <vector3d.h>
#include <IVolumeLightSceneNode.h>

#include <glm/vec2.hpp>

#include <vector>

// Contains functions for all lighting elements that do not require updating after initialisation

namespace client {

	void createLasers(irr::scene::ISceneManager *smgr,
					  irr::video::IVideoDriver * driver, 
					  std::vector<glm::ivec2> positions);

	void createDiscoBalls(irr::scene::ISceneManager *smgr,
					  irr::video::IVideoDriver * driver, 
					  std::vector<glm::ivec2> positions);
}

#endif
