/********************************************************************
	Create:	28:1:2010   14:35
	File  :	nrpAppEventer.cpp
	Author:	Dalerank
	
	Desc  :	���������� ������� 
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

CNrpAppEventer::CNrpAppEventer( CNrpEngine* ptr_app ) : INrpObject( "CNrpAppEventer", "appEventer" )  /*����������� */
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
		case EET_KEY_INPUT_EVENT:								//������� ����������
			if( event.KeyInput.Key == KEY_SNAPSHOT )			//������� ��������� ��������� �����
			{
				ptr_app_->CreateScreenShot_();
				return true;
			}

			if( event.KeyInput.Char == nrp::CNrpConsoleConfig::Instance().GetValue<int>( CONSOLE_INIT_KEY ) )							//������� ������ �������
			{
				if( !event.KeyInput.Control && event.KeyInput.PressedDown && ptrCon )
					ptrCon->ToggleVisible();

				return true;
			}

			if( !event.KeyInput.Control && event.KeyInput.PressedDown && ptrCon && ptrCon->isVisible() )						//���� ������� ������� 				
			{																//��� ����� �������������� ���
				ptrCon->KeyPress( event );
				return true;
			}
			break;

		case EET_GUI_EVENT:									//������� ����������������� ����������
			{
			}
		break;

		case EET_LOG_TEXT_EVENT:								//������� ���� ������
			{
				if( ptrCon )
					ptrCon->AppendMessage( event.LogEvent.Text ); //���������� ������ ������� � �������
				return true;
			}
		break;
		}

		if( ptr_app_->currentScene_ != NULL )						//�������� ������� � ������� �����
			return ptr_app_->currentScene_->OnEvent( event );
	}
	catch(...)
	{
		return true;
	}	

	return false;
}