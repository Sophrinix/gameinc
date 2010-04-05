#pragma once
#include <IGUIScrollBar.h>
#include <IGUIButton.h>
#include <ITexture.h>

namespace irr
{

namespace gui
{

class CNrpScrollBar : public IGUIScrollBar
{
public:

	//! constructor
	CNrpScrollBar(bool horizontal, IGUIEnvironment* environment,
		IGUIElement* parent, s32 id, core::rect<s32> rectangle,
		bool noclip=false);

	//! destructor
	virtual ~CNrpScrollBar();

	//! called if an event happened.
	virtual bool OnEvent(const SEvent& event);

	//! draws the element and its children
	virtual void draw();

	virtual void OnPostRender(u32 timeMs);

	//! gets the maximum value of the scrollbar.
	virtual s32 getMax() const;

	//! sets the maximum value of the scrollbar.
	virtual void setMax(s32 max);

	//! gets the minimum value of the scrollbar.
	virtual s32 getMin() const;

	//! sets the minimum value of the scrollbar.
	virtual void setMin(s32 max);

	//! gets the small step value
	virtual s32 getSmallStep() const;

	//! sets the small step value
	virtual void setSmallStep(s32 step);

	//! gets the large step value
	virtual s32 getLargeStep() const;

	//! sets the large step value
	virtual void setLargeStep(s32 step);

	//! gets the current position of the scrollbar
	virtual s32 getPos() const;

	//! sets the position of the scrollbar
	virtual void setPos(s32 pos);

	//! sets the texture which draw
	virtual void setTexture( video::ITexture* texture );

	//! gets the UpButton
	virtual gui::IGUIButton* getUpButton() { return UpButton; }

	virtual gui::IGUIButton* getDownButton() { return DownButton; }

	//! sets the onChangedAction
	virtual void setAction( const char* funcName ) { onChangedAction_ = funcName; }

	virtual void setSliderTexture( video::ITexture* texture );

	virtual const char* getAction() { return onChangedAction_.c_str(); }

	//! updates the rectangle
	virtual void updateAbsolutePosition();

	//! Writes attributes of the element.
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const;

	//! Reads attributes of the element
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

protected:
	
	void refreshControls();
	s32 getPosFromMousePos(const core::position2di &p) const;

	IGUIButton* UpButton;
	IGUIButton* DownButton;

	core::rect<s32> SliderRect;

	bool Dragging;
	bool Horizontal;
	bool DraggedBySlider;
	bool TrayClick;
	s32 Pos;
	s32 DrawPos;
	s32 DrawHeight;
	s32 Min;
	s32 Max;
	s32 SmallStep;
	s32 LargeStep;
	s32 DesiredPos;
	u32 LastChange;
	video::ITexture* texture_;
	video::ITexture* sliderTexture_;
	core::recti textureRect_, sliderTextureRect_;
	core::stringc onChangedAction_;

	f32 range () const { return (f32) ( Max - Min ); }
};


}//namespace gui

}//namespace irr