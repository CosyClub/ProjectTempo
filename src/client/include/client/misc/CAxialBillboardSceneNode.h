//added by JAMES
#ifndef __C_AXIALBB_NODE_H_INCLUDED__
#define __C_AXIALBB_NODE_H_INCLUDED__

// jfh "calimero" custom node
// this is an axial billboard node
//original code by  calimero,slightly modified by me, JAMES
// calimero 14 august 2004 this is version 1.0 of the node
// I think this version is usable

#include <client/misc/IAxialBillboardSceneNode.h>
#include "S3DVertex.h"


namespace irr
{
namespace scene
{

class CAxialBillboardSceneNode : public scene::IAxialBillboardSceneNode
{
public:
CAxialBillboardSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id,
core::vector3df position,core::vector3df orientation, f32 width);
virtual ~CAxialBillboardSceneNode();
virtual void OnPreRender();
virtual void render();
virtual const core::aabbox3d<f32>& getBoundingBox() const;
virtual s32 getMaterialCount();
virtual video::SMaterial& getMaterial(s32 i);
virtual void setWidth(const f32 width);
virtual f32 getWidth();
virtual void setOrientation(const core::vector3df& orientation);
virtual const core::vector3df& getOrientation();

private:
core::aabbox3d<f32> Box;
video::S3DVertex vertices[4];
u16 indices[6];
video::SMaterial Material;
core::vector3df orientation;
f32 width;
};

} // end namespace scene
} // end namespace irr

#endif
//end added by JAMES
