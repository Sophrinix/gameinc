#ifndef _INCLUDE_NRPGUILINK_
#define _INCLUDE_NRPGUILINK_

#include <irrlicht.h>
#include "LuaFunctionality.h"

namespace irr
{

namespace video
{
	class ITexture;
}

namespace gui
{

class CNrpGuiLink : public irr::gui::IGUIButton, public ILuaFunctionality
{
public:

	//! constructor
	CNrpGuiLink( IGUIEnvironment* guienv, 
				 gui::IGUIElement* parent,
				 s32 id, const core::recti& rectangle, bool noclip=false);

	//! destructor
	virtual ~CNrpGuiLink();

	//! called if an event happened.
	virtual bool OnEvent(const SEvent& event);

	//! draws the element and its children
	virtual void draw();

	//! sets another skin independent font. if this is set to zero, the button uses the font of the skin.
	virtual void setOverrideFont( gui::IGUIFont* font=0);

	//! Writes attributes of the element.
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const;

	//! Reads attributes of the element
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);
	virtual void setOverrideColor(video::SColor color);

	virtual inline void enableOverrideColor( bool bvalue ) { overrideColorEnabled_ = bvalue; }

	virtual void setOnClickAction( int funcRef );

protected:

		//! Sets an image which should be displayed on the button when it is in normal state. 
	virtual void setImage( video::ITexture* image );
	virtual void setImage( video::ITexture* image, const core::recti& pos);

	//! Sets an image which should be displayed on the button when it is in hover state. 
	virtual void setHoveredImage( video::ITexture* image );
	virtual void setHoveredImage( video::ITexture* image, const core::recti& pos );

	//! Sets an image which should be displayed on the button when it is in pressed state. 
	virtual void setPressedImage(video::ITexture* image);
	virtual void setPressedImage(video::ITexture* image, const core::recti& pos);

	//! Sets the sprite bank used by the button
	virtual void setSpriteBank(gui::IGUISpriteBank* bank);

	//! Sets the animated sprite for a specific button state
	/** \param index: Number of the sprite within the sprite bank, use -1 for no sprite
	\param state: State of the button to set the sprite for
	\param index: The sprite number from the current sprite bank
	\param color: The color of the sprite
	*/
	virtual void setSprite(gui::EGUI_BUTTON_STATE state, s32 index, 
		video::SColor color=video::SColor(255,255,255,255), 
		bool loop=false);

	//! Sets if the button should behave like a push button. Which means it
	//! can be in two states: Normal or Pressed. With a click on the button,
	//! the user can change the state of the button.
	virtual void setIsPushButton(bool isPushButton);

	//! Returns if the button is currently pressed
	virtual bool isPressed() const;

	//! Sets the pressed state of the button if this is a pushbutton
	virtual void setPressed(bool pressed);

	//! Sets if the button should use the skin to draw its border
	virtual void setDrawBorder(bool border);

	//! Sets if the alpha channel should be used for drawing images on the button (default is false)
	virtual void setUseAlphaChannel(bool useAlphaChannel);

	//! Returns if the alpha channel should be used for drawing images on the button
	virtual bool isAlphaChannelUsed() const;

	//! Returns if the button face and border are being drawn
	virtual bool isDrawingBorder() const;

	//! Returns whether the button is a push button
	virtual bool isPushButton() const;

	virtual void setScaleImage(bool scaleImage/* =true */ );	

	virtual bool isScalingImage() const;

	virtual bool isPointInside(const core::position2d<s32>& point) const;

	virtual video::ITexture* getImage() { return NULL; }

	bool pressed_;
	bool isPushButton_;
	bool useAlphaChannel_;

	bool border_;

	u32 clickTime_;

	gui::IGUIFont* overrideFont_;

	irr::video::SColor overrideColor_;
	bool overrideColorEnabled_;

	bool _ButtonLMouseDown( const irr::SEvent& event );
	bool _ButtonMouseUp( const irr::SEvent& event );
	void _SwapImage( video::ITexture*& dest, video::ITexture* source, core::recti& dstRect, const core::recti& srcRect );
	void _CreateMask(video::ITexture* image);
	void _ClickButton();
};

}//namespace gui

}//namspace irr

#endif