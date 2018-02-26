//added by JAMES
//original code by calimero, modified slightly by me, JAMES
// calimero 14 august 2004 this is version 1.0 of the node
// I think this version is usable
#include <client/misc/CAxialBillboardSceneNode.h>
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "ICameraSceneNode.h"

namespace irr
{
namespace scene
{

CAxialBillboardSceneNode::CAxialBillboardSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id,
core::vector3df position,core::vector3df orientation, f32 width)
: IAxialBillboardSceneNode(parent, mgr, id, position)
{
AutomaticCullingState= false;

this->orientation = orientation;
this->width = width/2;

indices[0] = 0;
indices[1] = 1;
indices[2] = 2;
indices[3] = 0;
indices[4] = 2;
indices[5] = 3;

// by default the material is set to be used like a laser ray
// but feel free to modify these settings
Material.Lighting = false;
Material.BackfaceCulling = false;
this->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);


video::SColor color = video::SColor(255,255,255,255);
vertices[0].TCoords.set(0.0f, 0.0f);
vertices[0].Color = color;
vertices[1].TCoords.set(1.0f, 0.0f);
vertices[1].Color = color;
vertices[2].TCoords.set(1.0f, 1.0f);
vertices[2].Color = color;
vertices[3].TCoords.set(0.0f, 1.0f);
vertices[3].Color = color;

}

CAxialBillboardSceneNode::~CAxialBillboardSceneNode()
{

}

void CAxialBillboardSceneNode::OnPreRender()
{
   if (IsVisible)
   SceneManager->registerNodeForRendering(this);

   //ISceneNode::OnPreRender();
}


void CAxialBillboardSceneNode::render()
{
   // ok in this function is the interesting stuff
   // jfh "calimero" 11august2004 nice france
   // the axial billboard has two end, the first is the position of the node
   // the other is given by orientation in LOCAL coordinates.
   // for node attached to rootscenenode it behaves like the previous version
   // in this version orientation is expressed in local coordinates of the node
   // I think it's more interesting because it gives a more standard behaviour
   // if the node is attached to a parent different from rootscenenode
   // you can also attach animator to the node (like rotation)
    // and I hope it will behave like you expect
    //
    // known problem : node disappears if position gets too near of the camera

   core::vector3df orientationglobal; // to store the end of the billboard in world coordinates
   core::matrix4 mat; // to get an identity matrix for final render
   core::matrix4 matnode = getAbsoluteTransformation(); // get the transformation of the node

    ISceneNode* camera = SceneManager->getActiveCamera();
     video::IVideoDriver* driver = SceneManager->getVideoDriver();


   core::vector3df pos = getAbsolutePosition();
   core::vector3df campos = camera->getAbsolutePosition();
   matnode.transformVect(orientation,orientationglobal);
   core::vector3df vtemp = orientationglobal-pos;
    core::vector3df perp = vtemp.crossProduct(campos - pos);

    // perhaps here I should test if perp is null before normalize it ?
    // it doesn't seem to be necessary --> to investigate
    //perp.normalize();
    //perp *= largeur;
    perp.setLength(width);

    // here I update the coordinates of the billboard in GLOBAL space
    vertices[0].Pos = pos - perp;
   vertices[1].Pos = pos + perp;
   vertices[2].Pos = orientationglobal + perp  ;
   vertices[3].Pos = orientationglobal - perp;

   // update normals of the node
   core::vector3df norm = (perp).crossProduct(vtemp);
   norm.normalize();
   vertices[0].Normal = norm;
   vertices[1].Normal = norm;
   vertices[2].Normal = norm;
   vertices[3].Normal = norm;

   driver->setMaterial(Material);

   // doesn't seem to be necessary probably the default constructor
   // build the identity matrix
   //mat.makeIdentity();
     driver->setTransform(video::ETS_WORLD, mat);
   // finally I draw it
   driver->drawIndexedTriangleList(vertices, 4, indices, 2);

}


const core::aabbox3d<f32>& CAxialBillboardSceneNode::getBoundingBox() const
{
return Box;
}

s32 CAxialBillboardSceneNode::getMaterialCount()
{
return 1;
}

video::SMaterial& CAxialBillboardSceneNode::getMaterial(s32 i)
{
return Material;
}

//! sets the width of the billboard
void CAxialBillboardSceneNode::setWidth(const f32 width)
{
   this->width = width/2;
}

//! gets the width of the billboard
f32 CAxialBillboardSceneNode::getWidth()
{
   return (width*2);
}

//! sets the local orientation of the billboard
void CAxialBillboardSceneNode::setOrientation(const core::vector3df& orientation)
{
   this->orientation = orientation;
}

//! gets the local orientation of the billboard
const core::vector3df& CAxialBillboardSceneNode::getOrientation()
{
   return orientation;
}

} // end namespace scene
} // end namespace irr
//end added by JAMES
