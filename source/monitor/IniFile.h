#pragma once
#include <string>
#include <vector3d.h>
#include <rect.h>

namespace nrp
{

/*!
	\class   IniFile
	\author  Dalerank
	\version 2.0
	\date    2006-2010
	
	����� ��������� ������ ������� � ������ �������� 
	� �������������� ����������� �������� � ��������� ����
*/
class IniFile
{
public:			
	
	//! ������� ���������� � ���������������� ���� ������������� ��������
	/*! 
		\param section	�������� ������, ��� ����� �������� ��������
		\param key		��� ���������
		\param value	�������� ���������
		\param file_name ��� ����������������� ����� ��� ������
	*/
	static BOOL Write( const std::string& section,						
					   const std::string& key,							
					   const int& value,								
					   const std::string& file_name);						

	static void Write( const std::string& section,						
						const std::string& key,							
						const float& valuel,								
						const std::string& file_name);						


	//! ������ �� ����������������� ����� ������������� ��������
	/*!
		\param section	�������� ������, ��� ���������� ��������
		\param key		��� ���������
		\param def_value �������� ���������, ������� ����� ����������, ���� �� �� ������
		\param file_name ��� ����������������� �����
	*/
	static int Read(   const std::string& section, 
					   const std::string& key, 
					   const int& def_value, 
					   const std::string& file_name);

	//! ������ �� ����������������� ����� ������ ��������
	/*!
		\param section	�������� ������, ��� ���������� ��������
		\param key		��� ���������
		\param def_value �������� ���������, ������� ����� ����������, ���� �� �� ������
		\param file_name ��� ����������������� �����
	*/
	static bool Read(  const std::string& section, 
					   const std::string& key, 
					   const bool& def_value, 
					   const std::string& file_name );

	//! ������ �� ����������������� ����� ������
	/*!
		\param section	�������� ������, ��� ���������� ��������
		\param key		��� ���������
		\param def_str �������� ���������, ������� ����� ����������, ���� �� �� ������
		\param file_name ��� ����������������� �����
	*/
	static std::string Read( const std::string& section, 
							 const std::string& key, 
							 const std::string& def_str, 
							 const std::string& file_name );

	//! ���������� � ���������������� ���� ������
	/*!
		\param section	  �������� ������, ��� ���������� ��������
		\param key		  ��� ���������
		\param str_value  �������� ���������
		\param file_name  ��� ����������������� �����
	*/
	static void Write(	const std::string& section, 
						const std::string& key, 
						const std::string& str_value, 
						const std::string& file_name );
	

	//! ���������� � ���������������� ���� �������� ��������������
	/*!
		\param section	  �������� ������, ��� ���������� ��������
		\param key		  ��� ���������
		\param amount     �������� ���������
		\param file_name  ��� ����������������� �����
	*/
	static void Write(  const std::string& section, 
						const std::string& key, 
						const irr::core::recti& amount, 
						const std::string& file_name);

	//! ������ �� ����������������� ����� �������� ��������������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value �������� ���������, ������� ����� ����������, ���� �� �� ������
		\param file_name ��� ����������������� �����
	*/
	static irr::core::recti Read( const std::string& section, 
								  const std::string& key, 
								  const irr::core::recti& def_value, 
								  const std::string& file_name );


	//! ������ �� ����������������� ����� ������������ ��������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
		\param file_name ��� ����������������� �����
	*/
	static float Read(	const std::string& section,
						const std::string& key,
						const float& def_value,
						const std::string& file_name );

	//! ������ �� ����������������� ����� �������� ����������� �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
		\param file_name ��� ����������������� �����
	*/
	static irr::core::vector3df Read(   const std::string& section, 
										const std::string& key, 
										const irr::core::vector3df& def_value, 
										const std::string& file_name );

	//! ���������� � ���������������� ���� �������� ����������� �������
	/*!
		\param section	  �������� ������, ��� ���������� ��������
		\param key		  ��� ���������
		\param amount     �������� ���������
		\param file_name  ��� ����������������� �����
	*/
	static void Write( const std::string& section, 
					   const std::string& key, 
					   const irr::core::vector3df& amount, 
					   const std::string& file_name);

	//! ������ �� ����������������� ����� �������� ����������(�������������) �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
		\param file_name ��� ����������������� �����
	*/
	static irr::core::dimension2df Read( const std::string& section, 
										 const std::string& key, 
										 const irr::core::dimension2df& def_value, 
										 const std::string& file_name );
	   

	//! ������ �� ����������������� ����� �������� ����������(��������������) �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
		\param file_name ��� ����������������� �����
	*/
	static irr::core::dimension2di Read( const std::string& section, 
										 const std::string& key, 
										 const irr::core::dimension2di& def_value, 
										 const std::string& file_name );
   
	//! ������ �� ����������������� ����� �������� ����������(����������� �����) �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
		\param file_name ��� ����������������� �����
	*/
	static irr::core::dimension2du Read( const std::string& section, 
										 const std::string& key, 
										 const irr::core::dimension2du& def_value, 
										 const std::string& file_name );

	/*****************************************************************************/
   
	static void Write( const std::string& section, 
			  		   const std::string& key, 
					   const SYSTEMTIME& amount,
					   const std::string& fileName );

	static SYSTEMTIME Read( const std::string& section, 
							  const std::string& key, 
							  const SYSTEMTIME& def_value,
							  const std::string& fileName );
};

} //end namespace nrp