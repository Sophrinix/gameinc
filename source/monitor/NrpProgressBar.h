#pragma once
#include <irrlicht.h>

namespace irr
{

namespace gui
{

//! constructor
class CNrpProgressBar : public IGUIImage
{
public:

	//! constructor
	CNrpProgressBar(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle);

	//! destructor
	virtual ~CNrpProgressBar();

	//! sets an image
	virtual void setImage(video::ITexture* image);
	virtual void setFillImage( video::ITexture* image );
	virtual void setPosition( u32 pos );
	virtual int	 getPosition() const;
	//virtual void setPostfixText( const wchar_t* text );
	//virtual void setPrefixText( const wchar_t* text );
	//virtual void setTextVisible( bool visible );

	//! sets the color of the image
	virtual void setColor(video::SColor color);

	//! sets if the image should scale to fit the element
	virtual void setScaleImage(bool scale);

	//! draws the element and its children
	virtual void draw();

	//! sets if the image should use its alpha channel to draw itself
	virtual void setUseAlphaChannel(bool use);

	//! Returns true if the image is scaled to fit, false if not
	virtual bool isImageScaled() const;

	//! Returns true if the image is using the alpha channel, false if not
	virtual bool isAlphaChannelUsed() const;

	//! Writes attributes of the element.
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const;

	//! Reads attributes of the element
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

private:
	video::ITexture* Texture;
	video::ITexture* fillTexture_;
	video::SColor Color;
	bool UseAlphaChannel;
	bool ScaleImage;
	u32 position_;
};

}//namespace gui

}//namespace irr