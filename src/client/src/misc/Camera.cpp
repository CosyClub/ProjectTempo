// taken from irrlicht source
#include "client/misc/Camera.hpp"
#include "ISceneManager.h"
#include "IVideoDriver.h"

namespace irr
{
namespace scene
{
//! constructor
Camera::Camera(ISceneNode* parent,
                                   ISceneManager* mgr,
                                   s32 id,
                                   const core::vector3df& position,
                                   const core::vector3df& lookat)
    : ICameraSceneNode(parent, mgr, id, position), Target(lookat), UpVector(0.0f, 1.0f, 0.0f),
      ZNear(1.0f), ZFar(3000.0f), InputReceiverEnabled(true), TargetAndRotationAreBound(false)
{
#ifdef _DEBUG
	setDebugName("Camera");
#endif

	// set default projection
	Fovy = core::PI / 2.5f;  // Field of view, in radians.

	const video::IVideoDriver* const d = mgr ? mgr->getVideoDriver() : 0;
	if (d)
		Aspect =
		  (f32)d->getCurrentRenderTargetSize().Width / (f32)d->getCurrentRenderTargetSize().Height;
	else
		Aspect = 4.0f / 3.0f;  // Aspect ratio.

	recalculateProjectionMatrix();
	recalculateViewArea();
}


//! Disables or enables the camera to get key or mouse inputs.
void Camera::setInputReceiverEnabled(bool enabled)
{
	InputReceiverEnabled = enabled;
}


//! Returns if the input receiver of the camera is currently enabled.
bool Camera::isInputReceiverEnabled() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return InputReceiverEnabled;
}


//! Sets the projection matrix of the camera.
/** The core::matrix4 class has some methods
to build a projection matrix. e.g: core::matrix4::buildProjectionMatrixPerspectiveFovLH
\param projection: The new projection matrix of the camera. */
void Camera::setProjectionMatrix(const core::matrix4& projection, bool isOrthogonal)
{
	IsOrthogonal = isOrthogonal;
	ViewArea.getTransform(video::ETS_PROJECTION) = projection;
}


//! Gets the current projection matrix of the camera
//! \return Returns the current projection matrix of the camera.
const core::matrix4& Camera::getProjectionMatrix() const
{
	return ViewArea.getTransform(video::ETS_PROJECTION);
}


//! Gets the current view matrix of the camera
//! \return Returns the current view matrix of the camera.
const core::matrix4& Camera::getViewMatrix() const
{
	return ViewArea.getTransform(video::ETS_VIEW);
}


//! Sets a custom view matrix affector. The matrix passed here, will be
//! multiplied with the view matrix when it gets updated.
//! This allows for custom camera setups like, for example, a reflection camera.
/** \param affector: The affector matrix. */
void Camera::setViewMatrixAffector(const core::matrix4& affector)
{
	Affector = affector;
}


//! Gets the custom view matrix affector.
const core::matrix4& Camera::getViewMatrixAffector() const
{
	return Affector;
}


//! It is possible to send mouse and key events to the camera. Most cameras
//! may ignore this input, but camera scene nodes which are created for
//! example with scene::ISceneManager::addMayaCameraSceneNode or
//! scene::ISceneManager::addFPSCameraSceneNode, may want to get this input
//! for changing their position, look at target or whatever.
bool Camera::OnEvent(const SEvent& event)
{
	if (!InputReceiverEnabled)
		return false;

	// send events to event receiving animators

	ISceneNodeAnimatorList::Iterator ait = Animators.begin();

	for (; ait != Animators.end(); ++ait)
		if ((*ait)->isEventReceiverEnabled() && (*ait)->OnEvent(event))
			return true;

	// if nobody processed the event, return false
	return false;
}


//! sets the look at target of the camera
//! \param pos: Look at target of the camera.
void Camera::setTarget(const core::vector3df& pos)
{
	Target = pos;

	if (TargetAndRotationAreBound) {
		const core::vector3df toTarget = Target - getAbsolutePosition();
		ISceneNode::setRotation(toTarget.getHorizontalAngle());
	}
}


//! Sets the rotation of the node.
/** This only modifies the relative rotation of the node.
If the camera's target and rotation are bound ( @see bindTargetAndRotation() )
then calling this will also change the camera's target to match the rotation.
\param rotation New rotation of the node in degrees. */
void Camera::setRotation(const core::vector3df& rotation)
{
	if (TargetAndRotationAreBound)
		Target = getAbsolutePosition() + rotation.rotationToDirection();

	ISceneNode::setRotation(rotation);
}


//! Gets the current look at target of the camera
//! \return Returns the current look at target of the camera
const core::vector3df& Camera::getTarget() const
{
	return Target;
}


//! sets the up vector of the camera
//! \param pos: New upvector of the camera.
void Camera::setUpVector(const core::vector3df& pos)
{
	UpVector = pos;
}


//! Gets the up vector of the camera.
//! \return Returns the up vector of the camera.
const core::vector3df& Camera::getUpVector() const
{
	return UpVector;
}


f32 Camera::getNearValue() const
{
	return ZNear;
}


f32 Camera::getFarValue() const
{
	return ZFar;
}


f32 Camera::getAspectRatio() const
{
	return Aspect;
}


f32 Camera::getFOV() const
{
	return Fovy;
}


void Camera::setNearValue(f32 f)
{
	ZNear = f;
	recalculateProjectionMatrix();
}


void Camera::setFarValue(f32 f)
{
	ZFar = f;
	recalculateProjectionMatrix();
}


void Camera::setAspectRatio(f32 f)
{
	Aspect = f;
	recalculateProjectionMatrix();
}


void Camera::setFOV(f32 f)
{
	Fovy = f;
	recalculateProjectionMatrix();
}


void Camera::recalculateProjectionMatrix()
{
	ViewArea.getTransform(video::ETS_PROJECTION)
	  .buildProjectionMatrixPerspectiveFovLH(Fovy, Aspect, ZNear, ZFar);
}


//! prerender
void Camera::OnRegisterSceneNode()
{
	if (SceneManager->getActiveCamera() == this)
		SceneManager->registerNodeForRendering(this, ESNRP_CAMERA);

	ISceneNode::OnRegisterSceneNode();
}


//! render
void Camera::render()
{
	core::vector3df pos = getAbsolutePosition();
	core::vector3df tgtv = Parent->getAbsolutePosition() + Target;
	tgtv.normalize();

	// if upvector and vector to the target are the same, we have a
	// problem. so solve this problem:
	core::vector3df up = UpVector;
	up.normalize();

	f32 dp = tgtv.dotProduct(up);

	if (core::equals(core::abs_<f32>(dp), 1.f)) {
		up.X += 0.5f;
	}

	ViewArea.getTransform(video::ETS_VIEW).buildCameraLookAtMatrixLH(pos, Parent->getAbsolutePosition() + Target, up);
	ViewArea.getTransform(video::ETS_VIEW) *= Affector;
	recalculateViewArea();

	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	if (driver) {
		driver->setTransform(video::ETS_PROJECTION, ViewArea.getTransform(video::ETS_PROJECTION));
		driver->setTransform(video::ETS_VIEW, ViewArea.getTransform(video::ETS_VIEW));
	}
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& Camera::getBoundingBox() const
{
	return ViewArea.getBoundingBox();
}


//! returns the view frustum. needed sometimes by bsp or lod render nodes.
const SViewFrustum* Camera::getViewFrustum() const
{
	return &ViewArea;
}


void Camera::recalculateViewArea()
{
	ViewArea.cameraPosition = getAbsolutePosition();

	core::matrix4 m(core::matrix4::EM4CONST_NOTHING);
	m.setbyproduct_nocheck(ViewArea.getTransform(video::ETS_PROJECTION),
	                       ViewArea.getTransform(video::ETS_VIEW));
	ViewArea.setFrom(m);
}


//! Writes attributes of the scene node.
void Camera::serializeAttributes(io::IAttributes* out,
                                           io::SAttributeReadWriteOptions* options) const
{
	ICameraSceneNode::serializeAttributes(out, options);

	out->addVector3d("Target", Target);
	out->addVector3d("UpVector", UpVector);
	out->addFloat("Fovy", Fovy);
	out->addFloat("Aspect", Aspect);
	out->addFloat("ZNear", ZNear);
	out->addFloat("ZFar", ZFar);
	out->addBool("Binding", TargetAndRotationAreBound);
	out->addBool("ReceiveInput", InputReceiverEnabled);
}

//! Reads attributes of the scene node.
void Camera::deserializeAttributes(io::IAttributes* in,
                                             io::SAttributeReadWriteOptions* options)
{
	ICameraSceneNode::deserializeAttributes(in, options);

	Target = in->getAttributeAsVector3d("Target");
	UpVector = in->getAttributeAsVector3d("UpVector");
	Fovy = in->getAttributeAsFloat("Fovy");
	Aspect = in->getAttributeAsFloat("Aspect");
	ZNear = in->getAttributeAsFloat("ZNear");
	ZFar = in->getAttributeAsFloat("ZFar");
	TargetAndRotationAreBound = in->getAttributeAsBool("Binding");
	if (in->findAttribute("ReceiveInput"))
		InputReceiverEnabled = in->getAttributeAsBool("ReceiveInput");

	recalculateProjectionMatrix();
	recalculateViewArea();
}


//! Set the binding between the camera's rotation adn target.
void Camera::bindTargetAndRotation(bool bound)
{
	TargetAndRotationAreBound = bound;
}


//! Gets the binding between the camera's rotation and target.
bool Camera::getTargetAndRotationBinding(void) const
{
	return TargetAndRotationAreBound;
}


//! Creates a clone of this scene node and its children.
ISceneNode* Camera::clone(ISceneNode* newParent, ISceneManager* newManager)
{
	ICameraSceneNode::clone(newParent, newManager);

	if (!newParent)
		newParent = Parent;
	if (!newManager)
		newManager = SceneManager;

	Camera* nb =
	  new Camera(newParent, newManager, ID, RelativeTranslation, Target);

	nb->ISceneNode::cloneMembers(this, newManager);
	nb->ICameraSceneNode::cloneMembers(this);

	nb->Target = Target;
	nb->UpVector = UpVector;
	nb->Fovy = Fovy;
	nb->Aspect = Aspect;
	nb->ZNear = ZNear;
	nb->ZFar = ZFar;
	nb->ViewArea = ViewArea;
	nb->Affector = Affector;
	nb->InputReceiverEnabled = InputReceiverEnabled;
	nb->TargetAndRotationAreBound = TargetAndRotationAreBound;

	if (newParent)
		nb->drop();
	return nb;
}


}  // namespace scene
}  // namespace irr
