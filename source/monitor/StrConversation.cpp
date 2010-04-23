#include "stdafx.h"
#include "StrConversation.h"
#include <stdio.h>
#include <sstream>
#include <cwchar>

namespace nrp
{

std::string ExtractFileDir(const std::string& file_name )
{
	char buffer[ MAX_PATH ] = { 0 };										//заполняем массив нулями
	size_t len = file_name.size();
	for( size_t i = len; i > 0; i-- )										//просматриваем символы строки с конца
	{
		if( file_name[ i ] == '\\' || file_name[ i ] == '/')				//ищем символ начала директории
		{
			memcpy( buffer, file_name.c_str(), i );							//копируем строку до этого символа
			return std::string( buffer );
		}
	}
	
	return std::string("");														//строка не была найдена	
}
//////////////////////////////////////////////////////////////////////////

std::string ExtractFileName( const std::string& file_name)
{
	char buffer[ MAX_PATH ] = { 0 };										//заполняем массив нулями
	size_t len = file_name.size();

	for (size_t i=len; i > 0; i--) 
	{
		if( file_name[ i ] == '\\' || file_name[ i ] == '/' )
		{
			memcpy ( buffer, &(file_name.c_str()[ i + 1 ]), len-i+1 );
			return std::string( buffer );
		}
	}

	return std::string("");
}
//////////////////////////////////////////////////////////////////////////

std::string ExtractFileExt ( const std::string& file_name )
{
	char buffer[ MAX_PATH ] = { 0 };										//заполняем массив нулями
	size_t len = file_name.size();

	for (size_t i = len; i > 0; i--) 
	{
		if( file_name[i] == '.' ) 
		{
			memcpy ( buffer, &(file_name.c_str()[ i ]), len-i+1);
			return std::string( buffer );
		}
	}

	return std::string("");
}
//////////////////////////////////////////////////////////////////////////

void MultiByteToHex( char* ptr_result, char* ptr_str, size_t len )
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

void HexToMultiByte( char* ptr_result, char* ptr_hex )
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

std::wstring StrToWide(const std::string& str)
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

std::string WideToStr(const std::wstring& str)
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

std::string IntToStr( int num )
{
	std::stringstream out;
	out << num;
	return out.str();
}

int WideToInt( const wchar_t* ws )
{
	std::wistringstream win(ws);
	int value;
	return win >> value ? value : 0;
}

int StrToInt( const char* s )
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

float StrToFloat( const char* s )
{
	float result( 0 );
	sscanf_s( s, "%f", &result );
	return result;
}

irr::core::vector3df StrToVector3df( const char* s )
{
	irr::core::vector3df result( 0, 0, 0 );

	sscanf_s( s, "%f,%f,%f", &result.X, &result.Y, &result.Z );

	return result;
}

} //namespace nrp