/********************************************************************
	created:	2010/01/28
	created:	28:1:2010   10:03
	file base:	NrpText
	file ext:	h
	author:		Dalerank
	
	purpose:	Функции для работы со строками ANSI и UNICODE
*********************************************************************/
#pragma once
#include <sstream>
#include <irrlicht.h>
#include "TimeHelpers.h"

using namespace irr;
using namespace core;

namespace nrp															
{

class NrpText : public stringw
{
public:
	NrpText();
	NrpText( const stringc& str );
	NrpText( const stringw& str );
	NrpText( const char* str );
	NrpText( const wchar_t* str );
	NrpText( const NrpTime& time );
	NrpText( bool amount );
	NrpText( int amount );
	NrpText( float amount );
	NrpText( const vector3df& vec );
	NrpText( const recti& rectangle );
	NrpText( const dimension2di& dim );
	NrpText( const dimension2df& dim );
	NrpText( const dimension2du& dim );

	operator const char* ();
	operator const wchar_t* ();
	operator std::string ();
	
	bool operator == ( const NrpText& ) const;
	bool operator == ( const wchar_t* ) const;
	NrpText& operator = ( const char* );
	NrpText& operator = ( const wchar_t* );
	NrpText& operator = ( const core::stringw& );

	NrpText operator+( const wchar_t* ) const;
	NrpText operator+( const NrpText& ) const;
	NrpText operator+( int amount ) const;

	void Conv2Hex();
	NrpText& FromHex( const char* ptr_hex );

	//представление строки в виде трехмерного вектора
	vector3df ToVector3df();
	dimension2di ToDim2di();
	dimension2df ToDim2df();
	dimension2du ToDim2du();
	NrpText ToLower();
	NrpText ToLower() const;
	recti ToRect();
	
	//перевод широкой строки в число
	bool IsNumber();
	bool IsFloat();
	float ToFloat();
	int ToInt();											//перевод строки в число
	NrpTime ToTime();
	bool ToBool();

	const char* ToStr();
	const wchar_t* ToWide() { return stringw::c_str(); }
	const wchar_t* ToWide() const { return stringw::c_str(); }

	static NrpText LuaString( const NrpText& luaVarName );
	static float LuaNumber( const NrpText& luaVarName );

private:
	void _FromStr( const char* str );
	const wchar_t* c_str() { return stringw::c_str(); }
	stringc _append;
};//end namespace conv

typedef const NrpText OPTION_NAME;

}; //namespace nrp

using nrp::NrpText;