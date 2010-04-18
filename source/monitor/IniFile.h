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
	
	Класс реализует методы доступа к файлам настроек 
	и преобразование прочитанных значений к заданному типу
*/
class IniFile
{
public:			
	
	//! Функция записывает в конфигурационный файл целочисленное значение
	/*! 
		\param section	название секции, где будет размещен параметр
		\param key		имя параметра
		\param value	значение параметра
		\param file_name имя конфигурационного файла для записи
	*/
	static BOOL Write( const std::string& section,						
					   const std::string& key,							
					   const int& value,								
					   const std::string& file_name);						

	static void Write( const std::string& section,						
						const std::string& key,							
						const float& valuel,								
						const std::string& file_name);						


	//! Читает из конфигурационного файла целочисленное значение
	/*!
		\param section	название секции, где расположен параметр
		\param key		имя параметра
		\param def_value значение параметра, которое будет возвращено, если он не найден
		\param file_name имя конфигурационного файла
	*/
	static int Read(   const std::string& section, 
					   const std::string& key, 
					   const int& def_value, 
					   const std::string& file_name);

	//! Читает из конфигурационного файла булево значение
	/*!
		\param section	название секции, где расположен параметр
		\param key		имя параметра
		\param def_value значение параметра, которое будет возвращено, если он не найден
		\param file_name имя конфигурационного файла
	*/
	static bool Read(  const std::string& section, 
					   const std::string& key, 
					   const bool& def_value, 
					   const std::string& file_name );

	//! Читает из конфигурационного файла строку
	/*!
		\param section	название секции, где расположен параметр
		\param key		имя параметра
		\param def_str значение параметра, которое будет возвращено, если он не найден
		\param file_name имя конфигурационного файла
	*/
	static std::string Read( const std::string& section, 
							 const std::string& key, 
							 const std::string& def_str, 
							 const std::string& file_name );

	//! Записывает в конфигурационный файл строку
	/*!
		\param section	  название секции, где расположен параметр
		\param key		  имя параметра
		\param str_value  значение параметра
		\param file_name  имя конфигурационного файла
	*/
	static void Write(	const std::string& section, 
						const std::string& key, 
						const std::string& str_value, 
						const std::string& file_name );
	

	//! Записывает в конфигурационный файл описание прямоугольника
	/*!
		\param section	  название секции, где расположен параметр
		\param key		  имя параметра
		\param amount     значение параметра
		\param file_name  имя конфигурационного файла
	*/
	static void Write(  const std::string& section, 
						const std::string& key, 
						const irr::core::recti& amount, 
						const std::string& file_name);

	//! Читает из конфигурационного файла описание прямоугольника
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение параметра, которое будет возвращено, если он не найден
		\param file_name имя конфигурационного файла
	*/
	static irr::core::recti Read( const std::string& section, 
								  const std::string& key, 
								  const irr::core::recti& def_value, 
								  const std::string& file_name );


	//! Читает из конфигурационного файла вещественное значение
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
		\param file_name имя конфигурационного файла
	*/
	static float Read(	const std::string& section,
						const std::string& key,
						const float& def_value,
						const std::string& file_name );

	//! Читает из конфигурационного файла описание трехмерного вектора
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
		\param file_name имя конфигурационного файла
	*/
	static irr::core::vector3df Read(   const std::string& section, 
										const std::string& key, 
										const irr::core::vector3df& def_value, 
										const std::string& file_name );

	//! Записывает в конфигурационный файл описание трехмерного вектора
	/*!
		\param section	  название секции, где расположен параметр
		\param key		  имя параметра
		\param amount     значение параметра
		\param file_name  имя конфигурационного файла
	*/
	static void Write( const std::string& section, 
					   const std::string& key, 
					   const irr::core::vector3df& amount, 
					   const std::string& file_name);

	//! Читает из конфигурационного файла описание двумерного(вещественного) вектора
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
		\param file_name имя конфигурационного файла
	*/
	static irr::core::dimension2df Read( const std::string& section, 
										 const std::string& key, 
										 const irr::core::dimension2df& def_value, 
										 const std::string& file_name );
	   

	//! Читает из конфигурационного файла описание двумерного(целочисленного) вектора
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
		\param file_name имя конфигурационного файла
	*/
	static irr::core::dimension2di Read( const std::string& section, 
										 const std::string& key, 
										 const irr::core::dimension2di& def_value, 
										 const std::string& file_name );
   
	//! Читает из конфигурационного файла описание двумерного(натуральные числа) вектора
	/*!
		\param section	 название секции, где расположен параметр
		\param key		 имя параметра
		\param def_value значение, которое будет возвращено, если параметр не найден
		\param file_name имя конфигурационного файла
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