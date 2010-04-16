/********************************************************************
	created:	2010/01/28
	created:	28:1:2010   10:03
	file base:	StrConversation
	file ext:	h
	author:		Dalerank
	
	purpose:	������� ��� ������ �� �������� ANSI � UNICODE
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
																			��������� ����� ����� �� ������� ����
																			IN file_name - ������ ���� ��� �������
																			RESULT ��� �����
																			*/
	std::string ExtractFileName( const std::string& file_name);						/*
																			�������� ��� ����� �� ������� ���� � �����
																			IN file_name - ������ ���� � �����
																			RESULT ��� �����
																			*/
	std::string ExtractFileExt ( const std::string& file_name );					/*
																			�������� ���������� �� ����� �����
																			IN file_name - ���� � �����
																			RESULT ������ ��������, ��� ����� ��������� ����������
																			*/
	void MultiByteToHex( char* ptr_result, char* ptr_str, size_t len );
	void HexToMultiByte( char* ptr_result, char* ptr_hex );
	
	std::string IntToStr( int num );										//������� ����� � ������
	int StrToInt( const char* s );											//������� ������ � �����

	//������������� ������ � ���� ����������� �������
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

	//������� ������� ������ � �����
	int WideToInt(const wchar_t* ws);
	bool IsNumber( const char* s );

}; //namespace nrp


