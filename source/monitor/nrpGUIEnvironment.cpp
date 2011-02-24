#include "stdafx.h"
#include "nrpGUIEnvironment.h"
#include "nrpHUDConfig.h"
#include "SkinLoader.h"
#include "NrpText.h"
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
#include "Nrp2DPictureFlow.h"
#include "NrpTechMap.h"
#include "NrpGuiElementDestructor.h"
#include "NrpGuiTextRunnerAnimator.h"

#include <stdexcept>

namespace irr
{

namespace gui
{

bool CNrpGUIEnvironment::CreateSkin_()
{
	SImageGUISkinConfig guicfg;
	CNrpImageSkinLoader::Load( getFileSystem(), 
	 						   getVideoDriver(), 
							   "skin/guiskin.cfg",
							   guicfg );  //файл конфигурации скина
	CImageGUISkin* adv_skin = new CImageGUISkin( this );
	adv_skin->LoadConfig( guicfg );													//инициализируем скин										
    
	_nativeEnv->setSkin( adv_skin );													//заменяем дефолтовый обработчик	

	LoadFonts_();  //получаем шрифты для работы	
						
	return true;
}
//////////////////////////////////////////////////////////////////////////

void CNrpGUIEnvironment::LoadFonts_()
{
	for( int cnt=0; ; cnt++ )
	{
		NrpText option = NrpText( "font_" ) + NrpText( cnt );
		if( !nrp::CNrpHUDConfig::Instance().IsExist( option ) )
			break;

		NrpText filename = nrp::CNrpHUDConfig::Instance()[ option ];
	
		IGUIFont* font = _nativeEnv->getFont( filename );
		fonts_[ option ] = font ? font : _nativeEnv->getBuiltInFont();
	}
	
	{
		NrpText filename = nrp::CNrpHUDConfig::Instance()[ nrp::FONT_TOOLTIP ];
		IGUIFont* font = _nativeEnv->getFont( filename );
	
		if( !font )
			font = _nativeEnv->getBuiltInFont();

		_nativeEnv->getSkin()->setFont( font, gui::EGDF_TOOLTIP); //устанавливаем шрифт
	}

	{
		NrpText filename = nrp::CNrpHUDConfig::Instance()[ nrp::FONT_SIMPLE ];
		IGUIFont* font = _nativeEnv->getFont( filename );

		if( !font )
			font = _nativeEnv->getBuiltInFont();

		_nativeEnv->getSkin()->setFont( font, gui::EGDF_BUTTON ); //устанавливаем шрифт
		_nativeEnv->getSkin()->setFont( font, gui::EGDF_DEFAULT );
	}
}
//////////////////////////////////////////////////////////////////////////

CNrpGUIEnvironment::CNrpGUIEnvironment( gui::IGUIEnvironment* native_gui, ICursorControl* cursor )
{
	_nativeEnv = native_gui;
	_dragObjectSave = NULL;
	_dragTexture = NULL;
	_cursor = cursor;
	CreateSkin_();
}
//////////////////////////////////////////////////////////////////////////

CNrpGUIEnvironment::~CNrpGUIEnvironment()
{
	CImageGUISkin* adv_skin = reinterpret_cast< CImageGUISkin* >( _nativeEnv->getSkin() );
	_nativeEnv->setSkin( adv_skin->GetNativeSkin() );

	adv_skin->drop();
	IGUISkin* old_skin = _nativeEnv->getSkin();
	
	old_skin->drop();
}

//////////////////////////////////////////////////////////////////////////

IGUIEditBox* CNrpGUIEnvironment::addEditBox( const wchar_t* text,	
											 const core::rect<s32>& rectangle, 
											 bool border/*=true*/, 
											 IGUIElement* parent/*=0*/, 
											 s32 id/*=-1*/ )
{
	IGUIEditBox *result = _nativeEnv->addEditBox( text, rectangle, border, parent, id );
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

	video::IVideoDriver* driver = _nativeEnv->getVideoDriver();
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
	IGUIStaticText *result = _nativeEnv->addStaticText( text, rectangle, border, wordWrap, parent, id, fillBackground );
	result->setOverrideColor( video::SColor(0xff, 0, 0, 0 ) );
	result->enableOverrideColor( true );

	return result;
}

//////////////////////////////////////////////////////////////////////////
irr::gui::IGUIElement* CNrpGUIEnvironment::getRootGUIElement()
{
	return _nativeEnv->getRootGUIElement();
}
//////////////////////////////////////////////////////////////////////////

void CNrpGUIEnvironment::drawAll()
{
	core::list< IGUIElement* >::Iterator pIter = _deletionQueue.begin();
	for( ; pIter != _deletionQueue.end(); pIter++ )
	{
		try{ (*pIter)->remove(); }
		catch(...){}
	}

	_deletionQueue.clear();

	_nativeEnv->drawAll();

	for( u32 i=0; i < _overlay.size(); i++ )
	{
		if( _overlay[ i ]->getParent() )
			_overlay[ i ]->getParent()->bringToFront( _overlay[ i ] );
		_overlay[ i ]->draw();
	}

	if( _dragObjectSave )
	{
		video::IVideoDriver* driver = _nativeEnv->getVideoDriver();
		core::dimension2di size = _dragObjectSave->getAbsolutePosition().getSize();
		core::position2di pos = _cursor->getPosition() - size / 2;
		core::recti rectangle( pos, core::dimension2du( size.Width, size.Height ) );
		if( _dragTexture  )
			driver->draw2DImage( _dragTexture, rectangle, core::recti( core::position2di( 0, 0 ), _dragTexture->getOriginalSize() ),
				                 0, 0, true );
		else
			driver->draw2DRectangle( video::SColor( 0xffff0000 ), rectangle);
	}
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIContextMenu* CNrpGUIEnvironment::addContextMenu( 
	const irr::core::rect< irr::s32 >& rectangle, 
	irr::gui::IGUIElement* parent/*=0*/, 
	irr::s32 id/*=-1 */ )
{
  return _nativeEnv->addContextMenu( rectangle, parent, id );
}
//////////////////////////////////////////////////////////////////////////

bool CNrpGUIEnvironment::setFocus( irr::gui::IGUIElement* element )
{
    return _nativeEnv->setFocus( element );
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIElement* CNrpGUIEnvironment::getFocus() const
{
	return _nativeEnv->getFocus();
}
//////////////////////////////////////////////////////////////////////////

irr::video::IVideoDriver* CNrpGUIEnvironment::getVideoDriver() const
{
	return _nativeEnv->getVideoDriver();
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
	return _nativeEnv->addFileOpenDialog( title, modal, parent, id );
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
	return _nativeEnv->addMessageBox( caption, text, modal, flags, parent, id, image );
}

gui::IGUIWindow* CNrpGUIEnvironment::addMessageBox( const wchar_t* text, s32 flags, core::array< int >& funcRefs )
{
	core::dimension2du scrsize = getVideoDriver()->getScreenSize();
	core::recti rectangle( scrsize.Width / 2 - 200, scrsize.Height / 2 - 100, scrsize.Width / 2 + 200, scrsize.Height / 2 + 100 );
	IGUIWindow* wnd = new CNrpWindow( this, getRootGUIElement(), NULL, -1, rectangle );
	rectangle = wnd->getRelativePosition();
	addStaticText( text, core::recti( 20, 20, rectangle.getWidth() - 20, rectangle.getHeight() - 50 ), false, false, wnd, -1, false );

	core::recti btnRect( 30, rectangle.getHeight() - 50, rectangle.getWidth() / 2 - 30,  rectangle.getHeight() - 20 );
	if( flags & gui::EMBF_YES )
	{
		CNrpButton* btn = (CNrpButton*)addButton( btnRect, wnd, -1, L"Yes", 0 );
		btn->setOnClickAction( funcRefs[ 0 ] );
	}

	btnRect += core::position2di( rectangle.getWidth() / 2, 0 );
	if( flags & gui::EMBF_NO )
	{
		CNrpButton* btn = (CNrpButton*)addButton( btnRect, wnd, -1, L"No", 0 );
		btn->setOnClickAction( funcRefs[ 1 ] );
	}

	if( flags == 0 )
	{
		CNrpButton* btn = (CNrpButton*)addButton( core::recti( rectangle.getWidth() / 3, rectangle.getHeight() - 50,
												        	   rectangle.getWidth() * 2 / 3, rectangle.getHeight() - 20 ),
												  wnd, -1, L"OK", 0 );
		btn->setOnClickAction( funcRefs[ 0 ] );
	}

	return wnd;
}


irr::gui::IGUICheckBox* CNrpGUIEnvironment::addCheckBox( bool checked, 
														const irr::core::rect< irr::s32 >& rectangle, 
														irr::gui::IGUIElement* parent /*= 0*/, 
														irr::s32 id /*= -1*/, 
														const wchar_t* text /*= 0*/ )
{
	return _nativeEnv->addCheckBox( checked, rectangle, parent, id, text );
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIComboBox* CNrpGUIEnvironment::addComboBox( const irr::core::rect< irr::s32 >& rectangle, 
														irr::gui::IGUIElement* parent/*=0*/, 
														irr::s32 id/*=-1*/ )
{
	return _nativeEnv->addComboBox( rectangle, parent, id );
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIListBox* CNrpGUIEnvironment::addListBox( 
				const irr::core::rect< irr::s32>& rectangle, 
				irr::gui::IGUIElement* parent/*=0*/, 
				irr::s32 id/*=-1*/, 
				bool drawBackground/*=false*/ )
{
	return _nativeEnv->addListBox( rectangle, parent, id, drawBackground );
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUISkin* CNrpGUIEnvironment::getSkin() const
{
	return _nativeEnv->getSkin();
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIImage* CNrpGUIEnvironment::addImage( 
					const irr::core::rect< irr::s32 >& rectangle, 
					irr::gui::IGUIElement* parent/*=0*/, 
					irr::s32 id/*=-1*/, 
					const wchar_t* text/*=0*/ )
{
	return _nativeEnv->addImage( rectangle, parent, id, text );
}

irr::gui::IGUIImage* CNrpGUIEnvironment::addImage(  irr::video::ITexture* image, 
																						irr::core::position2d<irr::s32> pos, 
																						bool useAlphaChannel/*=true*/, 
																						irr::gui::IGUIElement* parent/*=0*/, 
																						irr::s32 id/*=-1*/, const wchar_t* text/*=0*/ )
{
	return _nativeEnv->addImage( image, pos, useAlphaChannel, parent, id, text );
}

irr::gui::IGUIFont* CNrpGUIEnvironment::getFont( const io::path& font_name )
{
	core::map< stringw, gui::IGUIFont* >::Node* pIter = fonts_.find( font_name.c_str() );
	return pIter ? pIter->getValue() : _nativeEnv->getFont( font_name );
}
//////////////////////////////////////////////////////////////////////////

irr::gui::IGUIFont* CNrpGUIEnvironment::getBuiltInFont() const
{
	return _nativeEnv->getBuiltInFont();
}

irr::gui::IGUIColorSelectDialog* CNrpGUIEnvironment::addColorSelectDialog( const wchar_t* text, 
																		   bool modal /*= true*/, 
																		   irr::gui::IGUIElement* parent/*=0*/, 
																		   irr::s32 id /*= -1*/ )
{
	return _nativeEnv->addColorSelectDialog( text, modal, parent, id );	
}

irr::IOSOperator* CNrpGUIEnvironment::getOSOperator() const
{
	return _nativeEnv->getOSOperator();
}

bool CNrpGUIEnvironment::removeFocus( IGUIElement* element )
{
	return _nativeEnv->removeFocus( element );	
}

bool CNrpGUIEnvironment::hasFocus( IGUIElement* element ) const
{
	return _nativeEnv->hasFocus( element );
}

io::IFileSystem* CNrpGUIEnvironment::getFileSystem() const
{
	return _nativeEnv->getFileSystem();
}

void CNrpGUIEnvironment::clear()
{
	_nativeEnv->clear();
}

bool CNrpGUIEnvironment::postEventFromUser( const SEvent& event )
{
	return _nativeEnv->postEventFromUser( event );
}

void CNrpGUIEnvironment::setUserEventReceiver( IEventReceiver* evr )
{
	_nativeEnv->setUserEventReceiver( evr );
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
	return _nativeEnv->createImageList( texture, imageSize, useAlphaChannel );
}

IGUISpriteBank* CNrpGUIEnvironment::getSpriteBank( const io::path& filename )
{
	return _nativeEnv->getSpriteBank( filename );
}

IGUISpriteBank* CNrpGUIEnvironment::addEmptySpriteBank( const io::path& name )
{
	return _nativeEnv->addEmptySpriteBank( name );
}

IGUIWindow* CNrpGUIEnvironment::addWindow( const core::rect<s32>& rectangle, bool modal /*= false*/, const wchar_t* text/*=0*/, IGUIElement* parent/*=0*/, s32 id/*=-1*/ )
{
	return _nativeEnv->addWindow( rectangle, modal, text, parent, id );
}

IGUIElement* CNrpGUIEnvironment::addModalScreen( IGUIElement* parent )
{
	return _nativeEnv->addModalScreen( parent );
}

IGUITreeView* CNrpGUIEnvironment::addTreeView( const core::rect<s32>& rectangle, IGUIElement* parent/*=0*/, s32 id/*=-1*/, bool drawBackground/*=false*/, bool scrollBarVertical /*= true*/, bool scrollBarHorizontal /*= false*/ )
{
	return _nativeEnv->addTreeView( rectangle, parent, id, drawBackground, scrollBarVertical, scrollBarHorizontal );
}

IGUIMeshViewer* CNrpGUIEnvironment::addMeshViewer( const core::rect<s32>& rectangle, IGUIElement* parent/*=0*/, s32 id/*=-1*/, const wchar_t* text/*=0*/ )
{
	return _nativeEnv->addMeshViewer( rectangle, parent, id, text );
}

IGUISpinBox* CNrpGUIEnvironment::addSpinBox( const wchar_t* text, const core::rect<s32>& rectangle, bool border/*=true*/,IGUIElement* parent/*=0*/, s32 id/*=-1*/ )
{
	return _nativeEnv->addSpinBox( text, rectangle, border, parent, id );
}

IGUIInOutFader* CNrpGUIEnvironment::addInOutFader( const core::rect<s32>* rectangle/*=0*/, IGUIElement* parent/*=0*/, s32 id/*=-1*/ )
{
	return _nativeEnv->addInOutFader( rectangle, parent, id );
}

IGUITabControl* CNrpGUIEnvironment::addTabControl(	const core::rect<s32>& rectangle, 
													IGUIElement* parent/*=0*/, 
													bool fillbackground/*=false*/, 
													bool border/*=true*/, 
													s32 id/*=-1*/ )
{
	return _nativeEnv->addTabControl( rectangle, parent, fillbackground, border, id );
}

IGUITab* CNrpGUIEnvironment::addTab( const core::rect<s32>& rectangle, IGUIElement* parent/*=0*/, s32 id/*=-1*/ )
{
	return _nativeEnv->addTab( rectangle, parent, id );
}

IGUIContextMenu* CNrpGUIEnvironment::addMenu( IGUIElement* parent/*=0*/, s32 id/*=-1*/ )
{
	return _nativeEnv->addMenu( parent, id );
}

IGUIToolBar* CNrpGUIEnvironment::addToolBar( IGUIElement* parent/*=0*/, s32 id/*=-1*/ )
{
	return _nativeEnv->addToolBar( parent, id );
}

IGUITable* CNrpGUIEnvironment::addTable( const core::rect<s32>& rectangle, 
										 IGUIElement* parent/*=0*/, 
										 s32 id/*=-1*/, 
										 bool drawBackground/*=false*/ )
{
	return _nativeEnv->addTable( rectangle, parent, id, drawBackground );
}

IGUIElementFactory* CNrpGUIEnvironment::getDefaultGUIElementFactory() const
{
	return _nativeEnv->getDefaultGUIElementFactory();
}

void CNrpGUIEnvironment::registerGUIElementFactory( IGUIElementFactory* factoryToAdd )
{
	_nativeEnv->registerGUIElementFactory( factoryToAdd );
}

irr::u32 CNrpGUIEnvironment::getRegisteredGUIElementFactoryCount() const
{
	return _nativeEnv->getRegisteredGUIElementFactoryCount();
}

IGUIElementFactory* CNrpGUIEnvironment::getGUIElementFactory( u32 index ) const
{
	return _nativeEnv->getGUIElementFactory( index );
}

IGUIElement* CNrpGUIEnvironment::addGUIElement( const c8* elementName, IGUIElement* parent/*=0*/ )
{
	return _nativeEnv->addGUIElement( elementName, parent );
}

bool CNrpGUIEnvironment::saveGUI( io::IWriteFile* file, IGUIElement* start/*=0*/ )
{
	return _nativeEnv->saveGUI( file, start );
}

bool CNrpGUIEnvironment::saveGUI( const io::path& file, IGUIElement* start/*=0*/ )
{
	return _nativeEnv->saveGUI( file, start );
}

bool CNrpGUIEnvironment::loadGUI( io::IReadFile* file, IGUIElement* parent/*=0*/ )
{
	return _nativeEnv->loadGUI( file, parent );
}

bool CNrpGUIEnvironment::loadGUI( const io::path& filename, IGUIElement* parent/*=0*/ )
{
	return _nativeEnv->loadGUI( filename, parent );
}

void CNrpGUIEnvironment::serializeAttributes( io::IAttributes* out, io::SAttributeReadWriteOptions* options/*=0*/ ) const
{
	_nativeEnv->serializeAttributes( out, options );
}

void CNrpGUIEnvironment::deserializeAttributes( io::IAttributes* in, io::SAttributeReadWriteOptions* options/*=0*/ )
{
	_nativeEnv->deserializeAttributes( in, options );
}

void CNrpGUIEnvironment::writeGUIElement( io::IXMLWriter* writer, IGUIElement* node )
{
	_nativeEnv->writeGUIElement( writer, node );
}

void CNrpGUIEnvironment::readGUIElement( io::IXMLReader* reader, IGUIElement* node )
{
	_nativeEnv->readGUIElement( reader, node );
}

bool CNrpGUIEnvironment::isHovered( IGUIElement* element ) const
{
	return _nativeEnv->isHovered( element );
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

gui::IGUIAnimator* CNrpGUIEnvironment::addLuaAnimator( IGUIElement* parent, const nrp::NrpText& funcName )
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
	return _nativeEnv->addFont(name, font);
}

gui::IGUIAnimator* CNrpGUIEnvironment::addMoveAnimator( IGUIElement* parent, core::position2di stopPos, u32 step, 
														bool visibleOnStop, bool removeOnStop, bool removeParentOnStop )
{
	IGUIAnimator* anim = new CNrpGuiMoveAnimator( this, parent, stopPos, step, visibleOnStop, removeOnStop, removeParentOnStop );

	return anim;
}

void CNrpGUIEnvironment::addToDeletionQueue( IGUIElement* ptrElement )
{
	core::list< IGUIElement* >::Iterator pIter = _deletionQueue.begin();
	for( ; pIter != _deletionQueue.end(); pIter++ )
		if( (*pIter) == ptrElement )
			return;

	_deletionQueue.push_back( ptrElement );
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
	elm->setImage( _nativeEnv->getVideoDriver()->getTexture( "skin/combobox.png" ) );
	elm->setHoveredImage( _nativeEnv->getVideoDriver()->getTexture( "skin/combobox_focused.png" ) );
	elm->setPressedImage( _nativeEnv->getVideoDriver()->getTexture( "skin/combobox_disabled.png" ) );
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

void CNrpGUIEnvironment::setDragObject( IGUIElement* elm, video::ITexture* image )
{
	_dragObjectSave = elm;
	
	if( _dragTexture )
		_dragTexture->drop();

	_dragTexture = image;
	if( _dragTexture )
		_dragTexture->grab();
	
}

gui::IGUIListBox* CNrpGUIEnvironment::addComponentListBox( const core::recti& rectangle, gui::IGUIElement* parent, s32 id )
{
	return new CNrpComponentListbox( this, parent, id, rectangle );
}

gui::CNrp2DPictureFlow* CNrpGUIEnvironment::addPictureFlow( const core::recti& rectangle, 
															const core::recti& pictureRect, 
															s32 id, 
															gui::IGUIElement* parent )
{
	return new CNrp2DPictureFlow( this, parent, rectangle, pictureRect, id );
}

CNrpTechMap* CNrpGUIEnvironment::AddTechMap( const core::recti& rectangle, IGUIElement* parent/*=0*/, s32 id/*=-1*/, bool drawBackground/*=false*/ )
{
	return new CNrpTechMap( this, parent, id, rectangle, true, drawBackground, false );
}

gui::IGUIElement* CNrpGUIEnvironment::AddDestructor( gui::IGUIElement* parent, int time )
{
	return new CNrpGuiElementDestructor( this, parent, time );
}

gui::IGUIAnimator* CNrpGUIEnvironment::addTextRunnerAnimator( IGUIElement* parent, const wchar_t* text )
{
	return new CNrpGuiTextRunnerAnimator( this, parent, text );
}

void CNrpGUIEnvironment::LunchToolTip( IGUIElement* elm )
{

}

void CNrpGUIEnvironment::AddTopElement( IGUIElement* elm )
{
	if( elm )
		_overlay.push_back( elm );
}

}//namespace gui

}//namespace irr