//added by JAMES
#ifndef __I_AXIALBB_NODE_H_INCLUDED__
#define __I_AXIALBB_NODE_H_INCLUDED__

// jfh "calimero" custom node
// this is an axial billboard node
//original code by  calimero,slightly modified by me, JAMES
// calimero 14 august 2004 this is version 1.0 of the node
// I think this version is usable

#include "ISceneNode.h"


namespace irr
{
namespace scene
{

class IAxialBillboardSceneNode : public ISceneNode
{
public:
IAxialBillboardSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id,
const core::vector3df& position = core::vector3df(0,0,0) )
: ISceneNode(parent, mgr, id, position) {}

virtual void render()=0;
const core::aabbox3d<f32>& getBoundingBox() const=0;
virtual s32 getMaterialCount()=0;
virtual video::SMaterial& getMaterial(s32 i)=0;
virtual void setWidth(const f32 width)=0;
virtual f32 getWidth()=0;
virtual void setOrientation(const core::vector3df& orientation)=0;
virtual const core::vector3df& getOrientation()=0;

};

} // end namespace scene
} // end namespace irr

#endif
//end added by JAMES
