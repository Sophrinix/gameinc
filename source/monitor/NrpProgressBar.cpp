#include "StdAfx.h"
#include "NrpProgressBar.h"
#include <irrlicht.h>

namespace irr
{

namespace gui
{

CNrpProgressBar::CNrpProgressBar(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle)
: IGUIImage(environment, parent, id, rectangle), Texture(0), fillTexture_(0), Color(255,255,255,255),
	UseAlphaChannel(false), ScaleImage(false), position_(0)
{
#ifdef _DEBUG
	setDebugName("CGUIImage");
#endif
}


//! destructor
CNrpProgressBar::~CNrpProgressBar()
{
	if (Texture)
		Texture->drop();

	if( fillTexture_ )
		fillTexture_->drop();
}


//! sets an image
void CNrpProgressBar::setImage(video::ITexture* image)
{
	if (image == Texture)
		return;

	if (Texture)
		Texture->drop();

	Texture = image;

	if (Texture)
		Texture->grab();
}

//! sets an image
void CNrpProgressBar::setFillImage(video::ITexture* image)
{
	if (image == fillTexture_)
		return;

	if (fillTexture_)
		fillTexture_->drop();

	fillTexture_ = image;

	if (fillTexture_)
		fillTexture_->grab();
}


//! sets the color of the image
void CNrpProgressBar::setColor(video::SColor color)
{
	Color = color;
}


//! draws the element and its children
void CNrpProgressBar::draw()
{
	if (!IsVisible)
		return;

	IGUISkin* skin = Environment->getSkin();
	video::IVideoDriver* driver = Environment->getVideoDriver();

	core::rect<s32> rect = AbsoluteRect;

	if (Texture)
	{
		const video::SColor Colors[] = {Color,Color,Color,Color};

		driver->draw2DImage(Texture, AbsoluteRect,
				core::rect<s32>(core::position2d<s32>(0,0), core::dimension2di(Texture->getOriginalSize())),
				&AbsoluteClippingRect, Colors, UseAlphaChannel);
	}
	else
	{
		skin->draw2DRectangle(this, skin->getColor(EGDC_3D_DARK_SHADOW), AbsoluteRect, &AbsoluteClippingRect);
	}

	if (fillTexture_)
	{
		const video::SColor Colors[] = {Color,Color,Color,Color};

		core::recti dr = AbsoluteRect;
		dr.LowerRightCorner.X = AbsoluteRect.UpperLeftCorner.X + (AbsoluteRect.getWidth() * position_) / 100;

		core::recti rt = core::recti(core::position2di(0,0), 
									 core::dimension2di( (fillTexture_->getOriginalSize().Width * position_) / 100, 
														 fillTexture_->getOriginalSize().Height ) );

		driver->draw2DImage(fillTexture_, dr, rt, &AbsoluteClippingRect, Colors, UseAlphaChannel  );
	}
	else
	{
		core::recti fillRect;
		fillRect.UpperLeftCorner = AbsoluteRect.UpperLeftCorner;
		fillRect.LowerRightCorner = AbsoluteRect.UpperLeftCorner + core::position2di( (AbsoluteRect.getWidth() * position_) / 100, 
																					  AbsoluteRect.getHeight() ); 

		skin->draw2DRectangle(this, video::SColor( Color.getAlpha(), 0xff, 0, 0 ), fillRect, &AbsoluteClippingRect );
	}

	IGUIFont* font = Environment->getBuiltInFont();
	if( font )
	{
		core::recti cr = AbsoluteClippingRect;
		font->draw( Text, AbsoluteRect, video::SColor( 0xff0000ff ), true, true, &cr );
	}

	IGUIElement::draw();
}


//! sets if the image should use its alpha channel to draw itself
void CNrpProgressBar::setUseAlphaChannel(bool use)
{
	UseAlphaChannel = use;
}


//! sets if the image should use its alpha channel to draw itself
void CNrpProgressBar::setScaleImage(bool scale)
{
	ScaleImage = scale;
}


//! Returns true if the image is scaled to fit, false if not
bool CNrpProgressBar::isImageScaled() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return ScaleImage;
}

//! Returns true if the image is using the alpha channel, false if not
bool CNrpProgressBar::isAlphaChannelUsed() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return UseAlphaChannel;
}

//! Writes attributes of the element.
void CNrpProgressBar::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const
{
	IGUIImage::serializeAttributes(out,options);

	out->addTexture	("Texture", Texture);
	out->addTexture ("FillTexture", fillTexture_ );
	out->addBool	("UseAlphaChannel", UseAlphaChannel);
	out->addColor	("Color", Color);
	out->addBool	("ScaleImage", ScaleImage);

}

//! Reads attributes of the element
void CNrpProgressBar::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0)
{
	IGUIImage::deserializeAttributes(in,options);

	setImage(in->getAttributeAsTexture("Texture"));
	setFillImage(in->getAttributeAsTexture("FillTexture"));
	setUseAlphaChannel(in->getAttributeAsBool("UseAlphaChannel"));
	setColor(in->getAttributeAsColor("Color"));
	setScaleImage(in->getAttributeAsBool("ScaleImage"));
}

void CNrpProgressBar::setPosition( u32 pos )
{
	position_ = pos % 100;
}

int CNrpProgressBar::getPosition() const
{
	return position_;
}
}//namespace gui

}//namespace irr