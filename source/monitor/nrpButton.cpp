#include "StdAfx.h"
#include "nrpButton.h"
#include "ImageGUISkin.h"

namespace irr
{

using namespace video;

namespace gui
{

// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

//! constructor
CNrpButton::CNrpButton( IGUIEnvironment* environment, 
						IGUIElement* parent,
						s32 id, 
						const core::recti& rectangle, 
						bool noclip)
			: IGUIButton( environment, parent, id, rectangle), pressed_(false),
			isPushButton_(false), useAlphaChannel_(false), border_(true),
			clickTime_(0), spriteBank_(0), overrideFont_(0), image_(0), pressedImage_(0), hoveredImage_(NULL)
{
	pressed_ = false;
	setNotClipped(noclip);
	//Type = EGUIET_SKINBUTTON;

	// Initialize the sprites.
	for (u32 i=0; i<EGBS_COUNT; ++i)
		ButtonSprites[i].Index = -1;

	// This element can be tabbed.
	setTabStop(true);
	setTabOrder(-1);
}
//////////////////////////////////////////////////////////////////////////

		//! destructor
CNrpButton::~CNrpButton()
{
	if (overrideFont_)
		overrideFont_->drop();

	if (image_)
		image_->drop();

	if (pressedImage_)
		pressedImage_->drop();

	if( hoveredImage_ )
		hoveredImage_->drop();

	if (spriteBank_)
		spriteBank_->drop();
}
//////////////////////////////////////////////////////////////////////////

		//! Sets if the button should use the skin to draw its border
void CNrpButton::setDrawBorder(bool border)
{
	border_ = border;
}
//////////////////////////////////////////////////////////////////////////

void CNrpButton::setSpriteBank(IGUISpriteBank* sprites)
{
	if (sprites)
		sprites->grab();

	if (spriteBank_)
		spriteBank_->drop();

	spriteBank_ = sprites;
}
//////////////////////////////////////////////////////////////////////////

void CNrpButton::setSprite(EGUI_BUTTON_STATE state, s32 index, video::SColor color, bool loop)
{
	if (spriteBank_)
	{
		ButtonSprites[(u32)state].Index	= index;
		ButtonSprites[(u32)state].Color	= color;
		ButtonSprites[(u32)state].Loop	= loop;
	}
	else
	{
		ButtonSprites[(u32)state].Index = -1;
	}
}
//////////////////////////////////////////////////////////////////////////

//! called if an event happened.
bool CNrpButton::OnEvent(const SEvent& event)
{
	if (!IsEnabled)
		return Parent ? Parent->OnEvent(event) : false;

	switch(event.EventType)
	{
	case EET_KEY_INPUT_EVENT:
		if (event.KeyInput.PressedDown &&
			(event.KeyInput.Key == KEY_RETURN || 
			event.KeyInput.Key == KEY_SPACE))
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

				if (Parent)
				{
					SEvent newEvent;
					newEvent.EventType = EET_GUI_EVENT;
					newEvent.GUIEvent.Caller = this;
					newEvent.GUIEvent.Element = 0;
					newEvent.GUIEvent.EventType = EGET_BUTTON_CLICKED;
					Parent->OnEvent(newEvent);
				}
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
					return ButtonLMouseDown_( event );
		case EMIE_LMOUSE_LEFT_UP:
					return ButtonLMouseUp_( event );
		}
		break;
	default:
		break;
	}

