#include "stdafx.h"
#include "nrpConsoleCommand.h"
#include "nrpConsoleCmds.h"
#include "StrConversation.h"
#include "nrpScript.h"

using namespace irr;
using namespace core;
using namespace nrp;

IC_Command_ECHO::IC_Command_ECHO() : CNrpConsoleCommand(L"echo")
{
	SetUsage(L"echo <string>");
	AddDescLine(L"Комманда отображает на экране введенный текст");
}
//////////////////////////////////////////////////////////////////////////

IC_Command_ECHO::~IC_Command_ECHO() {}
//////////////////////////////////////////////////////////////////////////

bool IC_Command_ECHO::invoke(const array<stringw>& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput)
{
	if(args.size() > 0)
	{
		stringw wstr = L"";
		for(u32 i = 0; i < args.size(); i++)
		{
			wstr += args[i];
			wstr += L" ";
		}
		pOutput->AppendMessage( wstr );
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////

IC_Command_HELP::IC_Command_HELP() : CNrpConsoleCommand(L"help")
{
	SetUsage(L"help <cmd-name>");
	AddDescLine(L"Команда отображает на экране справку для введенной команды");
 }
//////////////////////////////////////////////////////////////////////////

IC_Command_HELP::~IC_Command_HELP()
{
}
//////////////////////////////////////////////////////////////////////////

bool IC_Command_HELP::invoke(const array<stringw>& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput)
{
	if(args.size() == 0)
	{
		pDispatcher->printCommandList( pOutput, true );
	}
	else
	{
		stringw  wstr = args[0];
		for(u32 i = 1; i < args.size(); i++)
		{
			wstr += L" ";
			wstr += args[i];
		}

		if(pDispatcher->hasCommand(wstr))
		{
			pDispatcher->printCommandDesc(wstr,pOutput);
		}
		else
		{
			stringw msg = "Комманда не обнаружена ";
			msg+= wstr;
			pOutput->AppendMessage( msg );
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////

IC_Command_LIST::IC_Command_LIST(): CNrpConsoleCommand(L"list")
{
	SetUsage("list <detailed>");
	AddDescLine("Выводит на экран список доступных комманд");
}
//////////////////////////////////////////////////////////////////////////

IC_Command_LIST::~IC_Command_LIST() {}
//////////////////////////////////////////////////////////////////////////

bool IC_Command_LIST::invoke(const array<stringw>& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput)
{
	pDispatcher->printCommandList(pOutput, args.size() > 0);
	return true;
}
///////////////////////////////////////////////////////////////////////////

IC_Command_INFO::IC_Command_INFO(irr::IrrlichtDevice *pDevice) 
					   : CNrpConsoleCommand("info"),device(pDevice)
{
	SetUsage("info");
	AddDescLine("Команда выводит данные о параметрах работы");
	AddDescLine("-system программные и аппаратные средства");
	AddDescLine("-objlist состояние массива системных объектов");
}
//////////////////////////////////////////////////////////////////////////

IC_Command_INFO::~IC_Command_INFO()
{
	device = NULL;
}
//////////////////////////////////////////////////////////////////////////

bool IC_Command_INFO::invoke(const array<stringw>& args, 
									CCommandDispatcher* pDispatcher, 
									CMessageSink* pOutput)
{
	if( args.size() == 0 )
	{
		pOutput->AppendMessage( L"Нет параметров, используйте команду help info" );
		return true;
	}

	if( args[ 0 ] == L"-system" && device)
	{
		irr::video::IVideoDriver* driver = device->getVideoDriver();
		stringw wstr = L"Видео движок: Irrlicht-";
		wstr+= device->getVersion();
		pOutput->AppendMessage( wstr );

		wstr = L"Драйвер видео: ";
		wstr+= device->getVideoDriver()->getName();
		pOutput->AppendMessage(wstr);

		wstr = L"Операционная система : ";
		wstr+= device->getOSOperator()->getOperationSystemVersion();
		pOutput->AppendMessage(wstr);

		u32 usemem, totalmem;
		device->getOSOperator()->getSystemMemory( &totalmem, &usemem );
		wstr = stringw( L"Память: Всего " ) +  stringw( totalmem / 1024.f ) + stringw( " Mb" );
		pOutput->AppendMessage(wstr);

		wstr = stringw( L"Память: Доступно-" ) +  stringw( usemem / 1024.f ) + stringw( " Mb" );
		pOutput->AppendMessage( wstr );

		u32 speedmhz;
		device->getOSOperator()->getProcessorSpeedMHz( &speedmhz );
		wstr = stringw( L"Процессор: " ) + stringw( speedmhz );

		return true;
	}

	if( args[ 0 ] == L"-objlist" )
	{
		std::vector< std::string > strings;
		GGetListSystemObject( strings );

		for( size_t cnt=0; cnt < strings.size(); cnt++ )
			 pOutput->AppendMessage( StrToWide( strings[ cnt ] ).c_str() );


		pOutput->AppendMessage( StrToWide( "Size of objlist = " + IntToStr( strings.size() ) ).c_str() );

		return true;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////

IC_Command_CLS::IC_Command_CLS() : CNrpConsoleCommand(L"cls")
{
	SetUsage("cls");
	AddDescLine(L"Очистка экрана консоли");
}
//////////////////////////////////////////////////////////////////////////

IC_Command_CLS::~IC_Command_CLS() {}
//////////////////////////////////////////////////////////////////////////

bool IC_Command_CLS::invoke(const array< stringw >& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput)
{
	pOutput->ClearMessages();
	return true;
}
//////////////////////////////////////////////////////////////////////////

irr::core::IC_Command_SCRIPT::IC_Command_SCRIPT(): CNrpConsoleCommand(L"script")
{

}

irr::core::IC_Command_SCRIPT::~IC_Command_SCRIPT() {}

bool irr::core::IC_Command_SCRIPT::invoke( const array< stringw >& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput )
{
	if(args.size() == 0)
	{
		
	}
	else
	{
		if( args[ 0 ] == L"-reload" )
		{
			nrp::CNrpScript::Instance().Reload();
		}
		else if( args[ 0 ] == L"-so" )
		{ 
			std::string optionName = WideToStr( args[ 1 ].c_str() );
			if( args[ 2 ] == L"true" || args[ 2 ] == L"false" )
			{
				bool val = (args[ 2 ] == L"true");
				nrp::CNrpScript::Instance().SetValue<bool>( optionName, val );
			}
		}
		else if( args[ 0 ] == L"-show" )
		{
			nrp::CNrpScript& sc = nrp::CNrpScript::Instance();
			std::string text = SHOW_CALL_FUNCTION_NAME + std::string( sc.GetValue<bool>( SHOW_CALL_FUNCTION_NAME ) ? "=true" : "=false" );
			pOutput->AppendMessage( StrToWide( text ).c_str() );
			text = LOAD_FUNCTIONS_FILENAME + "=" + sc.GetValue<std::string>( LOAD_FUNCTIONS_FILENAME );
			pOutput->AppendMessage( StrToWide( text ).c_str() );
		}
	}

	return true;
}

irr::core::IC_Command_UPDATE::IC_Command_UPDATE(): CNrpConsoleCommand(L"update")
{

}

irr::core::IC_Command_UPDATE::~IC_Command_UPDATE() {}

bool irr::core::IC_Command_UPDATE::invoke( const array< stringw >& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput )
{
	if(args.size() == 0)
	{
		stringw wstr = L"Укажите путь к файлу update-rif-xxxx.exe";
		pOutput->AppendMessage( wstr );
	}
	else
	{
		stringw wstr = L"Функция не реализована";
		pOutput->AppendMessage( wstr );
	}

	return true;
}

irr::core::IC_Command_ACTIONDATA::IC_Command_ACTIONDATA(): CNrpConsoleCommand(L"data")
{

}

irr::core::IC_Command_ACTIONDATA::~IC_Command_ACTIONDATA() {}

bool irr::core::IC_Command_ACTIONDATA::invoke( const array< stringw >& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput )
{
	if(args.size() == 0)
	{
		stringw wstr = L"Доступные параметры: ";
		pOutput->AppendMessage( wstr );

		wstr = "-rand [-snr X] [-ch Y] [-dt Z]: добавление произвольных данных к сонару";
		pOutput->AppendMessage( wstr );

		wstr = "-gold [-snr X] [-ch Y] [-dt Z]: добавление золотой серии данных к сонару";
		pOutput->AppendMessage( wstr );

		wstr = "-gold_ti [-snr X] [-ch Y]: добавление золотой трассовой серии к сонару";
		pOutput->AppendMessage( wstr );
	}
	else
	{
		
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////

IC_Command_SONAR::IC_Command_SONAR() : CNrpConsoleCommand(L"sonar")
{
	SetUsage("sonar");
	AddDescLine(L"Работа с блоками ПОГУ");
}

IC_Command_SONAR::~IC_Command_SONAR() {}

bool IC_Command_SONAR::invoke(const array< stringw >& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput)
{
	return true;
}
//////////////////////////////////////////////////////////////////////////