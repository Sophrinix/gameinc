#ifndef _IrrConsole_Utils_h_
#define _IrrConsole_Utils_h_

#include <irrString.h>
#include <irrArray.h>
#include <ostream>
#include <string>
//=====================================================================================
//! a basic error class
class CNrpConsoleError
{
public:
	CNrpConsoleError(const irr::core::stringw str=L"", const irr::s32 code=-1);
	CNrpConsoleError(const CNrpConsoleError& err);

	virtual ~CNrpConsoleError();
	
	const CNrpConsoleError& operator = (const CNrpConsoleError& err);
	virtual const irr::core::stringw getType() const{ return L"Generic Error"; }
	const irr::core::stringw getMessage() const;
	const irr::s32 getCode() const;
protected:
	void setMessage(const irr::core::stringw str);
	void setCode(const irr::s32 code);
private:
	irr::core::stringw errorMessage;
	irr::s32		   errorCode;
};

//=====================================================================================
class CNrpConsoleParser
{
public:
	CNrpConsoleParser(const irr::core::stringw& line);
	virtual ~CNrpConsoleParser();
	bool parse( irr::core::stringw& cmdName, irr::core::array< irr::core::stringw >& args);
private:
	void shoveTmpString( irr::core::stringw& cmdName, irr::core::array< irr::core::stringw >& args);
	bool handleChar(wchar_t wc, irr::core::stringw& cmdName, irr::core::array< irr::core::stringw >& args);
	void resetTmpString();
	bool isQuoted();
	bool isEscaped();
	bool isNameDone();
	void setQuoted(bool bVal);
	void setEscaped(bool bVal);
	void setNameDone(bool bVal);

	irr::core::stringw cmdLine;
	bool bQuote,bEscape, bNameDone,bShouldAddLast;
	irr::core::stringw tmpString;
};
//=====================================================================================
//! overloaded operator for inserting a wide string to the stl ostream
inline std::ostream& operator<<(std::ostream& os, const std::string& s)
{
	return (os << s.c_str());
}
//=====================================================================================
//! overloaded operator for inserting a wide string to the stl ostream
std::ostream& operator<<(std::ostream& os, const irr::core::stringw& ws);
#endif