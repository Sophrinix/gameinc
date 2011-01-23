#pragma once

#include <irrlicht.h>

namespace irr
{

namespace gui
{

class CNrpGuiFlick : public irr::gui::IGUIListBox
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

	s32 _starty;
	u32 _column;

	irr::u32 getItemCount() const { return 0; }

	//! returns string of a list item. the id may be a value from 0 to itemCount-1
	const wchar_t* getListItem(irr::u32 id) const { return L""; }
	irr::u32 addItem(const wchar_t* text, irr::s32 icon) { return 0; };
	void removeItem(irr::u32 id) {};
	irr::u32 addItem( const wchar_t* text ) { return 0; };
	irr::s32 getIcon(irr::u32 id) const { return 0; };
	void setSpriteBank(irr::gui::IGUISpriteBank* bank) {};
	irr::s32 getSelected() const { return 0; }
	//! sets the selected item. Set this to -1 if no item should be selected
	void setSelected(irr::s32 id) {};
	void clear() {};
	void setSelected(const wchar_t *item) {};
	//! set whether the listbox should scroll to newly selected items
	void setAutoScrollEnabled(bool scroll) {}
	//! returns true if automatic scrolling is enabled, false if not.
	bool isAutoScrollEnabled() const { return false; }
	//! set all item colors of specified type at given index to color
	void setItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType, const video::SColor &color) {};
	//! set all item colors at given index to color
	void setItemOverrideColor(u32 index, const video::SColor &color) {};
	//! clear item color at index for given colortype
	void clearItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) {};
	void clearItemOverrideColor(u32 index) {};
	bool hasItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) const { return false; }
	video::SColor getItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) const { return 0; };
	video::SColor getItemDefaultColor(EGUI_LISTBOX_COLOR colorType) const { return 0; };
	void setItem(u32 index, const wchar_t* text, s32 icon) {}
	s32 insertItem(u32 index, const wchar_t* text, s32 icon) { return 0; }
	void swapItems(u32 index1, u32 index2) {}
	void setItemHeight( s32 height ) {}
	void setDrawBackground(bool draw) {}


};

}//namespace gui

}//namspace irr
