#pragma once

#pragma once
#include "NrpScrollBar.h"
namespace irr
{

namespace gui
{

class CNrpRotatableImage;

class CNrpCircleScrollBar : public CNrpScrollBar
{
public:

	//! constructor
	CNrpCircleScrollBar( IGUIEnvironment* environment,
						 IGUIElement* parent, s32 id, 
						 core::rect<s32> rectangle,
						 bool noclip=false);

	//! destructor
	virtual ~CNrpCircleScrollBar();

	//! called if an event happened.
	virtual bool OnEvent(const SEvent& event);

	//! draws the element and its children
	virtual void draw();

	//! sets the position of the scrollbar
	virtual void setPos(s32 pos);

	//! gets the current position of the scrollbar
	virtual s32 getPos() const;

	//! sets the texture which draw
	virtual void setTexture( video::ITexture* texture );

	virtual void setSliderTexture( video::ITexture* texture );

	void updateAbsolutePosition();

private:
	f32 GetCurrentAngle_( const core::position2di& mPos );
	bool mouse2rotate_;
	f32 startAngle_, aPos;
	int circleCounter_;

	gui::CNrpRotatableImage* rImage_;
};


}//namespace gui

}//namespace irr
