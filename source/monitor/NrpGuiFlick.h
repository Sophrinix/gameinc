#pragma once

#include <irrlicht.h>

namespace irr
{

namespace gui
{

class CNrpGuiFlick : public IGUIWindow
{
public:

	//! constructor
	CNrpGuiFlick( IGUIEnvironment* guienv, IGUIElement* parent, core::recti rectangle, u32 column=4, s32 id=-1 );

	//! destructor
	virtual ~CNrpGuiFlick();

	//! draws the element and its children
	void draw();

	void addChildToEnd( IGUIElement* elm );
	bool OnEvent( const irr::SEvent& event );
	IGUIElement* GetLast();
	IGUIElement* GetFirst();
	void Clear();
protected:
	IGUIButton* _btnUp;
	IGUIButton* _btnDown;

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

	s32 _starty;
	u32 _column;
};

}//namespace gui

}//namspace irr
