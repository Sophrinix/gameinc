#include "stdafx.h"
#include "nrpConsoleCommand.h"
#include "nrpConsoleCmds.h"
#include "nrptext.h"
#include "nrpScript.h"

using namespace irr;
using namespace core;
using namespace nrp;

IC_Command_ECHO::IC_Command_ECHO() : CNrpConsoleCommand(L"echo")
{
	SetUsage(L"echo <string>");
	AddDescLine(L"�������� ���������� �� ������ ��������� �����");
}
//////////////////////////////////////////////////////////////////////////

IC_Command_ECHO::~IC_Command_ECHO() {}
//////////////////////////////////////////////////////////////////////////

bool IC_Command_ECHO::invoke(const array<stringw>& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput)
{
	if(args.size() > 0)
	{
		NrpText wstr = L"";
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
	AddDescLine(L"������� ���������� �� ������ ������� ��� ��������� �������");
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
		NrpText  wstr = args[0];
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
			NrpText msg = "�������� �� ���������� ";
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
	AddDescLine("������� �� ����� ������ ��������� �������");
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
	AddDescLine("������� ������� ������ � ���������� ������");
	AddDescLine("-system ����������� � ���������� ��������");
	AddDescLine("-objlist ��������� ������� ��������� ��������");
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
		pOutput->AppendMessage( L"��� ����������, ����������� ������� help info" );
		return true;
	}

	if( args[ 0 ] == L"-system" && device)
	{
		irr::video::IVideoDriver* driver = device->getVideoDriver();
		stringw wstr = L"����� ������: Irrlicht-";
		wstr+= device->getVersion();
		pOutput->AppendMessage( wstr );

		wstr = L"������� �����: ";
		wstr+= device->getVideoDriver()->getName();
		pOutput->AppendMessage(wstr);

		wstr = L"������������ ������� : ";
		wstr+= device->getOSOperator()->getOperationSystemVersion();
		pOutput->AppendMessage(wstr);

		u32 usemem, totalmem;
		device->getOSOperator()->getSystemMemory( &totalmem, &usemem );
		wstr = stringw( L"������: ����� " ) +  stringw( totalmem / 1024.f ) + stringw( " Mb" );
		pOutput->AppendMessage(wstr);

		wstr = stringw( L"������: ��������-" ) +  stringw( usemem / 1024.f ) + stringw( " Mb" );
		pOutput->AppendMessage( wstr );

		u32 speedmhz;
		device->getOSOperator()->getProcessorSpeedMHz( &speedmhz );
		wstr = stringw( L"���������: " ) + stringw( speedmhz );

		return true;
	}

	if( args[ 0 ] == L"-objlist" )
	{
		core::array< NrpText > strings;
		//GGetListSystemObject( strings );

		for( size_t cnt=0; cnt < strings.size(); cnt++ )
			pOutput->AppendMessage( strings[ cnt ] );


		pOutput->AppendMessage( stringw( "Size of objlist = " ) + stringw( strings.size() ) );

		return true;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////

IC_Command_CLS::IC_Command_CLS() : CNrpConsoleCommand(L"cls")
{
	SetUsage("cls");
	AddDescLine(L"������� ������ �������");
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
	nrp::CNrpScript& script = nrp::CNrpScript::Instance();
	if(args.size() == 0)
	{
		
	}
	else
	{
		if( args[ 0 ] == L"-reload" )
		{
			NrpText remover( "package.loaded[ \" " );
			remover.append( args[ 1 ] );
			remover.append( " \" ] == false" );
			script.DoString( remover );

			NrpText adder( "IncludeScript( \" " );
			adder.append( args[ 1 ] );
			adder.append( " \" )" );
			script.DoString( adder );
		}
		else if( args[ 0 ] == L"-so" )
		{ 
			if( args[ 2 ] == L"true" || args[ 2 ] == L"false" )
			{
				bool val = (args[ 2 ] == L"true");
				script[ args[ 1 ] ] = val;
			}
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
		stringw wstr = L"������� ���� � ����� update-rif-xxxx.exe";
		pOutput->AppendMessage( wstr );
	}
	else
	{
		stringw wstr = L"������� �� �����������";
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
		stringw wstr = L"��������� ���������: ";
		pOutput->AppendMessage( wstr );

		wstr = "-rand [-snr X] [-ch Y] [-dt Z]: ���������� ������������ ������ � ������";
		pOutput->AppendMessage( wstr );

		wstr = "-gold [-snr X] [-ch Y] [-dt Z]: ���������� ������� ����� ������ � ������";
		pOutput->AppendMessage( wstr );

		wstr = "-gold_ti [-snr X] [-ch Y]: ���������� ������� ��������� ����� � ������";
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
	AddDescLine(L"������ � ������� ����");
}

IC_Command_SONAR::~IC_Command_SONAR() {}

bool IC_Command_SONAR::invoke(const array< stringw >& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput)
{
	return true;
}
//////////////////////////////////////////////////////////////////////////