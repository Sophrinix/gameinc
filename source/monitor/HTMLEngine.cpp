
#include "stdafx.h"
#include "HTMLEngine.h"
#include "NrpBrowserWindow.h"
#include "NrpHtmlEngineConfig.h"
#include "nrpEngine.h"
#include "nrpScript.h"
#include "NrpGame.h"
#include "NrpApplication.h"
#include "OpFileSystem.h"
#include "NrpTime.h"
#include "NrpPlatform.h"
#include "NrpTechnology.h"
#include "NrpGameMarket.h"
#include "NrpExtInfo.h"

#include <iostream>
#include <fstream>
#include <assert.h>
//*! создаем один экземпляр класса для работы
static nrp::HTMLEngine* HTMLRenderer = NULL;

const NrpText CARET_RESPONSE( "\n" );

using namespace irr;

namespace nrp
{

const NrpText HTMLEngine::nofollowScheme = L"nrpscript://";
const NrpText HTMLEngine::fileScheme = L"file://";
const NrpText HTMLEngine::screenshotSrcTemplate = L"#screenshot_src";
const int HTMLEngine::maxScrNumber=8;

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

	irr::video::IVideoDriver *driver = _nrpEngine.GetVideoDriver();
	//мозлибе для работы нужно знать, где хранятся конфииг веб-движка
	NrpText profileBaseDir = CNrpHtmlEngineConfig::Instance()[ BASEDIR ];
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
 		
		irr::video::IVideoDriver *driver = _nrpEngine.GetVideoDriver();
		pageTexture_ = driver->addTexture( core::dimension2du( width, height ) , "htmlPageTexture" );

 		browserWindow_ = new gui::CNrpBrowserWindow( _nrpEngine.GetGuiEnvironment(), NULL, -1, core::position2di( 0, 0 ), size );

		llmozlib_->focusBrowser(browserWindowId_, true);
		browserWindow_->SetTexture( pageTexture_ );
		llmozlib_->setNoFollowScheme( browserWindowId_, const_cast< NrpText& >( nofollowScheme ).ToStr() );
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
	bool ret = llmozlib_->navigateTo( browserWindowId_, const_cast< NrpText& >( url ).ToStr() );

	if( !ret )
	{
		Log( SCRIPT ) << "Can't open url " << url << term;
		llmozlib_->navigateTo( browserWindowId_, _page404.ToStr() );
	}
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

void HTMLEngine::SetFocus( bool focus )
{
	llmozlib_->focusBrowser(browserWindowId_, focus);
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


void HTMLEngine::onClickLinkNoFollow( const LLEmbeddedBrowserWindowEvent& eventIn )
{
	if( _noFollowLinkExec )
	{
		NrpText action = eventIn.getStringValue().c_str();
        action = NrpText::FromUtf8( action.subString( nofollowScheme.size(), action.size() ) );
		
		CNrpScript::Instance().DoString( action );
		_noFollowLinkExec = false;
	}
	else
		_noFollowLinkExec = true;

#ifdef _DEBUG
	Log( SCRIPT ) << "Event: clicked on nofollow link to " << eventIn.getStringValue().c_str() << term;
#endif
}

void HTMLEngine::SetPage404( const NrpText& pageUrl )
{
	llmozlib_->set404RedirectUrl( browserWindowId_, const_cast< NrpText& >( pageUrl ).ToStr() );
	_page404 = pageUrl;
}

void HTMLEngine::CreateDescription( const NrpText& templateFile, const NrpText& fileName, const CNrpGame& game )
{
    irr::io::IFileSystem* fs = _nrpEngine.GetFileSystem();
    if( !fs )
        return;

    irr::io::IReadFile* file = fs->createAndOpenFile( templateFile );
    
    if( !file )
        return;

    char* buf = new char[ file->getSize() + 5 ];
    memset( buf, 0, file->getSize() + 5 );
    file->read( buf, file->getSize() );

    NrpText templateText( buf ); 
    file->drop();

    templateText = templateText.Replace( L"#game_title", game[ NAME ] );
    templateText = templateText.Replace( L"#cover_title", game[ NAME ] );
    NrpText coverImgPath = OpFileSystem::CheckEndSlash( _nrpApp[ WORKDIR ] ) + (NrpText)game[ TEXTURENORMAL ];
    templateText = templateText.Replace( L"#cover_img_path", fileScheme + coverImgPath );
    templateText = templateText.Replace( L"#owner_title", game[ OWNER ] );
    //outText = outText.Replace( L"#developer_title", "" );
    wchar_t timeStr[ MAX_PATH ] = { 0 };
    NrpTime& startTime = game[ STARTDATE ].As<NrpTime>();
    swprintf( timeStr, MAX_PATH, L"%04d.%02d.%02d", startTime.RYear(), startTime.RMonth(), startTime.RDay() );
  
    templateText = templateText.Replace( L"#date_released", timeStr );

    NrpText platformStr, plName;
    for( int i=0; i < (int)game[ PLATFORMNUMBER ]; i++ )
    {
         plName = const_cast< CNrpGame& >( game ).GetPlatformName( i );
         CNrpPlatform* pl = CNrpGameMarket::Instance().GetPlatform( plName );
             
         platformStr.append( pl ? (NrpText)(*pl)[ NAME ] : plName );
         platformStr.append( i < (int)game[ PLATFORMNUMBER ]-1 ? ",\n" : "." );
    }

    templateText = templateText.Replace( L"#platforms_title", platformStr );

    NrpText genreStr, gnName;
    for( int i=0; i < (int)game[ GENRE_MODULE_NUMBER ]; i++ )
    {
        gnName = const_cast< CNrpGame& >( game ).GetGenreName( i );
        CNrpTechnology* gnr = _nrpApp.GetTechnology( gnName );

        genreStr.append( gnr ? (NrpText)(*gnr)[ NAME ] : gnName );
        genreStr.append( i < (int)game[ GENRE_MODULE_NUMBER ]-1 ? ",\n" : "." );
    }

    templateText = templateText.Replace( L"#genres_title", genreStr );
    templateText = templateText.Replace( L"#rank_title", NrpText( (int)game[ STARTRATING ] ) );

    if( CNrpExtInfo* extInfo = game[ EXTINFO ].As< CNrpExtInfo*>() )
    {
        const STRINGS& images = extInfo->GetImages();
        for( u32 i=0; i < images.size(), i < maxScrNumber; i++ )
        {
            int randomIndex = rand() % images.size();
            templateText = templateText.Replace( screenshotSrcTemplate + NrpText( (int)i ), fileScheme + OpFileSystem::CheckAbsolutePath( images[ randomIndex ] ) );
        }
    }

  
    irr::io::IWriteFile* wrFile = fs->createAndWriteFile( fileName );
    if( !wrFile )
        return;
    
    templateText = templateText.ToUtf8( "&#x04", ";" );
    wrFile->write( templateText.ToStr(), templateText.size() );
    wrFile->drop();
}

}