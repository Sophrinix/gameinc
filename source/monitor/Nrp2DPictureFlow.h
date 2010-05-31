#pragma once
#include <irrlicht.h>

namespace irr
{

namespace gui
{

class CNrp2DPictureFlow : public IGUIListBox
{
	class CNrpImageDescription
	{
	public:
		video::ITexture* texture;
		core::recti rectangle;
		core::rectf currentRect;
		core::stringw name;
	};

	CNrp2DPictureFlow();
public:
	~CNrp2DPictureFlow();
	CNrp2DPictureFlow( IGUIEnvironment* env, 
					   IGUIElement* parent, 
					   core::recti rectangle, 
					   core::recti pictureRect,
					   s32 id );
	u32 addItem( video::ITexture* texture, const wchar_t* text );
	u32 addItem(const wchar_t* text);
	void removeItem(u32 index);

	bool OnEvent(const SEvent& event);

	void draw();
	void Next( int offset );
	void Prev( int offset );

	void clear();
	s32 getSelected() const { return activeIndex_; }
	void setSelected(const wchar_t *item);
	void setSelected(s32 index);
	void setDrawBackground(bool draw) { drawBackground_ = draw; }

	u32 getItemCount() const { return images_.size(); }
	const wchar_t* getListItem(u32 id) const;
private:

	void UpdateImages_();
	void UpdatePositions_();
	void clearItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) {}
	void clearItemOverrideColor(u32 index) {}
	bool hasItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) const { return false; }
	video::SColor getItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) const {return 0;}
	video::SColor getItemDefaultColor(EGUI_LISTBOX_COLOR colorType) const { return 0; }
	void setItem(u32 index, const wchar_t* text, s32 icon) {}
	void setItemOverrideColor(u32 index, const video::SColor &color) {}
	void setItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType, const video::SColor &color) {};
	u32 addItem(const wchar_t* text, s32 icon) { return 0; }
	s32 getIcon(u32 index) const { return 0; }
	void setSpriteBank(IGUISpriteBank* bank) {}
	void setAutoScrollEnabled(bool scroll) {}
	bool isAutoScrollEnabled() const { return false; }
	void setItemHeight( s32 height ) {};
	void swapItems(u32 index1, u32 index2) {}
	s32 insertItem(u32 index, const wchar_t* text, s32 icon) { return 0; }

	s32 activeIndex_;
	s32 lastTimeKey_;
	core::array< CNrpImageDescription* > images_;

	bool drawBackground_;
	core::recti pictureRect_;
};

}//end namespace gui

}//end namespace irr