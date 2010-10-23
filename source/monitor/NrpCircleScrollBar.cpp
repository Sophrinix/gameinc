#include "StdAfx.h"
#include "NrpCircleScrollBar.h"
#include "nrpScript.h"
#include "NrpRotatableImage.h"
#include "StrConversation.h"

#include <irrlicht.h>

namespace irr
{

namespace gui
{

CNrpCircleScrollBar::CNrpCircleScrollBar(IGUIEnvironment* environment,
								 		 IGUIElement* parent, s32 id, 
										 core::rect<s32> rectangle,
										 bool noclip) : CNrpScrollBar( false, environment, parent, id, rectangle )
{
#ifdef _DEBUG
	setDebugName("CNrpCircleScrollBar");
#endif

	while( Children.getSize() )
		   (*Children.begin())->remove();

	UpButton = NULL;
	DownButton = NULL;

	mouse2rotate_ = false;
	Min = 0;
	Max = 100;
	startAngle_ = 0;
	circleCounter_ = 0;
	aPos = 0;

	rImage_ = new CNrpRotatableImage( core::recti( 0, 0, AbsoluteRect.getWidth(), AbsoluteRect.getHeight() ), environment, -1, this );		
}

CNrpCircleScrollBar::~CNrpCircleScrollBar()
{
}

void CNrpCircleScrollBar::setPos( s32 pos )
{
	aPos = (f32)core::s32_clamp ( pos, Min, Max );

	startAngle_ = pos / 360.f;

	rImage_->SetRotate( core::vector3df( 0, startAngle_, 0 ) );
}

void CNrpCircleScrollBar::updateAbsolutePosition()
{
	IGUIElement::updateAbsolutePosition();

	setPos ( Pos );
}

bool CNrpCircleScrollBar::OnEvent( const SEvent& event )
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
				f32 offset = GetCurrentAngle_( core::position2di( event.MouseInput.X, event.MouseInput.Y) ) - startAngle_;
										
				if( aPos + offset >= Min && aPos + offset < Max )
				{
					startAngle_ += offset;
					aPos += offset;
					rImage_->SetRotate( core::vector3df( 0, 0, rImage_->getRotate().Z - offset ) );

					if( onChangedAction_.size() > 0 )
						nrp::CNrpScript::Instance().CallFunction( onChangedAction_.c_str(), this );
				}
				else
				{
					mouse2rotate_ = false;
				}
			}		
		}
		break;

	}

	return false;
}

f32 CNrpCircleScrollBar::GetCurrentAngle_( const core::position2di& mPos )
{
	f32 angle=startAngle_;
	f32 dst = (f32)mPos.getDistanceFrom(  AbsoluteRect.getCenter() );
	f32 rad = AbsoluteRect.getWidth() / 2.f;

	if( dst > 0.2 * rad && dst < rad )
	{
		angle = atan2f( (f32)(mPos.Y - AbsoluteRect.getCenter().Y),
				        (f32)(mPos.X - AbsoluteRect.getCenter().X) ) * 180 / core::PI + 90;
	}
	
	if( angle < 0 )
		angle = 360 + angle;


	if( angle + circleCounter_ * 360 - startAngle_ < -100 )
		circleCounter_++;
	else if( angle + circleCounter_ * 360 - startAngle_ > 100 )
		circleCounter_--;

	return angle + circleCounter_ * 360;
}

void CNrpCircleScrollBar::setTexture( video::ITexture* texture )
{
	CNrpScrollBar::setTexture( texture );

}

irr::s32 CNrpCircleScrollBar::getPos() const
{
	return (s32)aPos;
}

void CNrpCircleScrollBar::setSliderTexture( video::ITexture* texture )
{
	CNrpScrollBar::setSliderTexture( texture );
	rImage_->SetTexture( texture );
}

void CNrpCircleScrollBar::draw()
{
	if( texture_ )
	{
		core::recti srcRect = core::recti( core::position2di( 0, 0 ), texture_->getSize() );
		Environment->getVideoDriver()->draw2DImage( texture_, AbsoluteRect, srcRect, 0, 0, true );
	}

	rImage_->draw();

	IGUIElement::draw();

#ifdef _DEBUG
	std::string text = nrp::IntToStr( (s32)aPos );
	Environment->getSkin()->getFont()->draw( nrp::StrToWide( text ).c_str(), 
											 core::recti( 0, 0, 100, 25 ) + AbsoluteRect.UpperLeftCorner,
											 video::SColor( 0xff, 0, 0, 0 ) );
#endif
}

}//namesapce gui

}//namesapce irr