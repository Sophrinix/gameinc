#include "stdafx.h"
#include "NrpText.h"
#include <stdio.h>
#include <sstream>
#include <cwchar>
#include <memory>
#include <assert.h>
#include "NrpTranslate.h"

namespace nrp
{ 

NrpText::NrpText() : stringw(L"")
{
	_append = "";
}

NrpText NrpText::FromHex( const char* str )
{
	NrpText outText = L"";
	size_t len = strlen( str );
	for( size_t pos=0; pos < len; pos+=3 )
	{
		int k=0;
		sscanf_s( str + pos, "%02x ", &k  );
		outText.append( k );
	}

	return outText;
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

NrpText NrpText::Replace( NrpText src, NrpText dst ) const
{
    stringw tmp( *this );
    return tmp.replace( src, dst );
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

NrpText Num2HexStr( NrpText pref, NrpText pstf, wchar_t num )
{
    const wchar_t *hlookup = L"0123456789ABCDEF"; 

    if( num < 0) num *= -1;

    pref.append( hlookup[(num >> 4) & 0x0f] );
    pref.append( hlookup[num & 0x0f] );
    pref.append( pstf );

    return pref;
}

NrpText NrpText::ToUtf8( const NrpText& pref, const NrpText& pstf )
{
    NrpText codeStr( *this );
    NrpText outText;
    wchar_t glyph;
    for( u32 pos=0; pos < codeStr.size(); pos++ )
    {
        glyph = codeStr[ pos ];
        if( glyph > 128 )
        {
            glyph = 0x10 + ( glyph - L'�' );
            outText += Num2HexStr( pref, pstf, glyph );
        }
        else
            outText += glyph;
    }

    return outText;
}

NrpText NrpText::FromUtf8( const NrpText& text )
{
   NrpText decodeStr;
   for( u32 pos=0; pos < text.size(); pos++ )
       if( text[ pos ] != L'%' )
           decodeStr.append( text[ pos ] ); 
       else
       {
           decodeStr.append( (( text[ pos+1 ] - 0x30 ) << 4) + ( text[ pos + 2 ] - 0x30 ) );
           pos += 2;
       }

   return decodeStr;
}

NrpText NrpText::ToHex()
{
    const wchar_t hexchar[] = L"0123456789abcdef";
    NrpText selfText( c_str() );
    NrpText outText;

    size_t selfSize = selfText.size();
    for( size_t pos=0; pos < selfSize; pos++ )
    {
        outText.append( Num2HexStr( L'\0', L'\0', selfText[ pos ] ) );
        outText.append( pos < selfSize - 1 ? L' ' : L'' );
    }

    return outText;
}

irr::s32 NrpText::find( const NrpText& str )
{
    return stringw::find( const_cast< NrpText& >( str ).c_str() );
}

NrpText NrpText::Translit() const
{
    NrpText outText;
    const wchar_t* str = stringw::c_str();
    for(; *str != 0; str++)
    {
        switch ( *str )
        {
        case L'�': outText.append( L"a" ); break;
        case L'�': outText.append( L"b"); break;
        case L'�': outText.append( L"v"); break;
        case L'�': outText.append( L"g"); break;
        case L'�': outText.append( L"d"); break;
        case L'�': outText.append( L"e"); break;
        case L'�': outText.append( L"ye"); break;
        case L'�': outText.append( L"zh"); break;
        case L'�': outText.append( L"z"); break;
        case L'�': outText.append( L"i"); break;
        case L'�': outText.append( L"y"); break;
        case L'�': outText.append( L"k"); break;
        case L'�': outText.append( L"l"); break;
        case L'�': outText.append( L"m"); break;
        case L'�': outText.append( L"n"); break;
        case L'�': outText.append( L"o"); break;
        case L'�': outText.append( L"p"); break;
        case L'�': outText.append( L"r"); break;
        case L'�': outText.append( L"s"); break;
        case L'�': outText.append( L"t"); break;
        case L'�': outText.append( L"u"); break;
        case L'�': outText.append( L"f"); break;
        case L'�': outText.append( L"ch"); break;
        case L'�': outText.append( L"z"); break;
        case L'�': outText.append( L"ch"); break;
        case L'�': outText.append( L"sh"); break;
        case L'�': outText.append( L"ch"); break;
        case L'�': outText.append( L"''"); break;
        case L'�': outText.append( L"y"); break;
        case L'�': outText.append( L"''"); break;
        case L'�': outText.append( L"e"); break;
        case L'�': outText.append( L"yu"); break;
        case L'�': outText.append( L"ya"); break;
        case L'�': outText.append( L"A"); break;
        case L'�': outText.append( L"B"); break;
        case L'�': outText.append( L"V"); break;
        case L'�': outText.append( L"G"); break;
        case L'�': outText.append( L"D"); break;
        case L'�': outText.append( L"E"); break;
        case L'�': outText.append( L"Ye"); break;
        case L'�': outText.append( L"Zh"); break;
        case L'�': outText.append( L"Z"); break;
        case L'�': outText.append( L"I"); break;
        case L'�': outText.append( L"Y"); break;
        case L'�': outText.append( L"K"); break;
        case L'�': outText.append( L"L"); break;
        case L'�': outText.append( L"M"); break;
        case L'�': outText.append( L"N"); break;
        case L'�': outText.append( L"O"); break;
        case L'�': outText.append( L"P"); break;
        case L'�': outText.append( L"R"); break;
        case L'�': outText.append( L"S"); break;
        case L'�': outText.append( L"T"); break;
        case L'�': outText.append( L"U"); break;
        case L'�': outText.append( L"F"); break;
        case L'�': outText.append( L"Ch"); break;
        case L'�': outText.append( L"Z"); break;
        case L'�': outText.append( L"Ch"); break;
        case L'�': outText.append( L"Sh"); break;
        case L'�': outText.append( L"Ch"); break;
        case L'�': outText.append( L"''"); break;
        case L'�': outText.append( L"Y"); break;
        case L'�': outText.append( L"''"); break;
        case L'�': outText.append( L"E"); break;
        case L'�': outText.append( L"Yu"); break;
        case L'�': outText.append( L"Ya"); break;
        default: outText += *str;
        }
   }

   return outText;
}
} //namespace nrp