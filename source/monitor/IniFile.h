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
	
	Класс реализует методы доступа к файлам настроек 
	и преобразование прочитанных значений к заданному типу
*/
class IniFile
{
	NrpText _fileName;
public:			
	IniFile( const NrpText& fileName ); 
	//! Функция записывает в конфигурационный файл целочисленное значение
	/*! 
		\param section	название секции, где будет размещен параметр
		\param key		имя параметра
		\param value	значение параметра
	*/
	bool Set( const NrpText& section, const NrpText& key, int value );						

    bool Set( const NrpText& section, const NrpText& key, float valuel );						


	//! Читает из конфигурационного файла целочисленное значение
	/*!
		\param section	название секции, где расположен параметр
		\param key		имя параметра
		\param def_value значение параметра, которое будет возвращено, если он не найден
	*/
	int Get( const NrpText& section, const NrpText& key, int def_value);

	//! Читает из конфигурационного файла булево значение
	/*!
		\param section	название секции, где расположен параметр
		\param key		имя параметра
		\param def_value значение параметра, которое будет возвращено, если он не найден
	*/
	bool Get( const NrpText& section, const NrpText& key, bool def_value );

	//! Читает из конфигурационного файла строку
	/*!
		\param section	название секции, где расположен параметр
		\param key		имя параметра
		\param def_str значение параметра, которое будет возвращено, если он не найден
	*/
	NrpText Get( const NrpText& section, const NrpText& key, const NrpText& def_str );

	//! Записывает в конфигурационный файл строку
	/*!
		\param section	  название секции, где расположен параметр
		\param key		  имя параметра
		\param str_value  значение параметра
	*/
	bool Set( const NrpText& section, const NrpText& key, const NrpText& str_value );
	

	//! Записывает в конфигурационный файл описание прямоугольника
	/*!
		\param section	  название секции, где расположен параметр
		\param key		  имя параметра
		\param amount     значение параметра
	*/
	bool Set(  const NrpText& section, const NrpText& key, const irr::core::recti& amount);

	//! Читает из конфигурационного файла описание прямоугольника
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение параметра, которое будет возвращено, если он не найден
	*/
	irr::core::recti Get( const NrpText& section, const NrpText& key, const irr::core::recti& def_value );


	//! Читает из конфигурационного файла вещественное значение
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
	*/
	float Get(	const NrpText& section,	const NrpText& key, float def_value );

	//! Читает из конфигурационного файла описание трехмерного вектора
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
	*/
	irr::core::vector3df Get( const NrpText& section, const NrpText& key, const irr::core::vector3df& def_value );

	//! Записывает в конфигурационный файл описание трехмерного вектора
	/*!
		\param section	  название секции, где расположен параметр
		\param key		  имя параметра
		\param amount     значение параметра
	*/
	bool Set( const NrpText& section, const NrpText& key, const irr::core::vector3df& amount);

	//! Читает из конфигурационного файла описание двумерного(вещественного) вектора
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
	*/
	irr::core::dimension2df Get( const NrpText& section, const NrpText& key, const irr::core::dimension2df& def_value );
	   

	//! Читает из конфигурационного файла описание двумерного(целочисленного) вектора
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
	*/
	irr::core::dimension2di Get( const NrpText& section, const NrpText& key, const irr::core::dimension2di& def_value );
   
	//! Читает из конфигурационного файла описание двумерного(натуральные числа) вектора
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
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