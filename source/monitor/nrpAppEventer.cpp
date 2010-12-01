/********************************************************************
	Create:	28:1:2010   14:35
	File  :	nrpAppEventer.cpp
	Author:	Dalerank
	
	Desc  :	Обработчик событий 
*********************************************************************/
#include "StdAfx.h"
#include "nrpAppEventer.h"
#include "nrpEngine.h"
#include "NrpConsoleConfig.h"
#include "nrpConsole.h"
#include "nrpScene.h"

#include <irrlicht.h>

using namespace irr;
using namespace nrp;

const nrp::CLASS_NAME CLASS_EVENTRECEIVER( L"CNrpAppEventer" );

CNrpAppEventer::CNrpAppEventer( CNrpEngine* ptr_app ) : INrpObject( CLASS_EVENTRECEIVER, CLASS_EVENTRECEIVER )  /*конструктор */
{
	ptr_app_ = ptr_app;
}

bool CNrpAppEventer::OnEvent( const SEvent& event )
{
	gui::CNrpConsole* ptrCon = ptr_app_->GetConsole();

	try
	{
		switch( event.EventType ) 
		{
		case EET_KEY_INPUT_EVENT:								//события клавиатуры
			if( event.KeyInput.Key == KEY_SNAPSHOT )			//событие получения скриншота проги
			{
				ptr_app_->CreateScreenShot_();
				return true;
			}

			if( nrp::CNrpConsoleConfig::Instance()[ CONSOLE_INIT_KEY ] == (int)event.KeyInput.Char )							//перхват вызова консоли
			{
				if( !event.KeyInput.Control && event.KeyInput.PressedDown && ptrCon )
				{
					gui::IGUIElement* parent = ptrCon->getParent();
					if( parent )
						parent->bringToFront( ptrCon );
					ptrCon->ToggleVisible();
				}

				return true;
			}

			if( !event.KeyInput.Control && event.KeyInput.PressedDown && ptrCon && ptrCon->isVisible() )						//если открыта консоль 				
			{																//вся клава обрабатывается там
				ptrCon->KeyPress( event );
				return true;
			}
			break;

		case EET_GUI_EVENT:									//события пользовательского интерфейса
			{
			}
		break;

		case EET_LOG_TEXT_EVENT:								//события лога движка
			{
				if( ptrCon )
					ptrCon->AppendMessage( event.LogEvent.Text ); //размещение текста события в консоли
				return true;
			}
		break;
		}

		if( ptr_app_->currentScene_ != NULL )						//передача события в текущую сцену
			return ptr_app_->currentScene_->OnEvent( event );
	}
	catch(...)
	{
		return true;
	}	

	return false;
}

nrp::NrpText CNrpAppEventer::ClassName()
{
	return CLASS_EVENTRECEIVER;
}