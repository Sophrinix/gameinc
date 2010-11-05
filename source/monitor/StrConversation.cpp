#include "stdafx.h"
#include "StrConversation.h"
#include <stdio.h>
#include <sstream>
#include <cwchar>

namespace nrp
{ 

namespace conv
{
	
void ToHex( char* ptr_result, char* ptr_str, size_t len )
{	
	const char hexchar[] = "0123456789abcdef";
	for( size_t pos=0; pos < len; pos++ )
	{
		 ptr_result[ pos * 3 ] = hexchar[ (ptr_str[ pos ] >> 4 ) & 0xf ];
		 ptr_result[ pos * 3 + 1 ] = hexchar[ ptr_str[ pos ] & 0xf ];
		 ptr_result[ pos * 3 + 2 ] = ' ';
	}
}
//////////////////////////////////////////////////////////////////////////

void FromHex( char* ptr_result, char* ptr_hex )
{
	size_t len = strlen( ptr_hex );
	for( size_t pos=0; pos < len; pos+=3 )
	{
		int k=0;
		 sscanf_s( ptr_hex + pos, "%02x ", &k  );
		 ptr_result[ pos / 3 ] = k;
	}
}
//////////////////////////////////////////////////////////////////////////

std::wstring ToWide(const std::string& str)
{
	int len = str.size() + 2;
	wchar_t* buf = new wchar_t[ len ];
	std::wstring wstr = L"error_converting";

	MultiByteToWideChar( CP_ACP, 0, str.c_str(), len-1, buf, len-1);

	buf[ len - 2 ] = 0;
	wstr = buf;

	delete [] buf;
	return wstr;
}
//////////////////////////////////////////////////////////////////////////

std::string ToStr(const std::wstring& str)
{
	int len = str.size() + 2;
	char* buf = new char[len];
	std::string wstr = "error_converting";

	BOOL istrue;
	WideCharToMultiByte( CP_ACP, 0, str.c_str(), len - 1, buf, len - 1, "!non_conv!", &istrue );

	buf[ len - 2 ] = 0;
	wstr = buf;

	delete[] buf;
	return wstr;
}

std::string ToStr( int num )
{
	std::stringstream out;
	out << num;
	return out.str();
}

int ToInt( const wchar_t* ws )
{
	std::wistringstream win(ws);
	int value;
	return win >> value ? value : 0;
}

int ToInt( const char* s )
{
	std::istringstream sti(s);
	int result;
	return sti >> result ? result : 0;
}

bool IsNumber( const char* s )
{
	int pos = 0;
	while( s[ pos ] != 0 )
	{
		if( s[ pos ] < '0' || s[ pos ] > '9' ) return false;
		pos++;
	}

	return true;
}

bool IsFloatNumber( const char* s )
{
	int pointNum = 0;
	int pos = 0;
	while( s[ pos ] != 0 )
		if( (( s[ pos ] >= '0' && s[ pos ] <= '9' ) || s[ pos ] == '.' ) && pointNum < 2 )
		{
			if( s[ pos ] == '.' )
				pointNum++;

			pos++;
		}
		else
			return false;


	return true;	
}

float ToFloat( const char* s )
{
	float result( 0 );
	sscanf_s( s, "%f", &result );
	return result;
}

irr::core::vector3df ToVector3df( const char* s )
{
	irr::core::vector3df result( 0, 0, 0 );

	sscanf_s( s, "%f,%f,%f", &result.X, &result.Y, &result.Z );

	return result;
}

SYSTEMTIME ToTime( const char* s )
{
	SYSTEMTIME time;

	sscanf_s( s, "y=%04d m=%02d d=%02d h=%02d mi=%02d", 
			  &time.wYear, &time.wMonth, &time.wDay, &time.wHour, &time.wMinute );
	time.wSecond = 0;
	time.wMilliseconds = 0;

	return time;	
}

bool ToBool( const char* s )
{
	return (_stricmp( s, "true" ) == 0 || _stricmp( s, "1" ) == 0);
}

} //namespace conv

} //namespace nrp