#include "stdafx.h"

#include "nrpConsoleCommand.h"
#include "nrpMessageSink.h"
#include "nrpConsoleUtils.h"
#include "NrpText.h"

namespace irr
{
	
namespace core
{

//! constructor
CNrpConsoleCommand::CNrpConsoleCommand(const stringw& name) : command_name_(name)
{

}
//=========================================================================================

//! the destructor
CNrpConsoleCommand::~CNrpConsoleCommand()
{
}
//=========================================================================================

//! get command name
const stringw CNrpConsoleCommand::GetName()
{
	return command_name_;
}
//=========================================================================================

//! get the usage string for the command
const stringw CNrpConsoleCommand::GetUsage()
{
	return command_usage_;
}
//=========================================================================================

//! get the description line count
const irr::u32 CNrpConsoleCommand::GetDescLineCount()
{
	return command_declines_.size();
}
//=========================================================================================

//! get the desc line at
const stringw CNrpConsoleCommand::GetDescLine(const u32 index)
{
	return command_declines_[index];
}
//=========================================================================================

//! print the usage to the message sink
void CNrpConsoleCommand::PrintUsage(CMessageSink* pOutput)
{
	stringw wstr = L"Command : ";
	wstr += GetName();
	pOutput->AppendMessage( wstr );

	wstr = L"usage : ";
	pOutput->AppendMessage( wstr );

	wstr = L" ";
	wstr += GetUsage();
	pOutput->AppendMessage( wstr );

}
//=========================================================================================

//! print the description to the message sink
void CNrpConsoleCommand::PrintDesc(CMessageSink* pOutput)
{
	PrintUsage( pOutput );

	stringw wstr = L"description";
	pOutput->AppendMessage( wstr );

	for(u32 i = 0; i < command_declines_.size(); i++)
	{
		wstr = L" ";
		wstr += command_declines_[i];
		pOutput->AppendMessage( wstr );
	}
}
//=========================================================================================

//! set usage
void CNrpConsoleCommand::SetUsage(const stringw& usage)
{
	command_usage_ = usage;
}
//=========================================================================================

//! append a line to the desc array
void CNrpConsoleCommand::AddDescLine(const stringw& line)
{
	command_declines_.push_back(line);
}
//=========================================================================================

//! set descrition to ""
void CNrpConsoleCommand::ClearDescLines()
{
	command_declines_.clear();
}
//////////////////////////////////////////////////////////////////////////

}//namespace core

}//namesapce irr