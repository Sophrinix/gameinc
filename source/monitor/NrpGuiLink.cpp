#include "StdAfx.h"
#include "NrpGuiLink.h"

namespace irr
{

using namespace video;

namespace gui
{
// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

//! constructor
CNrpGuiLink::CNrpGuiLink( IGUIEnvironment* environment, 
	IGUIElement* parent,
	s32 id, 
	const core::recti& rectangle, 
	bool noclip)
	: IGUIButton( environment, parent, id, rectangle), pressed_(false),
	isPushButton_(false), useAlphaChannel_(false), border_(true),
	clickTime_(0), overrideFont_(0)
{
	pressed_ = false;
	setNotClipped(noclip);
	//Type = EGUIET_SKINBUTTON;

	// This element can be tabbed.
	setTabStop(true);
	setTabOrder(-1);
}

//! destructor
CNrpGuiLink::~CNrpGuiLink()
{
	if (overrideFont_)
		overrideFont_->drop();
}


//! Sets if the button should use the skin to draw its border
void CNrpGuiLink::setDrawBorder(bool border)
{
	border_ = border;
}

void CNrpGuiLink::setSpriteBank(IGUISpriteBank* sprites)
{
}

void CNrpGuiLink::setSprite(EGUI_BUTTON_STATE state, s32 index, video::SColor color, bool loop)
{
}

//! called if an event happened.
bool CNrpGuiLink::OnEvent(const SEvent& event)
{
	if (!IsEnabled)
		return Parent ? Parent->OnEvent(event) : false;

	switch(event.EventType)
	{
	case EET_KEY_INPUT_EVENT:
		if (event.KeyInput.PressedDown &&
			(event.KeyInput.Key == KEY_RETURN || event.KeyInput.Key == KEY_SPACE))
		{
			if (!isPushButton_)
				setPressed(true);
			else
				setPressed(!pressed_);

			return true;
		}
		if (pressed_ && !isPushButton_ && event.KeyInput.PressedDown && event.KeyInput.Key == KEY_ESCAPE)
		{
			setPressed(false);
			return true;
		}
		else
			if (!event.KeyInput.PressedDown && pressed_ &&
				(event.KeyInput.Key == KEY_RETURN || 
				event.KeyInput.Key == KEY_SPACE))
			{
				//Environment->removeFocus(this);

				if (!isPushButton_)
					setPressed(false);

				_ClickButton();
				return true;
			}
			break;
	case EET_GUI_EVENT:
		if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST)
		{
			if (event.GUIEvent.Caller == this && !isPushButton_)
				setPressed(false);
		}
		break;
	case EET_MOUSE_INPUT_EVENT:
		switch( event.MouseInput.Event  )
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			return _ButtonLMouseDown( event );
		case EMIE_LMOUSE_LEFT_UP:
			return _ButtonMouseUp( event );
		}
		break;
	default:
		break;
	}

	return Parent ? Parent->OnEvent(event) : false;
}

void CNrpGuiLink::_ClickButton()
{
	if (Parent)
	{
		SEvent newEvent;
		newEvent.EventType = EET_GUI_EVENT;
		newEvent.GUIEvent.Caller = this;
		newEvent.GUIEvent.Element = 0;
		newEvent.GUIEvent.EventType = EGET_BUTTON_CLICKED;
		Parent->OnEvent(newEvent);
	}

	PCall( EGET_BUTTON_CLICKED, (void*)this );
}

bool CNrpGuiLink::isPointInside(const core::position2di& point) const
{
	bool isMyPoint = AbsoluteClippingRect.isPointInside(point);

	return isMyPoint;
}

bool CNrpGuiLink::_ButtonMouseUp( const irr::SEvent& event )
{
	bool wasPressed = pressed_;

	if ( !AbsoluteClippingRect.isPointInside( core::position2d<s32>(event.MouseInput.X, event.MouseInput.Y ) ) )
	{
		if (!isPushButton_)
			setPressed(false);
		return true;
	}

	if (!isPushButton_)
		setPressed(false);
	else
	{
		setPressed(!pressed_);
	}

	if ((!isPushButton_ && wasPressed && Parent) ||
		(isPushButton_ && wasPressed != pressed_))
		_ClickButton();

	return true;
}

bool CNrpGuiLink::_ButtonLMouseDown( const irr::SEvent& event )
{
	if (Environment->hasFocus(this) &&
		!AbsoluteClippingRect.isPointInside(core::position2d<s32>(event.MouseInput.X, event.MouseInput.Y)))
	{
		Environment->removeFocus(this);
		return false;
	}

	if (!isPushButton_)
		setPressed(true);

	Environment->setFocus(this);
	return true;
}

