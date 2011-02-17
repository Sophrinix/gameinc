#include "stdafx.h"
#include "nrpEngine.h"
#include "nrpScene.h"
#include "nrpVideoConfig.h"
#include "nrpVersion.h"
#include "NrpText.h"
#include "nrpText.h"
#include "nrpMainScene.h"
#include "nrpLoginScene.h"
#include "nrpAppEventer.h"
#include "nrpGUIEnvironment.h"
#include "nrpConsole.h"
#include "nrpScript.h"
//////////////////////////////////////////////////////////////////////////
#include <irrlicht.h>
#include <cassert>

using namespace irr;
using namespace nrp;
//////////////////////////////////////////////////////////////////////////
static CNrpEngine * global_app_instance = 0;

#define NOT_EVENTRECEIVER NULL
//////////////////////////////////////////////////////////////////////////

nrp::CLASS_NAME CLASS_NRPENGINE( L"CNrpEngine" );


void CNrpEngine::Run() //создание нерпы
{
	try
	{		
		CNrpScript::Instance().DoFile( "scripts/initApp.lua" );

		terminated_ = false;											//флаг завершения работы
		currentScene_ = NULL;											//текущая сцена
		status_ = STATUS_UNKNOWN;										//флаг завершения работы
		
		// start the game on loginscene
		ActivateScene_( next_scene_ );

		while ( device_->run() )
		{
			device_->setWindowCaption( NrpText( device_->getVideoDriver()->getFPS() ).ToWide() );
			try
			{
				switch( run_state_ )
				{
				case DRAW:
					if( currentScene_ != NULL )
						currentScene_->OnUpdate();					
					break;

				case LOAD_SCENE:
					ActivateScene_( next_scene_ );
					break;

				case CLOSE:
					terminated_ = true;
					break;
				}

				if( terminated_ )
					break;
			}
			catch(...)
			{
				run_state_ = CLOSE;
				//ErrLog(gfx) << all << "Ошибка потока графики" << term;
			}
		}

		if( currentScene_ )
			currentScene_->OnLeave();

		Logger::Stop();
		GetVideoDriver()->removeAllTextures();
		GetSceneManager()->clear();
		GetGuiEnvironment()->clear();
		delete guienv_;
		device_->drop();

		status_ = CREATE_OK;
	}
	catch(...)
	{
		status_ = CREATE_ERR;
	}
}
//////////////////////////////////////////////////////////////////////////

static WNDPROC OldWindowProc;

LRESULT CALLBACK NewWindowProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l)	//функция отлавливает попытки выхода из программы и блокирует их
{
	//Обрабатываем нужное нам сообщение

	if( msg == WM_DESTROY)
		return 0;

	if( msg == WM_QUIT )
		return 0;

	if( msg == WM_CLOSE)
		return 0;
	//Если нам необходимо прекратить дальнейшую обработку сообщения,
	//то выходим и возвращаем ноль
	//Вызываем старую оконную процедуру,
	//которая обработает сообщение.
	return CallWindowProc( OldWindowProc, hWnd, msg, w, l);
}
///////////////////////////////////////////////////////////////////////////

bool CNrpEngine::InitVideo()
{
	INrpConfig& config = CNrpVideoConfig::Instance();

	bool r = config[ FULLSCREEN ];
	device_ = createDevice( video::EDT_OPENGL,
							config[ SCREEN_SIZE ].As< core::dimension2du >(), 
							32, 
							config[ FULLSCREEN ], 
							config[ USE_SHADOWS ], 
							config[ VSYNC ], 
							0 );

	nrpVersion verInfo;

	NrpText newCaption = NrpText(L"GameInc ") + verInfo.ProductVersion();
	device_->setWindowCaption( newCaption.ToWide() );
	device_->getVideoDriver()->setTextureCreationFlag( video::ETCF_ALWAYS_32_BIT, true );

	windowHandle_ = 0;
	windowHandle_ = FindWindowW( NULL, newCaption.ToWide() );							//заменяем обработчик событий
	OldWindowProc = (WNDPROC)SetWindowLongW( windowHandle_, GWL_WNDPROC, (long)NewWindowProc);

	guienv_ = new gui::CNrpGUIEnvironment( device_->getGUIEnvironment(), device_->getCursorControl() );

	device_->setEventReceiver( new CNrpAppEventer( this ) );

	while( device_ == NULL )
		Sleep( 100 );

	return true;
}
//////////////////////////////////////////////////////////////////////////

