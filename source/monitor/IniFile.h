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
	
	Класс реализует методы доступа к файлам настроек 
	и преобразование прочитанных значений к заданному типу
*/
class IniFile
{
	NrpText _fileName;
	IniFile( const IniFile& ) {}
	IniFile() {}
public:			
	IniFile( const NrpText& fileName ); 
	//! Функция записывает в конфигурационный файл целочисленное значение
	/*! 
		\param section	название секции, где будет размещен параметр
		\param key		имя параметра
		\param value	значение параметра
	*/
	bool Set( SECTION_NAME& section, const NrpText& key, int value );						

    bool Set( SECTION_NAME& section, const NrpText& key, float valuel );						


	//! Читает из конфигурационного файла целочисленное значение
	/*!
		\param section	название секции, где расположен параметр
		\param key		имя параметра
		\param def_value значение параметра, которое будет возвращено, если он не найден
	*/
	int Get( SECTION_NAME& section, const NrpText& key, int def_value);

	//! Читает из конфигурационного файла булево значение
	/*!
		\param section	название секции, где расположен параметр
		\param key		имя параметра
		\param def_value значение параметра, которое будет возвращено, если он не найден
	*/
	bool Get( SECTION_NAME& section, const NrpText& key, bool def_value );

	//! Читает из конфигурационного файла строку
	/*!
		\param section	название секции, где расположен параметр
		\param key		имя параметра
		\param def_str значение параметра, которое будет возвращено, если он не найден
	*/
	NrpText Get( SECTION_NAME& section, const NrpText& key, const NrpText& def_str );

	//! Записывает в конфигурационный файл строку
	/*!
		\param section	  название секции, где расположен параметр
		\param key		  имя параметра
		\param str_value  значение параметра
	*/
	bool Set( SECTION_NAME& section, const NrpText& key, const NrpText& str_value );
	

	//! Записывает в конфигурационный файл описание прямоугольника
	/*!
		\param section	  название секции, где расположен параметр
		\param key		  имя параметра
		\param amount     значение параметра
	*/
	bool Set( SECTION_NAME& section, const NrpText& key, const irr::core::recti& amount);

	//! Читает из конфигурационного файла описание прямоугольника
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение параметра, которое будет возвращено, если он не найден
	*/
	irr::core::recti Get( SECTION_NAME& section, const NrpText& key, const irr::core::recti& def_value );


	//! Читает из конфигурационного файла вещественное значение
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
	*/
	float Get( SECTION_NAME& section,	const NrpText& key, float def_value );

	//! Читает из конфигурационного файла описание трехмерного вектора
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
	*/
	irr::core::vector3df Get( SECTION_NAME& section, const NrpText& key, const irr::core::vector3df& def_value );

	//! Записывает в конфигурационный файл описание трехмерного вектора
	/*!
		\param section	  название секции, где расположен параметр
		\param key		  имя параметра
		\param amount     значение параметра
	*/
	bool Set( SECTION_NAME& section, const NrpText& key, const irr::core::vector3df& amount);

	//! Читает из конфигурационного файла описание двумерного(вещественного) вектора
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
	*/
	irr::core::dimension2df Get( SECTION_NAME& section, const NrpText& key, const irr::core::dimension2df& def_value );
	   

	//! Читает из конфигурационного файла описание двумерного(целочисленного) вектора
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
	*/
	irr::core::dimension2di Get( SECTION_NAME& section, const NrpText& key, const irr::core::dimension2di& def_value );
   
	//! Читает из конфигурационного файла описание двумерного(натуральные числа) вектора
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
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