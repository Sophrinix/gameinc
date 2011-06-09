#ifndef _INCLUDE_ICONSOLE_LUACOMMAND_
#define _INCLUDE_ICONSOLE_LUACOMMAND_

#include "nrpConsoleUtils.h"
#include "nrpConsole.h"
#include "nrpConsoleCommand.h"

namespace irr
{

namespace core
{

class IC_Console_LuaCommand : public CNrpConsoleCommand
{
public:
	IC_Console_LuaCommand( stringw name, stringw desc, int funcRef );
	virtual ~IC_Console_LuaCommand();
	bool invoke(const array< stringw >& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput);

private:
	int _funcRef;
};

}

}

#endif