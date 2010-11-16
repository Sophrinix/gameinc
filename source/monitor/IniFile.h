#pragma once
#include "NrpText.h"
#include <vector3d.h>
#include <rect.h>
#include <irrmap.h>
#include "nrpArrays.h"

namespace nrp
{

typedef irr::core::map< int, int > REQUIRE_MAP;
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
public:			
	IniFile( const NrpText& fileName ); 
	//! ������� ���������� � ���������������� ���� ������������� ��������
	/*! 
		\param section	�������� ������, ��� ����� �������� ��������
		\param key		��� ���������
		\param value	�������� ���������
	*/
	bool Set( const NrpText& section, const NrpText& key, int value );						

    bool Set( const NrpText& section, const NrpText& key, float valuel );						


	//! ������ �� ����������������� ����� ������������� ��������
	/*!
		\param section	�������� ������, ��� ���������� ��������
		\param key		��� ���������
		\param def_value �������� ���������, ������� ����� ����������, ���� �� �� ������
	*/
	int Get( const NrpText& section, const NrpText& key, int def_value);

	//! ������ �� ����������������� ����� ������ ��������
	/*!
		\param section	�������� ������, ��� ���������� ��������
		\param key		��� ���������
		\param def_value �������� ���������, ������� ����� ����������, ���� �� �� ������
	*/
	bool Get( const NrpText& section, const NrpText& key, bool def_value );

	//! ������ �� ����������������� ����� ������
	/*!
		\param section	�������� ������, ��� ���������� ��������
		\param key		��� ���������
		\param def_str �������� ���������, ������� ����� ����������, ���� �� �� ������
	*/
	NrpText Get( const NrpText& section, const NrpText& key, const NrpText& def_str );

	//! ���������� � ���������������� ���� ������
	/*!
		\param section	  �������� ������, ��� ���������� ��������
		\param key		  ��� ���������
		\param str_value  �������� ���������
	*/
	bool Set( const NrpText& section, const NrpText& key, const NrpText& str_value );
	

	//! ���������� � ���������������� ���� �������� ��������������
	/*!
		\param section	  �������� ������, ��� ���������� ��������
		\param key		  ��� ���������
		\param amount     �������� ���������
	*/
	bool Set(  const NrpText& section, const NrpText& key, const irr::core::recti& amount);

	//! ������ �� ����������������� ����� �������� ��������������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value �������� ���������, ������� ����� ����������, ���� �� �� ������
	*/
	irr::core::recti Get( const NrpText& section, const NrpText& key, const irr::core::recti& def_value );


	//! ������ �� ����������������� ����� ������������ ��������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	float Get(	const NrpText& section,	const NrpText& key, float def_value );

	//! ������ �� ����������������� ����� �������� ����������� �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	irr::core::vector3df Get( const NrpText& section, const NrpText& key, const irr::core::vector3df& def_value );

	//! ���������� � ���������������� ���� �������� ����������� �������
	/*!
		\param section	  �������� ������, ��� ���������� ��������
		\param key		  ��� ���������
		\param amount     �������� ���������
	*/
	bool Set( const NrpText& section, const NrpText& key, const irr::core::vector3df& amount);

	//! ������ �� ����������������� ����� �������� ����������(�������������) �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	irr::core::dimension2df Get( const NrpText& section, const NrpText& key, const irr::core::dimension2df& def_value );
	   

	//! ������ �� ����������������� ����� �������� ����������(��������������) �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	irr::core::dimension2di Get( const NrpText& section, const NrpText& key, const irr::core::dimension2di& def_value );
   
	//! ������ �� ����������������� ����� �������� ����������(����������� �����) �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	irr::core::dimension2du Get( const NrpText& section, const NrpText& key, const irr::core::dimension2du& def_value );

	/*****************************************************************************/
   
	bool Set( const NrpText& section, const NrpText& key, const SYSTEMTIME& amount );

	SYSTEMTIME Get( const NrpText& section, const NrpText& key, const SYSTEMTIME& def_value );

	void Get( const NrpText& sectionName, REQUIRE_MAP& mapt );

	void Get( const NrpText& section, NrpText (*func)(int), u32 maxNum, STRINGS& art );

	void Set( const NrpText& section,  NrpText (*func)(int), const STRINGS& art );

	void Set( const NrpText& section, const REQUIRE_MAP& rmap );

	template< class T > 
	void SetMap( const NrpText& section, T& mapT, NrpText (*func)(int), const NrpText& param )
	{
		T::Iterator pIter = mapT.getIterator();
		for( int i=0; !pIter.atEnd(); pIter++, i++ )
			Set( section, func( i ), pIter->getValue()->GetString( param ) );
	}

	template< class T > 
	void SetArray( const NrpText& section, T& arrayT, NrpText (*func)(int), const NrpText& param, bool useType=false )
	{
		for( u32 i=0; i < arrayT.size(); i++ )
		{
			Set( section, func( i ), arrayT[ i ]->GetString( param ) );
			if( useType )
				Set( section, CreateKeyType( i ), arrayT[ i ]->ObjectTypeName() ); 
		}
	}
};

} //end namespace nrp