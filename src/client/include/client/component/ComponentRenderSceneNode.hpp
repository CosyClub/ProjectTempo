#ifndef CLIENT_COMPONENT_RENDER_SCENE_NODE_HPP
#define CLIENT_COMPONENT_RENDER_SCENE_NODE_HPP

#include <anax/Component.hpp>

#include <ISceneNode.h>
#include <ITexture.h>
#include <irrlicht.h>

#include <glm/vec2.hpp>

#include <string>

namespace client
{
struct ComponentRenderSceneNode : public anax::Component {

	bool isMesh;
	glm::ivec2 spriteDim;
	glm::vec2 spritePos;
	bool updateNeeded = false;
	irr::scene::IBillboardSceneNode * billboard;
	irr::scene::ISceneNode *node;
	
	ComponentRenderSceneNode(irr::scene::ISceneNode *node);
	~ComponentRenderSceneNode();
};

}  // namespace client

#endif
