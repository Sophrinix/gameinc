#ifndef _IrrConsole_Default_Cmds_h_
#define _IrrConsole_Default_Cmds_h_

#include "nrpConsoleUtils.h"
#include "nrpConsole.h"
#include "nrpConsoleCommand.h"

namespace irr
{

namespace core
{

//! Отображает введенный текст
class IC_Command_ECHO : public CNrpConsoleCommand
{
public:
	IC_Command_ECHO();
	virtual ~IC_Command_ECHO();
	bool invoke(const array< stringw >& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput);
};

//! Отображает помощь по указанной команде
class IC_Command_HELP : public CNrpConsoleCommand
{
public:
	IC_Command_HELP();
	virtual ~IC_Command_HELP();
	bool invoke(const array< stringw >& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput);
};

class IC_Command_LIST : public CNrpConsoleCommand
{
public:
	IC_Command_LIST();
	virtual ~IC_Command_LIST();
	bool invoke(const array< stringw>& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput);
};

class IC_Command_INFO : public CNrpConsoleCommand
{
public:
	IC_Command_INFO(irr::IrrlichtDevice *pDevice);
	virtual ~IC_Command_INFO();
	bool invoke(const array< stringw>& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput);
private:
	irr::IrrlichtDevice *device;
};

class IC_Command_CLS : public CNrpConsoleCommand
{
public:
	IC_Command_CLS();
	virtual ~IC_Command_CLS();
	bool invoke(const array< stringw >& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput);

};

class IC_Command_UPDATE : public CNrpConsoleCommand
{
public:
	IC_Command_UPDATE();
	virtual ~IC_Command_UPDATE();
	bool invoke(const array< stringw >& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput);

};

class IC_Command_SCRIPT : public CNrpConsoleCommand
{
public:
	IC_Command_SCRIPT();
	virtual ~IC_Command_SCRIPT();
	bool invoke(const array< stringw >& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput);

};

}//namespace core

}//namespace irr
#endif