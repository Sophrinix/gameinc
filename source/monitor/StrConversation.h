/********************************************************************
	created:	2010/01/28
	created:	28:1:2010   10:03
	file base:	StrConversation
	file ext:	h
	author:		Dalerank
	
	purpose:	Функции для работы со строками ANSI и UNICODE
*********************************************************************/
#pragma once
#include <string>
#include <sstream>
#include <vector3d.h>

namespace nrp															
{

namespace conv
{
	std::wstring ToWide(const std::string& str);
	
	void ToHex( char* ptr_result, char* ptr_str, size_t len );
	void FromHex( char* ptr_result, char* ptr_hex );
	
	std::string ToStr( int num );										//перевод числа в строку
	std::string ToStr( const std::wstring& str );

	int ToInt( const char* s );											//перевод строки в число
	int ToInt( const wchar_t* ws);

	//представление строки в виде трехмерного вектора
	irr::core::vector3df ToVector3df( const char* s );
	
	template<typename T> std::wstring AnyToWide( T num )
	{
		std::wstringstream wout;
		wout << num;
		return wout.str();
	}

	template<typename T> std::string AnyToStr( T num )
	{
		std::stringstream sout;
		sout << num;
		return sout.str();
	}

	//перевод широкой строки в число
	
	bool IsNumber( const char* s );
	bool IsFloatNumber( const char* s );
	float ToFloat( const char* s );
	SYSTEMTIME ToTime( const char* s );
	bool ToBool( const char* s );

}//end namespace conv

}; //namespace nrp


