#include "stdafx.h"
#include "NrpText.h"
#include <stdio.h>
#include <sstream>
#include <cwchar>
#include <memory>
#include <regex>
#include <assert.h>
#include "NrpTranslate.h"

namespace nrp
{ 

NrpText::NrpText() : stringw(L"")
{
	_append = "";
}

void NrpText::Conv2Hex()
{	
	const wchar_t hexchar[] = L"0123456789abcdef";
	stringw dd = *this;
	stringw::operator=( L"" );

	for( size_t pos=0; pos < dd.size(); pos++ )
	{
		 append( hexchar[ ( dd[ pos ] >> 4 ) & 0xf ] );
		 append( hexchar[ dd[ pos ] & 0xf ] );
		 append( ' ' );
	}
}

NrpText& NrpText::FromHex( const char* str )
{
	*this = L"";
	size_t len = strlen( str );
	for( size_t pos=0; pos < len; pos+=3 )
	{
		int k=0;
		sscanf_s( str + pos, "%02x ", &k  );
		append( k );
	}

	return *this;
}

NrpText::NrpText( const char* str ) : stringw(L"")
{
	_FromStr( str );
}

NrpText::NrpText( const core::recti& rectangle ) : stringw(L"")
{
	append(  rectangle.UpperLeftCorner.X ); append( L"," );
	append(  rectangle.UpperLeftCorner.Y ); append( L"," );
	append(  rectangle.LowerRightCorner.X ); append( L"," );
	append(  rectangle.LowerRightCorner.Y ); 
}

NrpText::NrpText( const dimension2di& dim ) : stringw(L"")
{
	append(  dim.Width ); append( L"," );
	append(  dim.Height );
}

NrpText::NrpText( const dimension2df& dim ) : stringw(L"")
{
	*this += dim.Width; append( L"," );
	*this += dim.Height;
}

NrpText::NrpText( const dimension2du& dim ) : stringw(L"")
{
	append(  dim.Width ); append( L"," );
	append(  dim.Height );
}

NrpText::NrpText( const vector3df& vec ) : stringw(L"")
{
	*this += vec.X; append( L"," );
	*this += vec.Y; append( L"," );
	*this += vec.Z; 
}

NrpText::NrpText( const stringc& str ) : stringw("")
{
	_FromStr( str.c_str() );
}

NrpText::NrpText( int amount ) : stringw( amount )
{

}

NrpText::NrpText( const wchar_t* str ) : stringw( str )
{

}

NrpText::NrpText( float amount ) : stringw( amount )
{ 

}

NrpText::NrpText( const stringw& str ) : stringw( str )
{

}

void NrpText::_FromStr( const char* str )
{
	assert( str != NULL );
	if( str )
	{
		int len = strlen( str ) + 2;
		std::auto_ptr<wchar_t> buf( new wchar_t[ len ] );
		MultiByteToWideChar( CP_ACP, 0, str, len-1, buf.get(), len-1);

		buf.get()[ len - 2 ] = 0;	
		stringw::operator=( buf.get() );
	}
	else
		stringw::operator=( L"" );
}

const char* NrpText::ToStr()
{
	int len = size() + 2;
	std::auto_ptr<char> buf( new char[len] );
	_append = "error_converting";

	BOOL istrue;
	WideCharToMultiByte( CP_ACP, 0, c_str(), len - 1, buf.get(), len - 1, "!non_conv!", &istrue );

	buf.get()[ len - 2 ] = 0;
	_append = buf.get();

	return _append.c_str();
}

int NrpText::ToInt()
{
	return atoi( ToStr() );
}

bool NrpText::IsNumber()
{
	for( size_t pos=0; pos < size(); pos++ )
		if( !core::isdigit( stringw::operator[](pos) ) ) return false;

	return true;
}

bool NrpText::IsFloat()
{
	int pointNum=0;
	for( size_t pos=0; pos < size(); pos++ )
		if( ( core::isdigit( stringw::operator[]( pos ) ) || stringw::operator[]( pos ) == '.' ) && pointNum < 2 )
		{
			if( stringw::operator[]( pos ) == L'.' )
				pointNum++;
		}
		else
			return false;


	return true;	
}

float NrpText::ToFloat()
{
	std::wistringstream win( c_str());
	float value;
	return win >> value ? value : 0;
}

vector3df NrpText::ToVector3df()
{
	irr::core::vector3df result( 0, 0, 0 );
	int ret = swscanf_s( c_str(), L"%f,%f,%f", &result.X, &result.Y, &result.Z );
	assert( ret == 3 );
	return result;
}

dimension2df NrpText::ToDim2df()
{
	dimension2df result( 0.f, 0.f );
	int ret = swscanf_s( c_str(), L"%f,%f", &result.Width, &result.Height );
	assert( ret = 2);
	return result;
}

dimension2di NrpText::ToDim2di()
{
	irr::core::dimension2di result( 0, 0 );
	int ret = swscanf_s( c_str(), L"%d,%d", &result.Width, &result.Height );
	assert( ret == 2 );
	return result;
}

dimension2du NrpText::ToDim2du()
{
	irr::core::dimension2du result( 0, 0 );
	int ret = swscanf_s( c_str(), L"%d,%d", &result.Width, &result.Height );
	assert( ret == 2 );
	return result;
}


recti NrpText::ToRect()
{
	irr::core::recti rct( 0, 0, 0, 0 );

	int ret = swscanf_s( c_str(), L"%d,%d,%d,%d", &rct.UpperLeftCorner.X, &rct.UpperLeftCorner.Y, 
													  	   &rct.LowerRightCorner.X, &rct.LowerRightCorner.Y  );
	assert( ret == 4 );

	return rct;
}

bool NrpText::ToBool()
{
	return (_wcsicmp( c_str(), L"true" ) == 0 || _wcsicmp( c_str(), L"1" ) == 0);
}

NrpText::operator const char*()
{
	return ToStr();
}

NrpText& NrpText::operator =( const char* oth )
{
	_FromStr( oth );
	return *this;
}

NrpText& NrpText::operator=( const core::stringw& a )
{
	stringw::operator=( a );
	return *this;
}

NrpText& NrpText::operator=( const wchar_t* a )
{
	stringw::operator=( a );
	return *this;
}

NrpText::operator std::string()
{
	ToStr();
	return _append.c_str();
}

NrpText NrpText::ToLower() const
{
	stringw ret( *this );
	ret.make_lower();
	return ret;
}

NrpText NrpText::ToLower()
{
	stringw ret( *this );
	ret.make_lower();
	return ret;
}

NrpText::operator const wchar_t*()
{
	return ToWide();
}

NrpText NrpText::operator+( const wchar_t* a ) const
{
	return stringw( *this ) + a;
}

NrpText NrpText::operator+( const NrpText& a ) const
{
	return stringw(*this) + stringw(a);
}

NrpText NrpText::operator+( int amount ) const
{
	return stringw(*this) + stringw(amount);
}

bool NrpText::operator==( const NrpText& a ) const
{
	return stringw(*this) == stringw(a);
}

bool NrpText::operator==( const wchar_t* a ) const
{
	return stringw(*this) == stringw(a);
}

NrpText NrpText::LuaString( const NrpText& luaVarName )
{
	return NrpText( translate::LuaString( const_cast< NrpText& >( luaVarName ).ToStr() ) );
}

float NrpText::LuaNumber( const NrpText& luaVarName )
{
	return static_cast< float>( translate::LuaNumber( luaVarName ) );
}
} //namespace nrp