#include "stdafx.h"
#include "StrConversation.h"
#include "nrpConsoleUtils.h"

using namespace irr;
using namespace core;

//=====================================================================================

std::ostream& operator<<( std::ostream& os, const std::wstring& ws)
{
	return (os << nrp::conv::ToStr(ws));
}
//=====================================================================================

CNrpConsoleError::CNrpConsoleError(const irr::core::stringw str, const irr::s32 code) : errorMessage(str), errorCode(code)
{
}
//=====================================================================================
CNrpConsoleError::CNrpConsoleError(const CNrpConsoleError& err)
{
	*this = err;
}
//=====================================================================================
CNrpConsoleError::~CNrpConsoleError()
{
}
//=====================================================================================	
const CNrpConsoleError& CNrpConsoleError::operator = (const CNrpConsoleError& err)
{
	setMessage(err.getMessage());
	setCode(err.getCode());
	return *this;
}
//=====================================================================================
const stringw CNrpConsoleError::getMessage() const
{ 
	return errorMessage; 
}
//=====================================================================================
const s32 CNrpConsoleError::getCode() const
{
	return errorCode; 
}
//=====================================================================================
void CNrpConsoleError::setMessage(const irr::core::stringw str)
{
	errorMessage = str;
}
//=====================================================================================
void CNrpConsoleError::setCode(const irr::s32 code)
{
	errorCode = code;
}
//=====================================================================================

CNrpConsoleParser::CNrpConsoleParser(const irr::core::stringw& line) : cmdLine(line)
{
	bShouldAddLast = false;
	bNameDone = false;
	bEscape = false;
	bQuote = false;
}
CNrpConsoleParser::~CNrpConsoleParser()
{
}
bool CNrpConsoleParser::parse(stringw& cmdName, irr::core::array<stringw>& args)
{
	//cout<< "Parsing : [" << cmdLine.c_str() << "]" <<endl;
	static const wchar_t spaceChar = (wchar_t)' ';
	args.clear();
	cmdName = L"";

	if(cmdLine.findFirst(spaceChar) == -1)
	{
		cmdName = cmdLine;
		return true;
	}
	
	setQuoted(false);
	setEscaped(false);
	setNameDone(false);
	bShouldAddLast = true;
	resetTmpString();

	for(size_t x = 0; x < cmdLine.size(); x++)
	{
		if(!handleChar(cmdLine[x],cmdName,args))
		{
			return false;
		}
	}
	if(bShouldAddLast)
	{
		shoveTmpString(cmdLine,args);
	}
	return true;
}
bool CNrpConsoleParser::handleChar(wchar_t wc, stringw& cmdName, array<stringw>& args)
{
	static const wchar_t spaceChar = (wchar_t)' ';
	static const wchar_t escapeChar = (wchar_t)'\\';
	static const wchar_t quoteChar = (wchar_t)'\"';
	if(wc == spaceChar)
	{
		if(!isQuoted())
		{
			shoveTmpString(cmdName,args);
		}
		else
		{
			tmpString += (spaceChar);
			bShouldAddLast = true;
		}
	}
	else if(wc == quoteChar)
	{
		if(isEscaped())
		{
			
			tmpString += quoteChar;
			bShouldAddLast = true;
			setEscaped(false);
		}
		else if(isQuoted())
		{
			shoveTmpString(cmdName,args);
			setQuoted(false);
		}
		else
		{
			setQuoted(true);
		}

	}
	else if(wc == escapeChar)
	{
		if(isEscaped())
		{
			tmpString += escapeChar;
			bShouldAddLast = true;
			setEscaped(false);
		}
		else
		{
			setEscaped(true);
		}
	}
	else
	{
		if(isEscaped())
		{
			return false;
		}
		tmpString += wc;
		bShouldAddLast = true;
	}
	return true;
}
void CNrpConsoleParser::shoveTmpString(stringw& cmdName, array<stringw>& args)
{
	if(bNameDone)
	{
		//cout << "arg : " << tmpString.c_str() <<endl;
		args.push_back(tmpString);
	}
	else 
	{
		//cout <<"name : " << tmpString.c_str() << endl;
		cmdName = tmpString;
		setNameDone(true);
	}
	bShouldAddLast = false;
	resetTmpString();
}
void CNrpConsoleParser::resetTmpString()
{
	tmpString = L"";
}
bool CNrpConsoleParser::isQuoted()
{
	return bQuote;
}
bool CNrpConsoleParser::isEscaped()
{
	return bEscape;
}
bool CNrpConsoleParser::isNameDone()
{
	return bNameDone;
}
void CNrpConsoleParser::setQuoted(bool bVal)
{
	bQuote = bVal;
}
void CNrpConsoleParser::setEscaped(bool bVal)
{
	bEscape = bVal;
}
void CNrpConsoleParser::setNameDone(bool bVal)
{
	bNameDone = bVal;
}
//=====================================================================================