#include "stdafx.h"

#include "nrpCommandDispatcher.h"
#include "nrpConsoleCommand.h"
#include "nrpMessageSink.h"
#include "nrpConsoleUtils.h"
#include "NrpText.h"
#include <exception>

namespace irr
{

namespace core
{

//! constructor
CCommandDispatcher::CCommandDispatcher()
{
}
//=========================================================================================

//! destructor
CCommandDispatcher::~CCommandDispatcher()
{
	deregisterAllCommands();
}
//=========================================================================================

//! register a command (the cmd->getName() should not already be registered else an exception is thrown)
void CCommandDispatcher::RegisterCommand(CNrpConsoleCommand* cmd)
{
	if(cmd)
	{
		NrpText name = cmd->GetName();
		
		if( !hasCommand(name) )
		{
			commands_[name] = cmd;
		}
		else
		{
			NrpText wstr = NrpText("Command ") + name + " is already registered";
			throw CNrpConsoleError( wstr );
		}
	}
}
//=========================================================================================

//! check if a command is already registered
bool CCommandDispatcher::hasCommand(const stringw& cmdName)
{
	return ( commands_.find( cmdName ) != NULL );
}
//=========================================================================================

//! dispatch a command with the name and the supplied args
void CCommandDispatcher::Dispatch(const stringw& cmdName, const array< stringw >& args, CMessageSink* pOutput)
{
	TCommandMap::Node* node = commands_.find(cmdName);

	if( node != NULL )
	{
		try
		{
			node->getValue()->invoke( args, this, pOutput);
			pOutput->AppendMessage( stringw( "" ) );
		}
		catch(CNrpConsoleError& err)
		{
			stringw wstr = "error of type ";
			wstr += err.getType();
			wstr += " in invoking command [";
			wstr += cmdName;
			wstr += "]";
			pOutput->LogError(wstr);
			pOutput->AppendMessage( err.getMessage() );
		}
		catch( std::exception& ex )
		{
			stringw wstr = "error in invoking command [";
			wstr += cmdName;
			wstr += "]";
			pOutput->LogError<stringw>(wstr);
			pOutput->AppendMessage( stringw( ex.what() ) );
		}
	}
	else
	{
		stringw wstr = L"command [";
		wstr += cmdName;
		wstr +=L"] is not a valid command";
		pOutput->LogError<stringw>(wstr);
		pOutput->AppendMessage( stringw( "for a list of messages type \"help\" or \"list\"" ) );
	}
}
//=========================================================================================

//! deregister (remove) a command
void CCommandDispatcher::DeregisterCommand(const core::stringw& cmdName)
{
	TCommandMap::Node* iter = commands_.find(cmdName);

	if( iter != NULL )
	{
		delete iter->getValue();
		commands_.remove( iter->getKey() );
	}
}
//=========================================================================================

//! deregister all commands
void CCommandDispatcher::deregisterAllCommands()
{
	TCommandMap::Iterator iter = commands_.getIterator();
	
	for(; !iter.atEnd(); iter++)
		delete iter->getValue();

	commands_.clear();
}
//=========================================================================================

//! get the list of registered commands
u32  CCommandDispatcher::GetRegisteredCommands( core::array< core::stringw >& cmdNames)
{
	cmdNames.clear();

	TCommandMap::Iterator iter = commands_.getIterator();
	
	for(; !iter.atEnd(); iter++ )
		cmdNames.push_back( iter->getKey() );

	return cmdNames.size();
}
//=========================================================================================

//! print a list of commands
void CCommandDispatcher::printCommandList( CMessageSink* pOutput, bool bUsage )
{
	core::array< core::stringw > cmdNames;

	GetRegisteredCommands(cmdNames);

	if(bUsage)
	{
		for(u32 i = 0; i < cmdNames.size(); i++)
		{
			printCommandUsage(cmdNames[i],pOutput);
			pOutput->AppendMessage( core::stringw( "" ) );
		}
	}
	else
	{
		for(u32 i = 0; i < cmdNames.size(); i++)
			pOutput->AppendMessage( cmdNames[ i ] );
	}
}
//////////////////////////////////////////////////////////////////////////

//! print the command usage
void CCommandDispatcher::printCommandUsage(const core::stringw& cmdName, CMessageSink* pOutput)
{
	CNrpConsoleCommand* cmd = commands_[cmdName];
	
	if(cmd)
	{
		cmd->PrintUsage(pOutput);
	}
}
//=========================================================================================

//! print detailed description
void CCommandDispatcher::printCommandDesc(const core::stringw& cmdName, CMessageSink* pOutput)
{
	CNrpConsoleCommand* cmd = commands_[cmdName];
	if(cmd)
	{
		cmd->PrintDesc(pOutput);
	}
}
//=========================================================================================

}//namespace core

}//namespace irr