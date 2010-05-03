#include "stdafx.h"
#include "nrpGUIEnvironment.h"
#include "nrpHUDConfig.h"
#include "SkinLoader.h"
#include "StrConversation.h"
#include "NrpWindow.h"
#include "NrpGuiBlendAnimator.h"
#include "NrpScrollBar.h"
#include "NrpGuiLuaAnimator.h"
#include "NrpCircleScrollBar.h"
#include "NrpGuiRectAnimator.h"
#include "NrpGuiMoveAnimator.h"
#include "NrpGuiHoveredAnimator.h"
#include "NrpGuiTextTimeAnimator.h"
#include "NrpProgressBar.h"
#include "nrpGuiLinkBox.h"
#include "NrpGuiCursorPosAnimator.h"
#include "NrpComponentListbox.h"

#include <stdexcept>
#include <string>

namespace irr
{

namespace gui
{

bool CNrpGUIEnvironment::CreateSkin_()
{
	video::IVideoDriver* driver = getVideoDriver();
	
	SImageGUISkinConfig guicfg;
	CNrpImageSkinLoader::Load( getFileSystem(), 
	 						   getVideoDriver(), 
							   "skin/guiskin.cfg",
							   guicfg );  //���� ������������ �����
	CImageGUISkin* adv_skin = new CImageGUISkin( this );
	adv_skin->LoadConfig( guicfg );													//�������������� ����										
    
	native_gui_->setSkin( adv_skin );													//�������� ���������� ����������	

	LoadFonts_();  //�������� ������ ��� ������	
						
	return true;
}
//////////////////////////////////////////////////////////////////////////

void CNrpGUIEnvironment::LoadFonts_()
{
	int min_font = nrp::CNrpHUDConfig::Instance().GetValue<int>( nrp::MIN_FONT_SIZE );
	size_t max_font = nrp::CNrpHUDConfig::Instance().GetValue<int>( nrp::MAX_FONT_SIZE );
	for( size_t cnt=min_font; 
		 cnt < max_font;
		 cnt++ )
	{
		std::string option = "font_" + nrp::IntToStr( (int)cnt );
		std::string filename = nrp::CNrpHUDConfig::Instance().GetValue<std::string>( option );
	
		IGUIFont* font = native_gui_->getFont( filename.c_str() );
		if ( font )																	//���� ����� ���������	
		{	
			fonts_[ option ] = font;
			//adv_skin->setColor( EGDC_ACTIVE_CAPTION, video::SColor( 0xff, 0, 0, 0 ) );
		}
	}
	
	{
		std::string filename = nrp::CNrpHUDConfig::Instance().GetValue<std::string>( nrp::FONT_TOOLTIP );
		IGUIFont* font = native_gui_->getFont( filename.c_str() );
	
		if( !font )
			font = native_gui_->getBuiltInFont();

		native_gui_->getSkin()->setFont( font, gui::EGDF_TOOLTIP); //������������� �����
	}

	{
		std::string filename = nrp::CNrpHUDConfig::Instance().GetValue<std::string>( nrp::FONT_SIMPLE );
		IGUIFont* font = native_gui_->getFont( filename.c_str() );

		if( !font )
			font = native_gui_->getBuiltInFont();

		native_gui_->getSkin()->setFont( font, gui::EGDF_BUTTON ); //������������� �����
		native_gui_->getSkin()->setFont( font, gui::EGDF_DEFAULT );
	}
}
//////////////////////////////////////////////////////////////////////////

CNrpGUIEnvironment::CNrpGUIEnvironment( gui::IGUIEnvironment* native_gui )
{
	native_gui_ = native_gui;
	dragObject_ = NULL;
	CreateSkin_();
}
//////////////////////////////////////////////////////////////////////////

CNrpGUIEnvironment::~CNrpGUIEnvironment()
{
	CImageGUISkin* adv_skin = reinterpret_cast< CImageGUISkin* >( native_gui_->getSkin() );
	native_gui_->setSkin( adv_skin->GetNativeSkin() );

	IGUIFont* fonggg =  native_gui_->getSkin()->getFont( gui::EGDF_TOOLTIP );

	adv_skin->drop();
	IGUISkin* old_skin = native_gui_->getSkin();
	
	old_skin->drop();
}

//////////////////////////////////////////////////////////////////////////

IGUIEditBox* CNrpGUIEnvironment::addEditBox( const wchar_t* text,	
											 const core::rect<s32>& rectangle, 
											 bool border/*=true*/, 
											 IGUIElement* parent/*=0*/, 
											 s32 id/*=-1*/ )
{
	IGUIEditBox *result = native_gui_->addEditBox( text, rectangle, border, parent, id );
	result->setOverrideColor( video::SColor(0xff, 0, 0, 0 ) );
	result->enableOverrideColor( true );

	return result;
}
//////////////////////////////////////////////////////////////////////////

IGUIButton* CNrpGUIEnvironment::addButton( const core::recti& rectangle, 
										   gui::IGUIElement* parent/*=0*/, 
										   s32 id/*=-1*/, 
										   const wchar_t* text/*=0*/, 
										   const wchar_t* tooltiptext /*= 0*/ )
{
	CNrpButton *button = new CNrpButton( this, parent, id, rectangle );
	button->setText( text );
	button->setToolTipText( tooltiptext );
	button->setOverrideColor( video::SColor(0xff, 0xff, 0xff, 0xff ) );
	button->enableOverrideColor( true );

	button->drop();
	return button;
}
//////////////////////////////////////////////////////////////////////////

IGUIButton* CNrpGUIEnvironment::addButton( const core::rect<s32>& rectangle, 
										   gui::IGUIElement* parent/*=0*/, 
										   const char* normal_texture,
										   const char* pressed_texture,
										   const char* hovered_texture,
										   s32 id/*=-1*/ )
{
	CNrpButton *button = new CNrpButton( this, parent, id, rectangle );
	button->setText( L"" );
	button->setToolTipText( L"" );

	video::IVideoDriver* driver = native_gui_->getVideoDriver();
	button->setImage( driver->getTexture( normal_texture ) );
	button->setPressedImage( driver->getTexture( pressed_texture ) );
	button->setHoveredImage( driver->getTexture( hovered_texture ) );

	button->setOverrideColor( video::SColor( 0xffffffff ) );
	button->enableOverrideColor( true );

	button->drop();
	return button;
}
//////////////////////////////////////////////////////////////////////////


IGUIStaticText* CNrpGUIEnvironment::addStaticText( const wchar_t* text, const core::rect<s32>& rectangle, bool border/*=false*/, bool wordWrap/*=true*/, gui::IGUIElement* parent/*=0*/, s32 id/*=-1*/, bool fillBackground /*= false*/ )
{
	IGUIStaticText *result = native_gui_->addStaticText( text, rectangle, border, wordWrap, parent, id, fillBackground );
	result->setOverrideColor( video::SColor(0xff, 0, 0, 0 ) );
	result->enableOverrideColor( true );

	return result;
}

//////////////////////////////////////////////////////////////////////////
irr::gui::IGUIElement* CNrpGUIEnvironment::getRootGUIElement()
{
	return native_gui_->getRootGUIElement();
}
//////////////////////////////////////////////////////////////////////////

void CNrpGUIEnvironment::drawAll()
{
	std::vector< IGUIElement* >::iterator pIter = deletionQueue_.begin();
	for( ; pIter != deletionQueue_.end(); pIter++ )
		 (*pIter)->remove();

	deletionQueue_.clear();

	native_gui_->drawAll();
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIContextMenu* CNrpGUIEnvironment::addContextMenu( 
	const irr::core::rect< irr::s32 >& rectangle, 
	irr::gui::IGUIElement* parent/*=0*/, 
	irr::s32 id/*=-1 */ )
{
  return native_gui_->addContextMenu( rectangle, parent, id );
}
//////////////////////////////////////////////////////////////////////////

bool CNrpGUIEnvironment::setFocus( irr::gui::IGUIElement* element )
{
    return native_gui_->setFocus( element );
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIElement* CNrpGUIEnvironment::getFocus() const
{
	return native_gui_->getFocus();
}
//////////////////////////////////////////////////////////////////////////

irr::video::IVideoDriver* CNrpGUIEnvironment::getVideoDriver() const
{
	return native_gui_->getVideoDriver();
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIScrollBar* CNrpGUIEnvironment::addScrollBar( bool horizontal, 
														   const irr::core::rect< irr::s32 >& rectangle, 
														   irr::gui::IGUIElement* parent/*=0*/, 
														   irr::s32 id/*=-1*/ )
{
	return new CNrpScrollBar( horizontal, this, parent, id, rectangle );
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIFileOpenDialog* CNrpGUIEnvironment::addFileOpenDialog( 
						const wchar_t* title /*= 0*/, 
						bool modal/*=true*/, 
						irr::gui::IGUIElement* parent/*=0*/, 
						irr::s32 id/*=-1*/ )
{
	return native_gui_->addFileOpenDialog( title, modal, parent, id );
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIWindow* CNrpGUIEnvironment::addMessageBox( const wchar_t* caption, 
														const wchar_t* text/*=0*/, 
														bool modal /*= true*/, 
														irr::s32 flags /*= irr::gui::EMBF_OK*/, 
														irr::gui::IGUIElement* parent/*=0*/, 
														irr::s32 id/*=-1*/,
														video::ITexture* image )
{
	return native_gui_->addMessageBox( caption, text, modal, flags, parent, id, image );
}

gui::IGUIWindow* CNrpGUIEnvironment::addMessageBox( const wchar_t* text, s32 flags, core::array< const char* >& funcNames )
{
	core::dimension2du scrsize = getVideoDriver()->getScreenSize();
	core::recti rectangle( scrsize.Width / 2 - 200, scrsize.Height / 2 - 100, scrsize.Width / 2 + 200, scrsize.Height / 2 + 100 );
	IGUIWindow* wnd = new CNrpWindow( this, getRootGUIElement(), NULL, -1, rectangle );
	rectangle = wnd->getRelativePosition();
	addStaticText( text, core::recti( 20, 20, rectangle.getWidth() - 20, 40 ), false, false, wnd, -1, false );

	core::recti btnRect( rectangle.getWidth() / 2 - 60, rectangle.getHeight() - 40, rectangle.getWidth() / 2 + 50,  rectangle.getHeight() - 20 );
	if( flags & gui::EMBF_YES )
	{
		CNrpButton* btn = (CNrpButton*)addButton( btnRect, wnd, -1, L"Yes", 0 );
		btn->setOnClickAction( funcNames[ 0 ] );
	}

	btnRect += core::position2di( 70, 0 );
	if( flags & gui::EMBF_NO )
	{
		CNrpButton* btn = (CNrpButton*)addButton( btnRect, wnd, -1, L"No", 0 );
		btn->setOnClickAction( funcNames[ 1 ] );
	}

	return wnd;
}


irr::gui::IGUICheckBox* CNrpGUIEnvironment::addCheckBox( bool checked, 
														const irr::core::rect< irr::s32 >& rectangle, 
														irr::gui::IGUIElement* parent /*= 0*/, 
														irr::s32 id /*= -1*/, 
														const wchar_t* text /*= 0*/ )
{
	return native_gui_->addCheckBox( checked, rectangle, parent, id, text );
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIComboBox* CNrpGUIEnvironment::addComboBox( const irr::core::rect< irr::s32 >& rectangle, 
														irr::gui::IGUIElement* parent/*=0*/, 
														irr::s32 id/*=-1*/ )
{
	return native_gui_->addComboBox( rectangle, parent, id );
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIListBox* CNrpGUIEnvironment::addListBox( 
				const irr::core::rect< irr::s32>& rectangle, 
				irr::gui::IGUIElement* parent/*=0*/, 
				irr::s32 id/*=-1*/, 
				bool drawBackground/*=false*/ )
{
	return native_gui_->addListBox( rectangle, parent, id, drawBackground );
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUISkin* CNrpGUIEnvironment::getSkin() const
{
	return native_gui_->getSkin();
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIImage* CNrpGUIEnvironment::addImage( 
					const irr::core::rect< irr::s32 >& rectangle, 
					irr::gui::IGUIElement* parent/*=0*/, 
					irr::s32 id/*=-1*/, 
					const wchar_t* text/*=0*/ )
{
	return native_gui_->addImage( rectangle, parent, id, text );
}

irr::gui::IGUIImage* CNrpGUIEnvironment::addImage(  irr::video::ITexture* image, 
																						irr::core::position2d<irr::s32> pos, 
																						bool useAlphaChannel/*=true*/, 
																						irr::gui::IGUIElement* parent/*=0*/, 
																						irr::s32 id/*=-1*/, const wchar_t* text/*=0*/ )
{
	return native_gui_->addImage( image, pos, useAlphaChannel, parent, id, text );
}

irr::gui::IGUIFont* CNrpGUIEnvironment::getFont( const io::path& font_name )
{
	std::map< std::string, gui::IGUIFont* >::iterator pIter;
		
	pIter = fonts_.find( font_name.c_str() );

	if( pIter != fonts_.end() )
		return pIter->second;
	
	return native_gui_->getFont( font_name );
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIFont* CNrpGUIEnvironment::getBuiltInFont() const
{
	return native_gui_->getBuiltInFont();
}

irr::gui::IGUIColorSelectDialog* CNrpGUIEnvironment::addColorSelectDialog( const wchar_t* text, 
																		   bool modal /*= true*/, 
																		   irr::gui::IGUIElement* parent/*=0*/, 
																		   irr::s32 id /*= -1*/ )
{
	return native_gui_->addColorSelectDialog( text, modal, parent, id );	
}

irr::IOSOperator* CNrpGUIEnvironment::getOSOperator() const
{
	return native_gui_->getOSOperator();
}

bool CNrpGUIEnvironment::removeFocus( IGUIElement* element )
{
	return native_gui_->removeFocus( element );	
}

bool CNrpGUIEnvironment::hasFocus( IGUIElement* element ) const
{
	return native_gui_->hasFocus( element );
}

io::IFileSystem* CNrpGUIEnvironment::getFileSystem() const
{
	return native_gui_->getFileSystem();
}

void CNrpGUIEnvironment::clear()
{
	native_gui_->clear();
}

bool CNrpGUIEnvironment::postEventFromUser( const SEvent& event )
{
	return native_gui_->postEventFromUser( event );
}

void CNrpGUIEnvironment::setUserEventReceiver( IEventReceiver* evr )
{
	native_gui_->setUserEventReceiver( evr );
}

void CNrpGUIEnvironment::setSkin( IGUISkin* skin )
{
	//??
	//native_gui_->setSkin( skin );
	throw "Don't use this function"; 
}

IGUISkin* CNrpGUIEnvironment::createSkin( EGUI_SKIN_TYPE type )
{
	return NULL;
}

IGUIImageList* CNrpGUIEnvironment::createImageList( video::ITexture* texture, core::dimension2di imageSize, bool useAlphaChannel )
{
	return native_gui_->createImageList( texture, imageSize, useAlphaChannel );
}

IGUISpriteBank* CNrpGUIEnvironment::getSpriteBank( const io::path& filename )
{
	return native_gui_->getSpriteBank( filename );
}

IGUISpriteBank* CNrpGUIEnvironment::addEmptySpriteBank( const io::path& name )
{
	return native_gui_->addEmptySpriteBank( name );
}

IGUIWindow* CNrpGUIEnvironment::addWindow( const core::rect<s32>& rectangle, bool modal /*= false*/, const wchar_t* text/*=0*/, IGUIElement* parent/*=0*/, s32 id/*=-1*/ )
{
	return native_gui_->addWindow( rectangle, modal, text, parent, id );
}

IGUIElement* CNrpGUIEnvironment::addModalScreen( IGUIElement* parent )
{
	return native_gui_->addModalScreen( parent );
}

IGUITreeView* CNrpGUIEnvironment::addTreeView( const core::rect<s32>& rectangle, IGUIElement* parent/*=0*/, s32 id/*=-1*/, bool drawBackground/*=false*/, bool scrollBarVertical /*= true*/, bool scrollBarHorizontal /*= false*/ )
{
	return native_gui_->addTreeView( rectangle, parent, id, drawBackground, scrollBarVertical, scrollBarHorizontal );
}

IGUIMeshViewer* CNrpGUIEnvironment::addMeshViewer( const core::rect<s32>& rectangle, IGUIElement* parent/*=0*/, s32 id/*=-1*/, const wchar_t* text/*=0*/ )
{
	return native_gui_->addMeshViewer( rectangle, parent, id, text );
}

IGUISpinBox* CNrpGUIEnvironment::addSpinBox( const wchar_t* text, const core::rect<s32>& rectangle, bool border/*=true*/,IGUIElement* parent/*=0*/, s32 id/*=-1*/ )
{
	return native_gui_->addSpinBox( text, rectangle, border, parent, id );
}

IGUIInOutFader* CNrpGUIEnvironment::addInOutFader( const core::rect<s32>* rectangle/*=0*/, IGUIElement* parent/*=0*/, s32 id/*=-1*/ )
{
	return native_gui_->addInOutFader( rectangle, parent, id );
}

IGUITabControl* CNrpGUIEnvironment::addTabControl(	const core::rect<s32>& rectangle, 
													IGUIElement* parent/*=0*/, 
													bool fillbackground/*=false*/, 
													bool border/*=true*/, 
													s32 id/*=-1*/ )
{
	return native_gui_->addTabControl( rectangle, parent, fillbackground, border, id );
}

IGUITab* CNrpGUIEnvironment::addTab( const core::rect<s32>& rectangle, IGUIElement* parent/*=0*/, s32 id/*=-1*/ )
{
	return native_gui_->addTab( rectangle, parent, id );
}

IGUIContextMenu* CNrpGUIEnvironment::addMenu( IGUIElement* parent/*=0*/, s32 id/*=-1*/ )
{
	return native_gui_->addMenu( parent, id );
}

IGUIToolBar* CNrpGUIEnvironment::addToolBar( IGUIElement* parent/*=0*/, s32 id/*=-1*/ )
{
	return native_gui_->addToolBar( parent, id );
}

IGUITable* CNrpGUIEnvironment::addTable( const core::rect<s32>& rectangle, 
										 IGUIElement* parent/*=0*/, 
										 s32 id/*=-1*/, 
										 bool drawBackground/*=false*/ )
{
	return native_gui_->addTable( rectangle, parent, id, drawBackground );
}

IGUIElementFactory* CNrpGUIEnvironment::getDefaultGUIElementFactory() const
{
	return native_gui_->getDefaultGUIElementFactory();
}

void CNrpGUIEnvironment::registerGUIElementFactory( IGUIElementFactory* factoryToAdd )
{
	native_gui_->registerGUIElementFactory( factoryToAdd );
}

irr::u32 CNrpGUIEnvironment::getRegisteredGUIElementFactoryCount() const
{
	return native_gui_->getRegisteredGUIElementFactoryCount();
}

IGUIElementFactory* CNrpGUIEnvironment::getGUIElementFactory( u32 index ) const
{
	return native_gui_->getGUIElementFactory( index );
}

IGUIElement* CNrpGUIEnvironment::addGUIElement( const c8* elementName, IGUIElement* parent/*=0*/ )
{
	return native_gui_->addGUIElement( elementName, parent );
}

bool CNrpGUIEnvironment::saveGUI( io::IWriteFile* file, IGUIElement* start/*=0*/ )
{
	return native_gui_->saveGUI( file, start );
}

bool CNrpGUIEnvironment::saveGUI( const io::path& file, IGUIElement* start/*=0*/ )
{
	return native_gui_->saveGUI( file, start );
}

bool CNrpGUIEnvironment::loadGUI( io::IReadFile* file, IGUIElement* parent/*=0*/ )
{
	return native_gui_->loadGUI( file, parent );
}

bool CNrpGUIEnvironment::loadGUI( const io::path& filename, IGUIElement* parent/*=0*/ )
{
	return native_gui_->loadGUI( filename, parent );
}

void CNrpGUIEnvironment::serializeAttributes( io::IAttributes* out, io::SAttributeReadWriteOptions* options/*=0*/ ) const
{
	native_gui_->serializeAttributes( out, options );
}

void CNrpGUIEnvironment::deserializeAttributes( io::IAttributes* in, io::SAttributeReadWriteOptions* options/*=0*/ )
{
	native_gui_->deserializeAttributes( in, options );
}

void CNrpGUIEnvironment::writeGUIElement( io::IXMLWriter* writer, IGUIElement* node )
{
	native_gui_->writeGUIElement( writer, node );
}

void CNrpGUIEnvironment::readGUIElement( io::IXMLReader* reader, IGUIElement* node )
{
	native_gui_->readGUIElement( reader, node );
}

bool CNrpGUIEnvironment::isHovered( IGUIElement* element ) const
{
	return native_gui_->isHovered( element );
}

IGUIWindow* CNrpGUIEnvironment::addWindow( video::ITexture* texture, 
										   const core::recti& rectangle, 
										   const wchar_t* text/*=0*/, 
										   IGUIElement* parent/*=0*/, 
										   s32 id/*=-1*/ )
{
	IGUIWindow* window = new CNrpWindow( this, parent, texture, id, rectangle );
	window->setText( text );
	return window;
}

gui::IGUIAnimator* CNrpGUIEnvironment::addBlendAnimator( IGUIElement* parent, u32 min, u32 max, f32 step,
														  bool visOnStop, bool remSelf, bool remParent )
{
	IGUIAnimator* anim = new CNrpGuiBlendAnimator( this, parent, min, max, step, visOnStop, remSelf, remParent  );

	return anim;
}

gui::IGUIAnimator* CNrpGUIEnvironment::addLuaAnimator( IGUIElement* parent, const core::stringc& funcName )
{
	IGUIAnimator* anim = new CNrpGuiLuaAnimator( this, parent, funcName );

	return anim;
}

gui::IGUIScrollBar* CNrpGUIEnvironment::addCicrcleScrollBar( IGUIElement* parent, s32 id, const core::recti& rectangle )
{
	IGUIScrollBar* scrb = new CNrpCircleScrollBar( this, parent, id, rectangle );

	return scrb;
}

gui::IGUIAnimator* CNrpGUIEnvironment::addRectAnimator( IGUIElement* parent, const core::recti& minrect,
														const core::recti& maxrect, s32 step )
{
	IGUIAnimator* anim = new CNrpGuiRectAnimator( this, parent, minrect, maxrect, step );

	return anim;
}

gui::IGUIFont* CNrpGUIEnvironment::addFont( const io::path& name, IGUIFont* font )
{
	return native_gui_->addFont(name, font);
}

gui::IGUIAnimator* CNrpGUIEnvironment::addMoveAnimator( IGUIElement* parent, core::position2di stopPos, u32 step, 
														bool visibleOnStop, bool removeOnStop, bool removeParentOnStop )
{
	IGUIAnimator* anim = new CNrpGuiMoveAnimator( this, parent, stopPos, step, visibleOnStop, removeOnStop, removeParentOnStop );

	return anim;
}

void CNrpGUIEnvironment::addToDeletionQueue( IGUIElement* ptrElement )
{
	std::vector< IGUIElement* >::iterator pIter = deletionQueue_.begin();
	for( ; pIter != deletionQueue_.end(); pIter++ )
		if( (*pIter) == ptrElement )
			return;

	deletionQueue_.push_back( ptrElement );
}

gui::IGUIAnimator* CNrpGUIEnvironment::addHoveredAnimator( IGUIElement* parent, u32 min, u32 max, u32 step, bool visOnStop, bool remSelf, bool remParent )
{
	IGUIAnimator* anim = new CNrpGuiHoveredAnimator( this, parent, min, max, step, visOnStop, remSelf, remParent );

	return anim;
}

gui::IGUIAnimator* CNrpGUIEnvironment::addTextTimeAnimator( IGUIElement* parent )
{
	IGUIAnimator* anim = new CNrpGuiTextTimeAnimator( this, parent );

	return anim;
}

gui::IGUIElement* CNrpGUIEnvironment::addProgressBar( IGUIElement* parent, s32 id, core::recti rectangle )
{
	IGUIElement* elm = new CNrpProgressBar( this, parent, id, rectangle );

	return elm;
}

gui::CNrpGuiLinkBox* CNrpGUIEnvironment::addLinkBox( IGUIElement* parent, s32 id, core::recti rectangle )
{
	gui::CNrpGuiLinkBox* elm = new CNrpGuiLinkBox( this, parent, id, rectangle, false );
	elm->setImage( native_gui_->getVideoDriver()->getTexture( "skin/combobox.png" ) );
	elm->setHoveredImage( native_gui_->getVideoDriver()->getTexture( "skin/combobox_focused.png" ) );
	elm->setPressedImage( native_gui_->getVideoDriver()->getTexture( "skin/combobox_disabled.png" ) );
	elm->setScaleImage( true );

	return elm;
}

gui::IGUIAnimator* CNrpGUIEnvironment::addCursorPosAnimator( IGUIElement* parent, core::position2di offset )
{
	IGUIAnimator* anim = new CNrpGuiCursorAnimator( this, parent, offset );

	return anim;
}

void CNrpGUIEnvironment::RemoveAnimators( IGUIElement* elm )
{
	core::list<gui::IGUIElement*> childs = elm->getChildren();
	core::list<gui::IGUIElement*>::Iterator it = childs.begin();

	for (; it != childs.end(); it++)
		if( strcmp( (*it)->getTypeName(), "GuiAnimator" ) == 0 )
			addToDeletionQueue( (*it) );
}

void CNrpGUIEnvironment::setDragObject( IGUIElement* elm )
{
	if (dragObject_)
	{
		RemoveAnimators( dragObject_ );
		dragObject_->setRelativePosition( dragObjBeginPos_ );
	}

	dragObject_ = elm;
	if (dragObject_)
	{
		dragObjBeginPos_ = elm->getRelativePosition().UpperLeftCorner;
		addCursorPosAnimator( dragObject_, core::position2di( 5, 5 ) );
		IGUIElement* parent = elm->getParent();
		if( parent )
			parent->bringToFront( elm );
	}
}

gui::IGUIListBox* CNrpGUIEnvironment::addComponentListBox( const core::recti& rectangle, gui::IGUIElement* parent, s32 id )
{
	return new CNrpComponentListbox( this, parent, id, rectangle );
}
}//namespace gui

}//namespace irr