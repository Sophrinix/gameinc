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
	std::wstring StrToWide(const std::string& str);
	std::string WideToStr(const std::wstring& str);
	std::string ExtractFileDir( const std::string& file_name );				/*
																			Получение имени папки из полного пути
																			IN file_name - полный путь для анализа
																			RESULT имя папки
																			*/
	std::string ExtractFileName( const std::string& file_name);						/*
																			Получает имя файла из полного пути к файлу
																			IN file_name - полный путь к файлу
																			RESULT имя файла
																			*/
	std::string ExtractFileExt ( const std::string& file_name );					/*
																			Получает расширение из имени файла
																			IN file_name - путь к файлу
																			RESULT строка символов, где будет размещено расширение
																			*/
	void MultiByteToHex( char* ptr_result, char* ptr_str, size_t len );
	void HexToMultiByte( char* ptr_result, char* ptr_hex );
	
	std::string IntToStr( int num );										//перевод числа в строку
	int StrToInt( const char* s );											//перевод строки в число

	//представление строки в виде трехмерного вектора
	irr::core::vector3df StrToVector3df( const char* s );
	
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
	int WideToInt(const wchar_t* ws);
	bool IsNumber( const char* s );

}; //namespace nrp


