#pragma once

#include <irrlicht.h>

namespace irr
{

namespace gui
{

class CNrpGuiLigthing : public irr::gui::IGUIElement
{
public:

	//! constructor
	CNrpGuiLigthing( IGUIEnvironment* guienv, 
					 gui::IGUIElement* firstElement,
					 gui::IGUIElement* secondElement,
					 s32 id, 
					 video::ITexture* texture,
					 s32 showTime );

	//! destructor
	virtual ~CNrpGuiLigthing();

	//! draws the element and its children
	virtual void draw();

	//! Sets an image which should be displayed on the button when it is in pressed state. 
	virtual void setImage(video::ITexture* image);
protected:

	video::ITexture* image_;
	gui::IGUIElement* _elm1;
	gui::IGUIElement* _elm2;

	core::array< core::position2di > _positions;

	s32 _lastTimeUpdate;
	s32 _length;
	s32 _showTime, _startTimeShow;
	video::SColor _opacity;
	void _UpdateImagePositions();
	void _CreateNewPositions(  s32 radius, s32 step );
};

}//namespace gui

}//namspace irr