//! draws the element and its children
void CNrpGuiLink::draw()
{
	if (!IsVisible)
		return;

	video::IVideoDriver* driver = Environment->getVideoDriver();
	IGUIFont* font = overrideFont_ != NULL ? overrideFont_ : Environment->getSkin()->getFont(EGDF_BUTTON);
	core::rect<s32> rect = AbsoluteRect;
	bool isHovered = Environment->isHovered( this );

	// todo:	move sprite up and text down if the pressed state has a sprite
	//			draw sprites for focused and mouse-over 
	core::position2di spritePos = AbsoluteRect.getCenter();
	s32 btnState = 0;

	core::position2d<s32> pos = AbsoluteRect.getCenter();
	int fps = (100 / driver->getFPS() + 1) * 2;

	if( IsEnabled )
	{
		if( pressed_ )
		{
			btnState = EGBS_BUTTON_DOWN;
		}
		else
		{
			btnState = EGBS_BUTTON_UP;
		}
	}
	else
	{
	}

	if (Text.size())
	{
		rect = AbsoluteRect;

		int color = overrideColorEnabled_ ? overrideColor_.color : 0xff00ff00;

		if( IsEnabled )
			color &= ( (Environment->isHovered( this ) || pressed_) ? 0xffffffff : 0x80ffffff );
		else
			color &= 0xff707070;
		
		if (font)
			font->draw(Text.c_str(), rect,
			video::SColor( color ), true, true, 
			&AbsoluteClippingRect);
	}

	IGUIElement::draw();
}

//! Sets another color for the text.
void CNrpGuiLink::setOverrideColor(video::SColor color)
{
	overrideColor_ = color;
	overrideColorEnabled_ = true;
}

//! sets another skin independent font. if this is set to zero, the button uses the font of the skin.
void CNrpGuiLink::setOverrideFont(IGUIFont* font)
{
	if (overrideFont_)
		overrideFont_->drop();

	overrideFont_ = font;

	if (overrideFont_)
		overrideFont_->grab();
}

//! Sets an image which should be displayed on the button when it is in normal state. 
void CNrpGuiLink::setImage(video::ITexture* image)
{
}

void CNrpGuiLink::setImage(video::ITexture* image, const core::rect<s32>& pos)
{
}

void CNrpGuiLink::setPressedImage(video::ITexture* image)
{
}

void CNrpGuiLink::setPressedImage(video::ITexture* image, const core::rect<s32>& pos)
{
}

//! Sets if the button should behave like a push button. Which means it
//! can be in two states: Normal or Pressed. With a click on the button,
//! the user can change the state of the button.
void CNrpGuiLink::setIsPushButton(bool isPushButton)
{
	isPushButton_ = isPushButton;
}

//! Returns if the button is currently pressed
bool CNrpGuiLink::isPressed() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return pressed_;
}

//! Sets the pressed state of the button if this is a pushbutton
void CNrpGuiLink::setPressed(bool pressed)
{
	if (pressed_ != pressed)
	{
		clickTime_ = GetTickCount();
		pressed_ = pressed;
	}
}

//! Returns whether the button is a push button
bool CNrpGuiLink::isPushButton() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return isPushButton_;
}

//! Sets if the alpha channel should be used for drawing images on the button (default is false)
void CNrpGuiLink::setUseAlphaChannel(bool useAlphaChannel)
{
	useAlphaChannel_ = useAlphaChannel;
}

//! Returns if the alpha channel should be used for drawing images on the button
bool CNrpGuiLink::isAlphaChannelUsed() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return useAlphaChannel_;
}

bool CNrpGuiLink::isDrawingBorder() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return border_;
}

//! Writes attributes of the element.
void CNrpGuiLink::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const
{
	IGUIButton::serializeAttributes(out,options);

	out->addBool	("PushButton",		isPushButton_ );
	if (isPushButton_)
		out->addBool("Pressed",		pressed_);

	out->addBool	("Border",		border_);
	out->addBool	("UseAlphaChannel",	useAlphaChannel_);

	//   out->addString  ("OverrideFont",	OverrideFont);
}

//! Reads attributes of the element
void CNrpGuiLink::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0)
{
	IGUIButton::deserializeAttributes(in,options);

	isPushButton_	= in->getAttributeAsBool("PushButton");
	pressed_		= isPushButton_ ? in->getAttributeAsBool("Pressed") : false;

	setDrawBorder(in->getAttributeAsBool("Border"));
	useAlphaChannel_ = in->getAttributeAsBool("UseAlphaChannel");

	//   setOverrideFont(in->getAttributeAsString("OverrideFont"));

	updateAbsolutePosition();
}

void CNrpGuiLink::setHoveredImage( irr::video::ITexture* image )
{
}

void CNrpGuiLink::setHoveredImage( irr::video::ITexture* image, const irr::core::rect< irr::s32 >& pos )
{
}

void CNrpGuiLink::setScaleImage( bool scaleImage/* =true */ )
{
}

bool CNrpGuiLink::isScalingImage() const
{
	//return IGUIButton::isScalingImage();
	return false;
}

void CNrpGuiLink::setOnClickAction( int funcRef )
{
	Bind( EGET_BUTTON_CLICKED, funcRef );
}

}//namespace gui

}//namespace irr