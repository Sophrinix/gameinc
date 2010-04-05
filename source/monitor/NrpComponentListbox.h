#pragma once

#include <irrlicht.h>

namespace irr
{

namespace gui
{

class CNrpComponentListbox : public irr::gui::IGUIListBox
{
public:
	//! constructor
	CNrpComponentListbox(  gui::IGUIEnvironment* env, 
					irr::gui::IGUIElement* parent, 
					irr::s32 id, 
					irr::core::rect<irr::s32> rectangle	);

	//! destructor
	virtual ~CNrpComponentListbox();

	//! returns amount of list items
	virtual irr::u32 getItemCount() const;

	//! returns string of a list item. the id may be a value from 0 to itemCount-1
	virtual const wchar_t* getListItem(irr::u32 id) const;

	//! adds an list item, returns id of item
	virtual irr::u32 addItem(const wchar_t* text, void* ptrObject);
	virtual irr::u32 addItem( const wchar_t* text );

	//! clears the list
	virtual void clear();

	//! returns id of selected item. returns -1 if no item is selected.
	virtual irr::s32 getSelected() const;
	virtual void* getSelectedObject();

	//! sets the selected item. Set this to -1 if no item should be selected
	virtual void setSelected(irr::s32 id);

	//! 
	virtual void setSelected( const wchar_t *item );

	//!
	virtual void setItemHeight( s32 height ); 

	//!
	virtual void setDrawBackground(bool draw);

	//! called if an event happened.
	virtual bool OnEvent(const irr::SEvent& event);

	//! draws the element and its children
	virtual void draw();

	//! adds an list item with an icon
	//! \param text Text of list entry
	//! \param icon Sprite index of the Icon within the current sprite bank. Set it to -1 if you want no icon
	//! \return
	//! returns the id of the new created item
	virtual irr::u32 addItem(const wchar_t* text, void* ptrObject, irr::s32 icon);
	virtual irr::u32 addItem(const wchar_t* text, irr::s32 icon);

	//! Returns the icon of an item
	virtual irr::s32 getIcon(irr::u32 id) const;
	virtual void* getObject( irr::u32 id ) const;

	//! removes an item from the list
	virtual void removeItem(irr::u32 id);

	//! Sets the sprite bank which should be used to draw list icons. This font is set to the sprite bank of
	//! the built-in-font by default. A sprite can be displayed in front of every list item.
	//! An icon is an index within the icon sprite bank. Several default icons are available in the
	//! skin through getIcon
	virtual void setSpriteBank(irr::gui::IGUISpriteBank* bank);

	//! sets if automatic scrolling is enabled or not. Default is true.
	virtual void setAutoScrollEnabled(bool scroll);

	//! returns true if automatic scrolling is enabled, false if not.
	virtual bool isAutoScrollEnabled() const;

	//! Update the position and size of the listbox, and update the scrollbar
	virtual void updateAbsolutePosition();

	//! Writes attributes of the element.
	virtual void serializeAttributes(	irr::io::IAttributes* out, 
										irr::io::SAttributeReadWriteOptions* options) const;

	//! Reads attributes of the element
	virtual void deserializeAttributes( irr::io::IAttributes* in, 
										irr::io::SAttributeReadWriteOptions* options);

	//! set all item colors at given index to color
	virtual void setItemOverrideColor(irr::u32 index, const irr::video::SColor &color);

	//! set all item colors of specified type at given index to color
	virtual void setItemOverrideColor(	irr::u32 index, 
										irr::gui::EGUI_LISTBOX_COLOR colorType, 
										const irr::video::SColor &color);

	//! clear all item colors at index
	virtual void clearItemOverrideColor(irr::u32 index);

	//! clear item color at index for given colortype 
	virtual void clearItemOverrideColor(	irr::u32 index, 
											irr::gui::EGUI_LISTBOX_COLOR colorType);

	//! has the item at index it's color overwritten?
	virtual bool hasItemOverrideColor(	irr::u32 index, 
										irr::gui::EGUI_LISTBOX_COLOR colorType) const;

	//! return the overwrite color at given item index. 
	virtual irr::video::SColor getItemOverrideColor(irr::u32 index, 
													irr::gui::EGUI_LISTBOX_COLOR colorType) const;

	//! return the default color which is used for the given colorType
	virtual irr::video::SColor getItemDefaultColor(irr::gui::EGUI_LISTBOX_COLOR colorType) const;

	//! set the item at the given index 
	virtual void setItem(irr::u32 index, const wchar_t* text, void* ptrObject, irr::s32 icon);
	virtual void setItem(irr::u32 index, const wchar_t* text, irr::s32 icon);

	//! Insert the item at the given index 
	//! Return the index on success or -1 on failure.
	virtual irr::s32 insertItem(irr::u32 index, const wchar_t* text, void* ptrObject, irr::s32 icon);
	virtual irr::s32 insertItem(irr::u32 index, const wchar_t* text, irr::s32 icon);

	//! Swap the items at the given indices
	virtual void swapItems(irr::u32 index1, irr::u32 index2);

protected:

	struct ListItem
	{
		ListItem() : icon(-1), obj(NULL) 
		{}

		irr::core::stringw text;
		irr::s32 icon;
		void* obj;
		bool blink, confirm;
		size_t last_time_blink;

		// A multicolor extension
		struct ListItemOverrideColor
		{
			ListItemOverrideColor() : Use(false) {}
			bool Use;
			irr::video::SColor Color;
		};
		ListItemOverrideColor OverrideColors[irr::gui::EGUI_LBC_COUNT];
	};

	void recalculateItemHeight();
	void selectNew(irr::s32 ypos, bool onlyHover=false);
	void recalculateScrollPos();

	// extracted that function to avoid copy&paste code
	void recalculateItemWidth(irr::s32 icon);

	// get labels used for serialization
	bool getSerializationLabels(irr::gui::EGUI_LISTBOX_COLOR colorType, 
								irr::core::stringc & useColorLabel, 
								irr::core::stringc & colorLabel) const;

	irr::core::array< ListItem > Items;
	irr::s32 Selected;
	irr::s32 ItemHeight;
	irr::s32 TotalItemHeight;
	irr::s32 ItemsIconWidth;
	irr::gui::IGUIFont* Font;
	irr::gui::IGUISpriteBank* IconBank;
	irr::gui::IGUIScrollBar* ScrollBar;
	bool Selecting;
	bool DrawBack;
	bool MoveOverSelect;
	irr::u32 selectTime;
	bool AutoScroll;
	irr::core::stringw KeyBuffer;
	irr::u32 LastKeyTime;
	bool HighlightWhenNotFocused;
};

}//end namespace gui

}//end namespace irr