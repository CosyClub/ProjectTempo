#ifndef CLIENT_COMPONENT_RENDER_SCENE_NODE_HPP
#define CLIENT_COMPONENT_RENDER_SCENE_NODE_HPP

#include <anax/Component.hpp>

#include <ISceneNode.h>
#include <ITexture.h>
#include <irrlicht.h>

namespace client
{
struct ComponentRenderSceneNode : public anax::Component {

	std::string spritesheet;
	int spriteRows = 0;
	int spriteCols = 0;
	float u, v;
	bool updateNeeded = false;
	irr::scene::IBillboardSceneNode * billboard;
	irr::scene::ISceneNode *node;
	ComponentRenderSceneNode(irr::scene::ISceneNode *node, std::string spritesheet, int spriteRows, int spriteCols);
	~ComponentRenderSceneNode();
};

}  // namespace client

#endif
