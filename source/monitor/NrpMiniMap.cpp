#include "StdAfx.h"
#include "NrpMiniMap.h"
#include "nrpScript.h"
#include <IGUIEnvironment.h>
#include <ISceneNode.h>

namespace irr
{

namespace gui
{

CNrpMiniMap::~CNrpMiniMap(void)
{
}

void CNrpMiniMap::draw()
{
	if( animNode_ )
		SetRotate( core::vector3df( 0, 0, animNode_->getRotation().X ) );

	CNrpRotatableImage::draw();
}

CNrpMiniMap::CNrpMiniMap( const core::recti& rectange, IGUIEnvironment* env, s32 id, scene::ISceneNode* pr_node/*=NULL */ )
: CNrpRotatableImage( rectange, env, id, env->getRootGUIElement() )
{
	animNode_ = pr_node;
	scale_ = 1.f;
	startSize_.Width = rectange.getWidth();
	startSize_.Height = rectange.getHeight();
	mouse2rotate_ = false;
	offsetAngle_ = 0;
}

bool CNrpMiniMap::OnEvent( const SEvent& event )
{
	switch( event.EventType )
	{
	case EET_GUI_EVENT:
		if(  event.GUIEvent.EventType == EGET_ELEMENT_LEFT )
		{
			mouse2rotate_ = false;
			return true;
		}
	break;

	case EET_MOUSE_INPUT_EVENT:
		if( event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
		{
			mouse2rotate_ = true;	
			startAngle_ = GetCurrentAngle_( core::position2di( event.MouseInput.X, event.MouseInput.Y) );
			return true;
		}

		if( event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
		{
			mouse2rotate_ = false;

			return true;
		}

		if( event.MouseInput.Event == EMIE_MOUSE_MOVED )
		{
			if( mouse2rotate_ )
			{
				offsetAngle_ = GetCurrentAngle_( core::position2di( event.MouseInput.X, event.MouseInput.Y) ) - startAngle_;

				startAngle_ += offsetAngle_;

				if( onRotateAction_.size() > 0 )
					nrp::CNrpScript::Instance().CallFunction( onRotateAction_.c_str(), this );
			}			
		}
	break;

	}

	return IGUIElement::OnEvent( event );
}

f32 CNrpMiniMap::GetCurrentAngle_( const core::position2di& mPos )
{
	f32 angle=startAngle_;
	s32 dst = mPos.getDistanceFrom(  AbsoluteRect.getCenter() );

	if( dst > 0.7 * startSize_.Width / 2 && dst < startSize_.Width / 2 )
	{
		angle = atan2f( (f32)(mPos.Y - AbsoluteRect.getCenter().Y),
			(f32)(AbsoluteRect.getCenter().X - mPos.X) ) * 180 / core::PI + 90.f;	
	}

	return angle;
}

void CNrpMiniMap::SetupQuadMesh_( scene::SMeshBuffer& mesh, const f32 width )
{
	scale_ = width;
	CNrpRotatableImage::SetupQuadMesh( mesh, scale_ );
}
}//namespace gui

}//namespace irr