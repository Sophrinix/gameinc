#pragma once

#include <irrlicht.h>
#include <vector>

const size_t MOUSE_BTN_NUMBER=3;

namespace irr
{

namespace scene
{

const int ESNAT_CAMERA_RTS = ESNAT_UNKNOWN + 2;

class CNrpCameraAnimator : public ISceneNodeAnimatorCameraMaya 
{ 
public: 
	
	CNrpCameraAnimator( gui::ICursorControl* cursor, 
						f32 angle, 
						f32 rotateSpeed = -1500.0f, 
						f32 zoomSpeed = 200.0f, 
						f32 translationSpeed = 1500.0f );

	virtual void animateNode(ISceneNode* node, u32 timeMs);

	//! Event receiver
	virtual bool OnEvent(const SEvent& event);

	//! Returns the speed of movement in units per millisecond
	virtual f32 getMoveSpeed() const;

	//! Sets the speed of movement in units per millisecond
	virtual void setMoveSpeed(f32 moveSpeed);

	//! Returns the rotation speed
	virtual f32 getRotateSpeed() const;

	//! Set the rotation speed
	virtual void setRotateSpeed(f32 rotateSpeed);

	//! Returns the zoom speed
	virtual f32 getZoomSpeed() const;

	//! Set the zoom speed
	virtual void setZoomSpeed(f32 zoomSpeed);

	virtual f32 getZoom() { return currentZoom_; }
	virtual void setZoom( f32 amount );

	virtual void AddTerrainToResponse( scene::ITerrainSceneNode* terrain )
	{
		terrains_.push_back( terrain );
	}

	virtual void setRotate( core::vector3df amount ) { rotX_ = amount.X; }
	virtual core::vector3df getRotate() { return core::vector3df( rotX_, rotY_, 0 ); }

	//! This animator will receive events when attached to the active camera
	virtual bool isEventReceiverEnabled() const
	{
		return true;
	}

	//! Returns type of the scene node
	virtual ESCENE_NODE_ANIMATOR_TYPE getType() const 
	{
		return (ESCENE_NODE_ANIMATOR_TYPE)ESNAT_CAMERA_RTS;
	}

	//! Creates a clone of this animator.
	/** Please note that you will have to drop
	(IReferenceCounted::drop()) the returned pointer after calling
	this. */
	virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0);
protected: 
	//Properties 
	void allKeysUp();
	void animate();
	bool isMouseKeyDown(s32 key);
	bool mouseKeys_[3];

	gui::ICursorControl *cursorControl_;
	core::vector3df pos_;
	bool zooming_;
	bool rotating_;
	bool moving_;
	bool translating_;
	f32 zoomSpeed_;
	f32 rotateSpeed_;
	f32 translateSpeed_;
	f32 currentAngle_, 
		needAngle_,
		stepAngle_;
	core::position2df rotateStart_;
	core::position2df zoomStart_;
	core::position2df translateStart_;
	f32 currentZoom_, 
		needZoom_,
		stepZoom_;
	f32 rotX_, rotY_;
	core::vector3df oldTarget_;
	scene::ICameraSceneNode* oldCamera_;

	core::list< scene::ITerrainSceneNode* > terrains_;

	core::position2df mousePos_;

	core::vector3df CorrectCameraPositionWithTerrains_( const core::vector3df& position );

}; 


}//namespace scene

}//namespace irr