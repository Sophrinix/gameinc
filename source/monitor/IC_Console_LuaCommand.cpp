#include "StdAfx.h"
#include "IC_Console_LuaCommand.h"
#include "nrpScript.h"

namespace irr
{

namespace core
{

IC_Console_LuaCommand::IC_Console_LuaCommand( stringw name, stringw desc, int funcRef ) 
		: CNrpConsoleCommand(name), _funcRef( funcRef )
{
	SetUsage(name);
	AddDescLine(desc);
}

IC_Console_LuaCommand::~IC_Console_LuaCommand(void)
{
}

bool IC_Console_LuaCommand::invoke( const array< stringw >& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput )
{
	nrp::CNrpScript::Instance().CallFunction( _funcRef, this );

	return true;
}

}

}