	return Parent ? Parent->OnEvent(event) : false;
}
//////////////////////////////////////////////////////////////////////////

bool CNrpButton::ButtonLMouseUp_( const irr::SEvent& event )
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
	{
		SEvent newEvent;
		newEvent.EventType = EET_GUI_EVENT;
		newEvent.GUIEvent.Caller = this;
		newEvent.GUIEvent.Element = 0;
		newEvent.GUIEvent.EventType = EGET_BUTTON_CLICKED;
		Parent->OnEvent(newEvent);
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////

bool CNrpButton::ButtonLMouseDown_( const irr::SEvent& event )
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
//////////////////////////////////////////////////////////////////////////

//! draws the element and its children
void CNrpButton::draw()
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
	core::recti txsRect;
    ITexture* texImage = NULL;
	s32 btnState = 0;

	core::position2d<s32> pos = AbsoluteRect.getCenter();
	pos.X -= imageRect_.getWidth() / 2;
	pos.Y -= imageRect_.getHeight() / 2;

	if( IsEnabled )
	{
		if( pressed_ )
		{
			texImage = pressedImage_;
			txsRect = pressedImageRect_;
			btnState = EGBS_BUTTON_DOWN;
			if (image_ == pressedImage_ && pressedImageRect_ == imageRect_)
			{
				pos.X += 1;
				pos.Y += 1;
			}
		}
		else
		{
			texImage = image_;
			txsRect = imageRect_;
			btnState = EGBS_BUTTON_UP;
		}
	}
	else
	{
		texImage = NULL;
	}

	if( texImage != NULL )
	{
		driver->draw2DImage( texImage, AbsoluteRect, txsRect, &AbsoluteClippingRect, 0, true  );	

		if( isHovered )
		{
			if( AlphaBlend < 0xff )	AlphaBlend+=core::s32_min( 3, 0xff - AlphaBlend );
		}
		else
		{
			if( AlphaBlend > 5 ) AlphaBlend-=core::s32_min( 3, AlphaBlend - 5 );
		}

		if( AlphaBlend <= 0xff && AlphaBlend >= 5 && hoveredImage_ != NULL )
		{
			video::SColor color( AlphaBlend, 0xff, 0xff, 0xff );
			video::SColor colors[4] = { color, color, color, color };		
			driver->draw2DImage( hoveredImage_, AbsoluteRect, hoveredImageRect_, &AbsoluteClippingRect, colors, true  );	
		}
	}
	else if( border_ )
	{
		if( pressed_ )
			Environment->getSkin()->draw3DButtonPanePressed( this, rect, &AbsoluteClippingRect );
		else
			Environment->getSkin()->draw3DButtonPaneStandard( this, rect, &AbsoluteClippingRect );
	}
	
	
	if (spriteBank_ && ButtonSprites[ btnState ].Index != -1)
	{
		spriteBank_->draw2DSprite(ButtonSprites[ btnState ].Index, spritePos, 
			&AbsoluteClippingRect, ButtonSprites[ btnState ].Color, clickTime_, GetTickCount(), 
			ButtonSprites[ btnState ].Loop, true);
	}

	if (Text.size())
	{
		rect = AbsoluteRect;
     	rect.UpperLeftCorner.Y += 2 * pressed_;

		if (font)
			font->draw(Text.c_str(), rect,
			overrideColorEnabled_ ? overrideColor_ : Environment->getSkin()->getColor(IsEnabled ? EGDC_BUTTON_TEXT : EGDC_GRAY_TEXT), true, true, 
			&AbsoluteClippingRect);
	}

	IGUIElement::draw();
}
//////////////////////////////////////////////////////////////////////////

//! Sets another color for the text.
void CNrpButton::setOverrideColor(video::SColor color)
{
	overrideColor_ = color;
	overrideColorEnabled_ = true;
}
//////////////////////////////////////////////////////////////////////////

//! sets another skin independent font. if this is set to zero, the button uses the font of the skin.
void CNrpButton::setOverrideFont(IGUIFont* font)
{
	if (overrideFont_)
		overrideFont_->drop();

	overrideFont_ = font;

	if (overrideFont_)
		overrideFont_->grab();
}
//////////////////////////////////////////////////////////////////////////

//! Sets an image which should be displayed on the button when it is in normal state. 
void CNrpButton::setImage(video::ITexture* image)
{
	if (image_)
		image_->drop();

	image_ = image;
	if (image)
		imageRect_ = core::rect<s32>(core::position2d<s32>(0,0), image->getOriginalSize());

	if (image_)
		image_->grab();

	//if (!PressedImage)
	//	setPressedImage(image_);
}
//////////////////////////////////////////////////////////////////////////

//! Sets the image which should be displayed on the button when it is in its normal state.
void CNrpButton::setImage(video::ITexture* image, const core::rect<s32>& pos)
{
	if (image_)
		image_->drop();

	image_ = image;
	imageRect_ = pos;

	if (image_)
		image_->grab();

	//if (!PressedImage)
	//	setPressedImage(image_, pos);
}
//////////////////////////////////////////////////////////////////////////

//! Sets an image which should be displayed on the button when it is in pressed state. 
void CNrpButton::setPressedImage(video::ITexture* image)
{
	if (pressedImage_)
		pressedImage_->drop();

	pressedImage_ = image;
	if (image)
		pressedImageRect_ = core::rect<s32>(core::position2d<s32>(0,0), image->getOriginalSize());

	if (pressedImage_)
		pressedImage_->grab();
}
//////////////////////////////////////////////////////////////////////////

//! Sets the image which should be displayed on the button when it is in its pressed state.
void CNrpButton::setPressedImage(video::ITexture* image, const core::rect<s32>& pos)
{
	if (pressedImage_)
		pressedImage_->drop();

	pressedImage_ = image;
	pressedImageRect_ = pos;

	if (pressedImage_)
		pressedImage_->grab();
}
//////////////////////////////////////////////////////////////////////////

//! Sets if the button should behave like a push button. Which means it
//! can be in two states: Normal or Pressed. With a click on the button,
//! the user can change the state of the button.
void CNrpButton::setIsPushButton(bool isPushButton)
{
	isPushButton_ = isPushButton;
}
//////////////////////////////////////////////////////////////////////////

//! Returns if the button is currently pressed
bool CNrpButton::isPressed() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return pressed_;
}
//////////////////////////////////////////////////////////////////////////

