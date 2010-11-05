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
		core::recti rectangle;
		core::rectf currentRect;
		core::stringw name;
		void* object;

		CNrpImageDescription()
		{
			texture_ = NULL;
			downTexture_ = NULL;
		}

		~CNrpImageDescription()
		{
			texture_->drop();
			downTexture_->drop();
		}

		void SetTexture( video::IVideoDriver* driver, video::ITexture* ptx )
		{
			if( texture_ )
				texture_->drop();

			texture_ = ptx;
			if( texture_ )
				texture_->grab();

			if( downTexture_ )
				downTexture_->drop();

			downTexture_ = CreateDownTexture_( driver, ptx );
		}

		video::ITexture* GetTexture() { return texture_; }
		video::ITexture* GetDownTexture() { return downTexture_; }
	private:
		video::ITexture* CreateDownTexture_( video::IVideoDriver* driver, video::ITexture* ptx );
		video::ITexture* texture_;
		video::ITexture* downTexture_;
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
	u32 addItem( const wchar_t* text);
	u32 addItem( video::ITexture* texture, const wchar_t* text, void* object );
	void removeItem(u32 index);

	bool OnEvent(const SEvent& event);

	void draw();
	void Next( int offset );
	void Prev( int offset );

	void clear();
	s32 getSelected() const { return _activeIndex; }
	void* getObject( int index );
	void setSelected(const wchar_t *item);
	void setSelected(s32 index);
	void setDrawBackground(bool draw) { _drawBackground = draw; }
	void setPictureRect( core::recti rectangle ) { _pictureRect = rectangle; }

	u32 getItemCount() const { return _images.size(); }
	const wchar_t* getListItem(u32 id) const;
private:

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

	video::ITexture* _GetDownTexture( video::ITexture* pTxr );
	void _DrawPairImage( CNrpImageDescription* pDesk );
	void _SendEventSelected( const SEvent& event );
	void _UpdateImages();
	void _UpdatePositions();
	core::recti _CorrectRect( video::ITexture* texture, const core::recti& rectangle );
	void _DrawAny( video::ITexture* txs, const core::recti& rectabgle, video::SColor* colors );
	s32 _activeIndex;
	s32 _lastTimeKey;
	core::array< CNrpImageDescription* > _images;

	bool _drawBackground;
	core::recti _pictureRect;
};

}//end namespace gui

}//end namespace irr