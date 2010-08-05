#include "stdafx.h"
#include "HTMLEngine.h"
#include "NrpBrowserWindow.h"
#include "nrpEngine.h"
#include <cassert>
#include <string>
//*! создаем один экземпляр класса для работы
static nrp::HTMLEngine* HTMLRenderer = 0;

const std::string CARET_RESPONSE( "\n" );

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
	std::string profileBaseDir = std::string(__argv[0]).substr(0, std::string( __argv[0]).find_last_of("\\/"));
	HWND w = (HWND)driver->getExposedVideoData().OpenGLWin32.HWnd;
	llmozlib_ = LLMozLib::getInstance();
	//стартуем веб-движок
	bool k = llmozlib_->init(profileBaseDir, profileBaseDir, profileBaseDir, reinterpret_cast<void*>(w));	

	browserWindow_ = NULL;
	dataUpdated_ = false;
	//время последнего обращения надо для уменьшения нагрузки на проц, 
	//чтобы не копировать страницу пять раз впустую
	lastTimeUpdate_ = GetTickCount();
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
	}

	return *browserWindow_;
}

void HTMLEngine::Update()
{
	if( !dataUpdated_ && GetTickCount() - lastTimeUpdate_ < 50 )
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
	lastTimeUpdate_ = GetTickCount();
}

void HTMLEngine::onPageChanged( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	dataUpdated_ = true;
}

void HTMLEngine::Navigate( std::string url )
{
	llmozlib_->navigateTo(browserWindowId_, url);
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
	std::string a = "Event: begin navigation to " + eventIn.getEventUri() + CARET_RESPONSE;
	OutputDebugString( a.c_str() );
}

void HTMLEngine::onNavigateComplete( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	std::string a = "Event: end navigation to " + eventIn.getEventUri() + std::string( " with response status of " ) + IntToStr( eventIn.getIntValue() ) + CARET_RESPONSE;
	OutputDebugString( a.c_str() );
}

void HTMLEngine::onUpdateProgress( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	std::string a = "Event: progress value updated to " + eventIn.getIntValue() + CARET_RESPONSE;
	OutputDebugString( a.c_str() );
}

void HTMLEngine::onStatusTextChange( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	std::string a = "Event: status updated to " + eventIn.getStringValue() + CARET_RESPONSE;
	OutputDebugString( a.c_str() );
}

void HTMLEngine::onLocationChange( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	std::string a = "Event: location changed to " + eventIn.getEventUri() + CARET_RESPONSE;
	OutputDebugString( a.c_str() );
}

void HTMLEngine::onClickLinkHref( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	std::string a = "Event: clicked on link to " + eventIn.getStringValue() + CARET_RESPONSE;
	//eventIn.mStringVal2
	OutputDebugString( a.c_str() );

	llmozlib_->navigateTo(browserWindowId_, eventIn.getStringValue());
}

void HTMLEngine::onClickLinkNoFollow( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	std::string a = "Event: clicked on nofollow link to " + eventIn.getStringValue() + CARET_RESPONSE;
	OutputDebugString( a.c_str() );
}
}