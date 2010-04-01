#ifndef _HEADER_CONSOLE_COMMAND_
#define _HEADER_CONSOLE_COMMAND_

#include <irrArray.h>
#include <irrString.h>

namespace irr
{

namespace core
{

class CMessageSink;
class CCommandDispatcher;

//! the command class
class CNrpConsoleCommand
{
public:
	//! the destructor
	virtual ~CNrpConsoleCommand();

	//! invoke the command with supplied args
	virtual bool invoke(const array< stringw >& args, CCommandDispatcher* pDispatcher, CMessageSink* pOutput) = 0;

	//! get command name
	const stringw GetName();
	//! get the usage string for the command
	const stringw GetUsage();
	//! get the description line count
	const u32 GetDescLineCount();
	//! get the desc line at
	const stringw GetDescLine(const u32 index);

	//! print the usage to the message sink
	void PrintUsage(CMessageSink* pOutput);
	//! print the description to the message sink
	void PrintDesc(CMessageSink* pOutput);

protected:
	//! constructor
	CNrpConsoleCommand(const stringw& name);
	//! set usage
	virtual void SetUsage(const stringw& usage);
	//! append a line to the desc array
	virtual void AddDescLine(const stringw& line);
	//! set descrition to ""
	void ClearDescLines();
private:
	//! the name of the command
	stringw command_name_;
	//! the usage string for the command
	stringw command_usage_;
	//! the description lines
	array< stringw > command_declines_;
};

}//namespace core

}//namespace irr

#endif