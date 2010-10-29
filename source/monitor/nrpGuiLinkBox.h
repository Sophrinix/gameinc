#pragma once

#include "nrpButton.h"
#include "LuaFunctionality.h"

namespace irr
{

namespace gui
{

const int EGUIET_GUILINK = EGUIET_COUNT + 10;

class CNrpGuiLinkBox : public CNrpButton, public ILuaFunctionality
{
public:

	//! constructor
	CNrpGuiLinkBox( IGUIEnvironment* guienv, 
					gui::IGUIElement* parent,
					s32 id, 
					const core::recti& rectangle,
					bool noclip=false);

	//! destructor
	virtual ~CNrpGuiLinkBox();

	//! called if an event happened.
	virtual bool OnEvent(const SEvent& event);

	//! draws the element and its children
	virtual void draw();

	//! 
	virtual void SetData( void* data );
	//! 
	virtual void* GetData() const { return data_; }

	//! 
	virtual void SetDraggable( bool isDraggable )
	{
		isDraggable_ = isDraggable;
	}

	video::ITexture* getImage() { return image_; }

	virtual bool IsDraggable() const { return isDraggable_; }

	virtual void setDefaultImage( video::ITexture* image );

	int getModuleType() const
	{
		return moduleType_;
	}

	void setModuleType( int typen )
	{
		moduleType_ = typen;
	}

	void setRelativePosition( const core::recti& rectangle );

	gui::EGUI_ELEMENT_TYPE getType();

	virtual const c8* getTypeName() const
	{
		return "CNrpGuiLinkBox";
	}
protected:
	u32 clickTime_;

	video::ITexture* _defaultImage;
	core::recti _defaultImageRect;

	void* data_;

	int moduleType_;
	bool isDraggable_;

	bool ButtonLMouseUp_( const irr::SEvent& event );
	bool ButtonLMouseDown_( const irr::SEvent& event );
};

}//namespace gui

}//namspace irr