//! Sets the pressed state of the button if this is a pushbutton
void CNrpButton::setPressed(bool pressed)
{
	if (pressed_ != pressed)
	{
		clickTime_ = GetTickCount();
		pressed_ = pressed;
	}
}
//////////////////////////////////////////////////////////////////////////

//! Returns whether the button is a push button
bool CNrpButton::isPushButton() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return isPushButton_;
}
//////////////////////////////////////////////////////////////////////////

//! Sets if the alpha channel should be used for drawing images on the button (default is false)
void CNrpButton::setUseAlphaChannel(bool useAlphaChannel)
{
	useAlphaChannel_ = useAlphaChannel;
}
//////////////////////////////////////////////////////////////////////////

//! Returns if the alpha channel should be used for drawing images on the button
bool CNrpButton::isAlphaChannelUsed() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return useAlphaChannel_;
}
//////////////////////////////////////////////////////////////////////////

bool CNrpButton::isDrawingBorder() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return border_;
}
//////////////////////////////////////////////////////////////////////////

//! Writes attributes of the element.
void CNrpButton::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const
{
	IGUIButton::serializeAttributes(out,options);

	out->addBool	("PushButton",		isPushButton_ );
	if (isPushButton_)
		out->addBool("Pressed",		pressed_);

	out->addTexture ("Image",		image_);
	out->addRect	("ImageRect",		imageRect_);
	out->addTexture	("PressedImage",	pressedImage_);
	out->addRect	("PressedImageRect",	pressedImageRect_);

	out->addBool	("Border",		border_);
	out->addBool	("UseAlphaChannel",	useAlphaChannel_);

	//   out->addString  ("OverrideFont",	OverrideFont);
}
//////////////////////////////////////////////////////////////////////////

//! Reads attributes of the element
void CNrpButton::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0)
{
	IGUIButton::deserializeAttributes(in,options);

	isPushButton_	= in->getAttributeAsBool("PushButton");
	pressed_		= isPushButton_ ? in->getAttributeAsBool("Pressed") : false;

	core::rect<s32> rec = in->getAttributeAsRect("ImageRect");
	if (rec.isValid())
		setImage( in->getAttributeAsTexture("Image"), rec);
	else
		setImage( in->getAttributeAsTexture("Image") );

	rec = in->getAttributeAsRect("PressedImageRect");
	if (rec.isValid())
		setPressedImage( in->getAttributeAsTexture("PressedImage"), rec);
	else
		setPressedImage( in->getAttributeAsTexture("PressedImage") );

	setDrawBorder(in->getAttributeAsBool("Border"));
	useAlphaChannel_ = in->getAttributeAsBool("UseAlphaChannel");

	//   setOverrideFont(in->getAttributeAsString("OverrideFont"));

	updateAbsolutePosition();
}
//////////////////////////////////////////////////////////////////////////

void CNrpButton::setHoveredImage( irr::video::ITexture* image )
{
	if (hoveredImage_)
		hoveredImage_->drop();

	hoveredImage_ = image;
	if (image)
		hoveredImageRect_ = core::rect<s32>(core::position2d<s32>(0,0), image->getOriginalSize());

	if (hoveredImage_)
		hoveredImage_->grab();
}
//////////////////////////////////////////////////////////////////////////

void CNrpButton::setHoveredImage( irr::video::ITexture* image, const irr::core::rect< irr::s32 >& pos )
{
	if (hoveredImage_)
		hoveredImage_->drop();

	hoveredImage_ = image;
	hoveredImageRect_ = pos;

	if (hoveredImage_)
		hoveredImage_->grab();
}

void CNrpButton::setScaleImage( bool scaleImage/* =true */ )
{
	//IGUIButton::setScaleImage( scaleImage );
}

bool CNrpButton::isScalingImage() const
{
	//return IGUIButton::isScalingImage();
	return false;
}

}//namespace gui

}//namespace irr