#ifndef _IRR_LAYOUT_H_
#define _IRR_LAYOUT_H_

#include <irrlicht.h>

namespace irr
{

namespace gui
{

class CNrpLayout : public IGUIWindow
{
public:

	//! constructor
	CNrpLayout( IGUIEnvironment* guienv, IGUIElement* parent, core::recti rectangle, u32 buttonInLine=4, s32 id=-1 );

	//! destructor
	virtual ~CNrpLayout();

	//! draws the element and its children
	void draw();

	void addChildToEnd( IGUIElement* elm );
	bool OnEvent( const SEvent& event );

	bool isPointInside(const core::position2d<s32>& point) const;

	void updateAbsolutePosition();
	void setRelativePosition(const core::rect<s32>& r);

	void setMaxSize(core::dimension2du size)
	{
		_size = size;
	}

	//void setVisible( bool visible );

protected:

	IGUIButton* getCloseButton(void) const { return NULL; }
	IGUIButton* getMinimizeButton(void) const { return NULL; }
	IGUIButton* getMaximizeButton(void) const { return NULL; }
	bool isDraggable(void) const { return false; }
	void setDraggable(bool) {}
	void setDrawBackground(bool) {}
	bool getDrawBackground(void) const { return false; } 
	void setDrawTitlebar(bool) {}
	bool getDrawTitlebar(void) const { return false; }
	irr::core::recti getClientRect(void) const { return core::recti(); }

	u32 _btnInLine;
	int _margin;
	core::dimension2du _size;
};

}

}
#endif // QLAYOUT_H
