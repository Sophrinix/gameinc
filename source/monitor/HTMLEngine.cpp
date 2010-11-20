#include "stdafx.h"
#include "HTMLEngine.h"
#include "NrpBrowserWindow.h"
#include "nrpEngine.h"
#include "nrpScript.h"

#include <cassert>
//*! создаем один экземпляр класса для работы
static nrp::HTMLEngine* HTMLRenderer = 0;

const NrpText CARET_RESPONSE( "\n" );
#define NRP_NOFOLLOW_SCHEME "nrpscript://"

using namespace irr;

namespace nrp
{

HTMLEngine& HTMLEngine::Instance()
{
	if (!HTMLRenderer)
		HTMLRenderer = new HTMLEngine;
		
	return *HTMLRenderer;
}

HTMLEngine::HTMLEngine()
{
	//критическая секция нужна для предотвращения споров за текстуру
	InitializeCriticalSectionAndSpinCount(&rasterCs_, 1000);

	irr::video::IVideoDriver *driver = CNrpEngine::Instance().GetVideoDriver();
	//мозлибе для работы нужно знать, где хранятся конфииг веб-движка
	NrpText profileBaseDir = NrpText(__argv[0]).subString(0, NrpText( __argv[0]).findLastChar( L"\\/", profileBaseDir.size() ));
	HWND w = (HWND)driver->getExposedVideoData().OpenGLWin32.HWnd;
	llmozlib_ = LLMozLib::getInstance();
	//стартуем веб-движок
	bool k = llmozlib_->init( profileBaseDir, profileBaseDir, profileBaseDir, reinterpret_cast<void*>(w));	

	browserWindow_ = NULL;
	dataUpdated_ = false;
	//время последнего обращения надо для уменьшения нагрузки на проц, 
	//чтобы не копировать страницу пять раз впустую
	_lastTimeUpdate = GetTickCount();
	_noFollowLinkExec = false;
	_firstMessage = false;
}

HTMLEngine::~HTMLEngine()
{
	llmozlib_->remObserver( browserWindowId_, this );
	llmozlib_->destroyBrowserWindow( browserWindowId_ ); 
	llmozlib_->reset();
}

//*! создание окна в котором будет отображаться срендериная текстурка
gui::CNrpBrowserWindow& HTMLEngine::GetBrowserWindow(irr::core::dimension2du size)
{
    //ленивое создание окна, создаем когда будет первое обращение
	if( browserWindow_ == NULL && size.Width > 0 && size.Height > 0 )
	{
		browserWindowId_ = llmozlib_->createBrowserWindow(size.Width, size.Height);

		// сообщить мозлибу о размерах созданного окна
		llmozlib_->setSize(browserWindowId_, size.Width, size.Height);

		// я - обсервер
		llmozlib_->addObserver(browserWindowId_, this);

		llmozlib_->setBackgroundColor(browserWindowId_, 128, 128, 128);

		size_t width = llmozlib_->getBrowserWidth(browserWindowId_);
		size_t height = llmozlib_->getBrowserHeight(browserWindowId_);
 		
		irr::video::IVideoDriver *driver = CNrpEngine::Instance().GetVideoDriver();
		pageTexture_ = driver->addTexture( core::dimension2du( width, height ) , "htmlPageTexture" );

 		browserWindow_ = new gui::CNrpBrowserWindow( CNrpEngine::Instance().GetGuiEnvironment(), NULL, -1, core::position2di( 0, 0 ), size );

		llmozlib_->focusBrowser(browserWindowId_, true);
		browserWindow_->SetTexture( pageTexture_ );
		llmozlib_->setNoFollowScheme( browserWindowId_, NRP_NOFOLLOW_SCHEME );
	}

	return *browserWindow_;
}

void HTMLEngine::Update()
{
	if( !dataUpdated_ && GetTickCount() - _lastTimeUpdate < 50 )
		return;

	const unsigned char* pixels = llmozlib_->grabBrowserWindow( browserWindowId_ );
	if( pixels )
	{
		size_t rasterSize = llmozlib_->getBrowserRowSpan(browserWindowId_) * llmozlib_->getBrowserHeight(browserWindowId_);	

		EnterCriticalSection(&rasterCs_);
		try
		{	
			u32* txsData = (u32*)pageTexture_->lock();

			memcpy(txsData, pixels, rasterSize );

			pageTexture_->unlock();
		}
		catch(...)
		{

		}

		LeaveCriticalSection(&rasterCs_);
	}

	dataUpdated_ = false;
	_lastTimeUpdate = GetTickCount();
}

void HTMLEngine::onPageChanged( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	dataUpdated_ = true;
}

void HTMLEngine::Navigate( const NrpText& url )
{
	Log( SCRIPT ) << "Navigate to " << url << term;
	llmozlib_->navigateTo( browserWindowId_, const_cast< NrpText& >( url ).ToStr() );
}

void HTMLEngine::MouseDown( size_t x, size_t y )
{
	llmozlib_->mouseDown(browserWindowId_, x,  y);
}

void HTMLEngine::MouseMoved( size_t x, size_t y )
{
	llmozlib_->mouseMove(browserWindowId_, x,  y);
}

void HTMLEngine::MouseUp(size_t x, size_t y)
{
	llmozlib_->mouseUp(browserWindowId_, x,  y);
}

void HTMLEngine::ScrollByLines( int lines )
{
	llmozlib_->scrollByLines(browserWindowId_, lines);
}

void HTMLEngine::SetFocus()
{
	llmozlib_->focusBrowser(browserWindowId_, true);
}

void HTMLEngine::KeyPress( int key )
{
	llmozlib_->keyPress(browserWindowId_, key);
}

void HTMLEngine::onNavigateBegin( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	if( _firstMessage )
	{
#ifdef _DEBUG
		Log( SCRIPT ) << "Event: begin navigation to " << eventIn.getEventUri().c_str() << term;
#endif
		_firstMessage = false;
	}
	else
		_firstMessage = true;
}

void HTMLEngine::onNavigateComplete( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	if( _firstMessage )
	{
#ifdef _DEBUG
		Log( SCRIPT ) << "Event: end navigation to " << eventIn.getEventUri().c_str() 
					  << " with response status of " << eventIn.getIntValue() << term;
#endif
		_firstMessage = false;
	}
	else
		_firstMessage = true;
}

void HTMLEngine::onUpdateProgress( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	if( _firstMessage )
	{
#ifdef _DEBUG
		Log( SCRIPT ) << "Event: progress value updated to "  << eventIn.getIntValue() << term;
#endif
		_firstMessage = false;
	}
	else
		_firstMessage = true;
}

void HTMLEngine::onStatusTextChange( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	if( _firstMessage )
	{
#ifdef _DEBUG
		Log( SCRIPT ) << "Event: status updated to " << eventIn.getStringValue().c_str() << term;
#endif
		_firstMessage = false;
	}
	else
		_firstMessage = true;
}

void HTMLEngine::onLocationChange( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	if( _firstMessage )
	{
#ifdef _DEBUG
		Log( SCRIPT ) << "Event: location changed to " << eventIn.getEventUri().c_str() << term;
#endif
		_firstMessage = false;
	}
	else
		_firstMessage = true;
}

void HTMLEngine::onClickLinkHref( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	if( _firstMessage )
	{
#ifdef _DEBUG
		Log( SCRIPT ) << "Event: clicked on link to " << eventIn.getStringValue().c_str() << term;
#endif
		llmozlib_->navigateTo(browserWindowId_, eventIn.getStringValue());
		_firstMessage = false;
	}
	else
		_firstMessage = true;
}

NrpText DecodeUrl( const NrpText& url )
{
	NrpText decodeStr;
	for( u32 pos=0; pos < url.size(); pos++ )
		if( url[ pos ] != L'%' )
			decodeStr.append( url[ pos ] ); 
		else
		{
			decodeStr.append( (( url[ pos+1 ] - 0x30 ) << 4) + ( url[ pos + 2 ] - 0x30 ) );
			pos += 2;
		}

	return decodeStr;
}

void HTMLEngine::onClickLinkNoFollow( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	if( _noFollowLinkExec )
	{
		NrpText action = eventIn.getStringValue().c_str();
		action = DecodeUrl( action.subString( strlen( NRP_NOFOLLOW_SCHEME ), action.size() ) );
		
		nrp::CNrpScript::Instance().SetSender( NULL );
		CNrpScript::Instance().DoString( action );
		_noFollowLinkExec = false;
	}
	else
		_noFollowLinkExec = true;

#ifdef _DEBUG
	Log( SCRIPT ) << "Event: clicked on nofollow link to " << eventIn.getStringValue().c_str() << term;
#endif
}
}