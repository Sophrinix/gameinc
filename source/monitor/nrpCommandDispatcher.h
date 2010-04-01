#ifndef _HEADER_COMMAND_DISPATCHER_
#define _HEADER_COMMAND_DISPATCHER_

#include <irrString.h>
#include <irrArray.h>
#include <irrMap.h>

//! the dispatcher interface
namespace irr
{

namespace core
{
class CNrpConsoleCommand;
class CMessageSink;

class CCommandDispatcher
{
public:
	//! destructor
	virtual ~CCommandDispatcher();
	//! register a command (the cmd->getName() should not already be registered else an exception is thrown)
	void RegisterCommand(CNrpConsoleCommand* cmd);
	//! check if a command is already registered
	bool hasCommand(const stringw& cmdName);
	//! dispatch a command with the name and the supplied args
	void Dispatch(const stringw& cmdName, const array< stringw >& args, CMessageSink* pOutput);
	//! deregister (remove) a command
	void DeregisterCommand(const stringw& cmdName);
	//! get the list of registered commands
	u32  GetRegisteredCommands( array< stringw >& cmdNames);
	//! deregister all commands
	void deregisterAllCommands();

	//! print a list of commands
	void printCommandList( CMessageSink* pOutput, bool bUsage);
	//! print the command usage
	void printCommandUsage(const stringw& cmdName, CMessageSink* pOutput);
	//! print detailed description
	void printCommandDesc(const stringw& cmdName, CMessageSink* pOutput);
protected:
	//! constructor
	CCommandDispatcher();
private:
	//! the map of command name to command pointer
	typedef core::map< stringw, CNrpConsoleCommand* > TCommandMap;
	TCommandMap commands_;
};

}//namespace core

}//namespace irr

#endif