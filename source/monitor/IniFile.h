#pragma once
#include "NrpText.h"
#include <vector3d.h>
#include <rect.h>
#include <irrmap.h>
#include "nrpArrays.h"
#include "SectionNames.h"
#include "NativeIniFile.h"
#include "TimeHelpers.h"

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
	NativeIniFile _native;

public:			
	IniFile( const NrpText& fileName ); 
	~IniFile();

	NativeIniFile& GetNative() { return _native; }

	//! ������� ���������� � ���������������� ���� ������������� ��������
	/*! 
		\param section	�������� ������, ��� ����� �������� ��������
		\param key		��� ���������
		\param value	�������� ���������
	*/
	void Set( NrpText section, NrpText key, int valuel );						

    void Set( NrpText section, NrpText key, float valuel );						

	void Save();

	//! ������ �� ����������������� ����� ������������� ��������
	/*!
		\param section	�������� ������, ��� ���������� ��������
		\param key		��� ���������
		\param def_value �������� ���������, ������� ����� ����������, ���� �� �� ������
	*/
	int Get( NrpText section, NrpText key, int def_value);

	//! ������ �� ����������������� ����� ������ ��������
	/*!
		\param section	�������� ������, ��� ���������� ��������
		\param key		��� ���������
		\param def_value �������� ���������, ������� ����� ����������, ���� �� �� ������
	*/
	bool Get( NrpText section, NrpText key, bool def_value );

	//! ������ �� ����������������� ����� ������
	/*!
		\param section	�������� ������, ��� ���������� ��������
		\param key		��� ���������
		\param def_str �������� ���������, ������� ����� ����������, ���� �� �� ������
	*/
	NrpText Get( NrpText section, NrpText key, const NrpText def_str );

	//! ���������� � ���������������� ���� ������
	/*!
		\param section	  �������� ������, ��� ���������� ��������
		\param key		  ��� ���������
		\param str_value  �������� ���������
	*/
	void Set( NrpText section, NrpText key, NrpText str_value );
	

	//! ���������� � ���������������� ���� �������� ��������������
	/*!
		\param section	  �������� ������, ��� ���������� ��������
		\param key		  ��� ���������
		\param amount     �������� ���������
	*/
	void Set( NrpText section, NrpText key, const irr::core::recti& amount);

	//! ������ �� ����������������� ����� �������� ��������������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value �������� ���������, ������� ����� ����������, ���� �� �� ������
	*/
	irr::core::recti Get( NrpText section, NrpText key, const irr::core::recti& def_value );


	//! ������ �� ����������������� ����� ������������ ��������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	float Get( NrpText section, NrpText key, float def_value );

	//! ������ �� ����������������� ����� �������� ����������� �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	irr::core::vector3df Get( NrpText section, NrpText key, const irr::core::vector3df& def_value );

	//! ���������� � ���������������� ���� �������� ����������� �������
	/*!
		\param section	  �������� ������, ��� ���������� ��������
		\param key		  ��� ���������
		\param amount     �������� ���������
	*/
	void Set( NrpText section, NrpText key, const irr::core::vector3df& amount);

	//! ������ �� ����������������� ����� �������� ����������(�������������) �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	irr::core::dimension2df Get( NrpText section, NrpText key, const irr::core::dimension2df& def_value );
	   

	//! ������ �� ����������������� ����� �������� ����������(��������������) �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	irr::core::dimension2di Get( NrpText section, NrpText key, const irr::core::dimension2di& def_value );
   
	//! ������ �� ����������������� ����� �������� ����������(����������� �����) �������
	/*!
		\param section	 �������� ������, ��� ���������� ��������
		\param key		 ��� ���������
		\param def_value ��������, ������� ����� ����������, ���� �������� �� ������
	*/
	irr::core::dimension2du Get( NrpText section, NrpText key, const irr::core::dimension2du& def_value );

	/*****************************************************************************/
   
	void Set( NrpText section, NrpText key, const NrpTime& amount );

	NrpTime Get( NrpText section, NrpText key, const NrpTime& def_value );

	void Get( NrpText sectionName, KNOWLEDGE_MAP& mapt );

	void Get( NrpText section, NrpText (*func)(int), u32 maxNum, STRINGS& art );

	void Set( NrpText section,  NrpText (*func)(int), const STRINGS& art );

	void Set( NrpText section, const KNOWLEDGE_MAP& rmap );

	template< class T > 
	void Set( NrpText section, T& arrayT, NrpText (*func)(int), NrpText param )
	{
		for( u32 i=0; i < arrayT.size(); i++ )
		{
			Set( section, func( i ), arrayT[ i ]->Text( param ) );
			Set( section, CreateKeyType( i ), arrayT[ i ]->ObjectTypeName() ); 
		}
	}

	template < class TARRAY, class T, class OWNER>
	void Get( NrpText section, NrpText (*func)(int), 
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