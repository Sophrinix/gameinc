#pragma once
#include "NrpText.h"
#include <vector3d.h>
#include <rect.h>
#include <irrmap.h>
#include "nrpArrays.h"
#include "SectionNames.h"

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
	NrpText _fileName;
	IniFile( const IniFile& ) {}
	IniFile() {}
public:			
	IniFile( const NrpText& fileName ); 
	//! ������� ���������� � ���������������� ���� ������������� ��������
	/*! 
		\param section	�������� ������, ��� ����� �������� ��������
		\param key		��� ���������
		\param value	�������� ���������
	*/
	bool Set( SECTION_NAME& section, const NrpText& key, int value );						

    bool Set( SECTION_NAME& section, const NrpText& key, float valuel );						


	//! ������ �� ����������������� ����� ������������� ��������
	/*!
		\param section	�������� ������, ��� ���������� ��������
		\param key		��� ���������
		\param def_value �������� ���������, ������� ����� ����������, ���� �� �� ������
	*/
	int Get( SECTION_NAME& section, const NrpText& key, int def_value);

	//! ������ �� ����������������� ����� ������ ��������
	/*!
		\param section	�������� ������, ��� ���������� ��������
		\param key		��� ���������
		\param def_value �������� ���������, ������� ����� ����������, ���� �� �� ������
	*/
	bool Get( SECTION_NAME& section, const NrpText& key, bool def_value );

	//! ������ �� ����������������� ����� ������
	/*!
		\param section	�������� ������, ��� ���������� ��������
		\param key		��� ���������
		\param def_str �������� ���������, ������� ����� ����������, ���� �� �� ������
	*/
	NrpText Get( SECTION_NAME& section, const NrpText& key, const NrpText& def_str );

	//! ���������� � ���������������� ���� ������
	/*!
		\param section	  �������� ������, ��� ���������� ��������
		\param key		  ��� ���������
		\param str_value  �������� ���������
	*/
	bool Set( SECTION_NAME& section, const NrpText& key, const NrpText& str_value );
	

	//! ���������� � ���������������� ���� �������� ��������������
	/*!
		\param section	  �������� ������, ��� ���������� ��������
		\param key		  ��� ���������
		\param amount     �������� ���������
	*/
	bool Set( SECTION_NAME& section, const NrpText& key, const irr::core::recti& amount);

	//! ������ �� ����������������� ����� �������� ��������������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value �������� ���������, ������� ����� ����������, ���� �� �� ������
	*/
	irr::core::recti Get( SECTION_NAME& section, const NrpText& key, const irr::core::recti& def_value );


	//! ������ �� ����������������� ����� ������������ ��������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	float Get( SECTION_NAME& section,	const NrpText& key, float def_value );

	//! ������ �� ����������������� ����� �������� ����������� �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	irr::core::vector3df Get( SECTION_NAME& section, const NrpText& key, const irr::core::vector3df& def_value );

	//! ���������� � ���������������� ���� �������� ����������� �������
	/*!
		\param section	  �������� ������, ��� ���������� ��������
		\param key		  ��� ���������
		\param amount     �������� ���������
	*/
	bool Set( SECTION_NAME& section, const NrpText& key, const irr::core::vector3df& amount);

	//! ������ �� ����������������� ����� �������� ����������(�������������) �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	irr::core::dimension2df Get( SECTION_NAME& section, const NrpText& key, const irr::core::dimension2df& def_value );
	   

	//! ������ �� ����������������� ����� �������� ����������(��������������) �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	irr::core::dimension2di Get( SECTION_NAME& section, const NrpText& key, const irr::core::dimension2di& def_value );
   
	//! ������ �� ����������������� ����� �������� ����������(����������� �����) �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	irr::core::dimension2du Get( SECTION_NAME& section, const NrpText& key, const irr::core::dimension2du& def_value );

	/*****************************************************************************/
   
	bool Set( SECTION_NAME& section, const NrpText& key, const SYSTEMTIME& amount );

	SYSTEMTIME Get( SECTION_NAME& section, const NrpText& key, const SYSTEMTIME& def_value );

	void Get( SECTION_NAME& sectionName, KNOWLEDGE_MAP& mapt );

	void Get( SECTION_NAME& section, NrpText (*func)(int), u32 maxNum, STRINGS& art );

	void Set( SECTION_NAME& section,  NrpText (*func)(int), const STRINGS& art );

	void Set( SECTION_NAME& section, const KNOWLEDGE_MAP& rmap );

	template< class T > 
	void Set( SECTION_NAME& section, T& arrayT, NrpText (*func)(int), const NrpText& param )
	{
		for( u32 i=0; i < arrayT.size(); i++ )
		{
			Set( section, func( i ), arrayT[ i ]->Text( param ) );
			Set( section, CreateKeyType( i ), arrayT[ i ]->ObjectTypeName() ); 
		}
	}

	template < class TARRAY, class T, class OWNER>
	void Get( SECTION_NAME& section, NrpText (*func)(int), 
		int maxVal, TARRAY& arr, T* (OWNER::*Method)( const NrpText& ),
		OWNER* owner )
	{
		for( int i=0; i < maxVal; ++i )
		{
			NrpText name = Get( section, func(i), NrpText("") );
			NrpText type = Get( section, CreateKeyType( i ), NrpText("") );
			assert( type == T::ClassName() );
			T* obj = (owner->*Method)( name );
			assert( obj );
			if( obj )
				arr.push_back( obj );
		}
	}
};

} //end namespace nrp