void CNrpEngine::ActivateScene_( const NrpText& name )
{
	std::map< NrpText, NrpText >::iterator pIter = scenes_.begin();

	for(; pIter != scenes_.end(); pIter++ )
		if( (*pIter).first == name )
		{
			CNrpScript::Instance().DoFile( (*pIter).second.ToWide() );	
			guienv_->getRootGUIElement()->addChild( console_ );
			run_state_ = DRAW;
			return;
		}

	MessageBox( NULL, "Any scene not identificated", "", 0 );
	throw "scene not identificated";
}
//////////////////////////////////////////////////////////////////////////

CNrpEngine& CNrpEngine::Instance()
{
	if( !global_app_instance)
		global_app_instance = new CNrpEngine();

	return *global_app_instance;
}

scene::INrpScene* CNrpEngine::GetCurrentScene() const
{
	return currentScene_;
}
//////////////////////////////////////////////////////////////////////////

void CNrpEngine::CreateScreenShot_() 
{
	assert( GetVideoDriver() != 0 );
	video::IImage* image = GetVideoDriver()->createScreenShot();
	if (!image)
		return;


	char filename[ MAX_PATH +1] = {0};
	SYSTEMTIME systime;
	GetLocalTime( &systime );
	snprintf( filename, MAX_PATH, 
				"capture/screen_%02d_%02d_%02d(%02d_%02d%02d).png", 
				systime.wYear, systime.wMonth, systime.wDay,
				systime.wHour, systime.wMinute, systime.wSecond );

	GetVideoDriver()->writeImageToFile( image, filename);
	image->drop();
}

CNrpEngine::CNrpEngine() : INrpObject( "CNrpEngine", "nrpEngine" )
{
	guienv_ = NULL;
	console_ = NULL;
	device_ = NULL;
}

irr::video::IVideoDriver* CNrpEngine::GetVideoDriver()
{
	return device_->getVideoDriver();
}

irr::scene::ISceneManager* CNrpEngine::GetSceneManager()
{
	return device_->getSceneManager();
}

irr::gui::IGUIEnvironment* CNrpEngine::GetGuiEnvironment()
{
	return (guienv_ != NULL ? guienv_ : device_->getGUIEnvironment());
}

void CNrpEngine::SetStatus( RUNNING_STATUS new_status, void* data )
{
	switch( new_status )
	{
	case LOAD_SCENE:
		next_scene_ = (char*)data;
		run_state_ = LOAD_SCENE;
	break;	  

	case CLOSE:
		run_state_ = CLOSE;
	break;
	}
}

irr::IrrlichtDevice* CNrpEngine::GetDevice()
{
	return device_;
}

bool CNrpEngine::InitConsole()
{
	try
	{  
		console_ = new irr::gui::CNrpConsole( guienv_, NULL, -1, irr::core::recti( 0, 0, 100, 100) );
	}
	catch(...)
	{
		int j=0;
	}

	return (console_ != NULL);
}

void CNrpEngine::InitializeScene( SCENE_CLASS who )
{
	scene::INrpScene* newScene; 
	switch( who ) 
	{
	case LOGIN_SCENE:
		newScene = new scene::CNrpLoginScene();
	break;

	case WORK_SCENE:
		newScene = new scene::CNrpMainScene();
	break;

	default:
		throw "Can't find scene class";
	}

	if( currentScene_ != NULL )
	{
		currentScene_->OnLeave();
		delete currentScene_;
		currentScene_ = NULL;
	}

	if( newScene != NULL )
	{
		currentScene_ = newScene;
		currentScene_->OnEnter();
	}
}

irr::io::IFileSystem* CNrpEngine::GetFileSystem()
{
	return device_->getFileSystem();
}

void CNrpEngine::AddScene( const NrpText& name, const NrpText& fileName )
{
	scenes_[ name ] = fileName;
}

NrpText CNrpEngine::ClassName()
{
	return CLASS_NRPENGINE;
}