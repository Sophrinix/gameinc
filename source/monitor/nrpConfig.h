/********************************************************************
	created:	2010/01/28
	created:	28:1:2010   9:57
	filename: 	nrpConfig.h
	file base:	nrpConfig
	file ext:	h
	author:		Dalerank
	
	purpose:	Описание свойств объекта
*********************************************************************/
#pragma once

#include <map>
#include <string>

#include "IniFile.h"
#include "INrpObject.h"

namespace nrp
{

typedef const std::string OPTION_NAME;
typedef const std::string ELEMENT_ALIGN;
OPTION_NAME CONFIG_FILE( "configFile" );

ELEMENT_ALIGN ALIGN_TOP( "top" );
ELEMENT_ALIGN ALIGN_BOTTOM( "bottom" );
ELEMENT_ALIGN ALIGN_MIDDLE( "middle" );
ELEMENT_ALIGN ALIGN_LEFT( "left" );
ELEMENT_ALIGN ALIGN_RIGTH( "rigth" );
ELEMENT_ALIGN ALIGN_CENTER( "center" );

OPTION_NAME BALANCE("balance");
OPTION_NAME NAME("name");
OPTION_NAME TYPE( "type" );


class INrpConfig : public INrpObject
{
protected:
	//! определение массива свойств
	typedef std::map< OPTION_NAME, LPVOID > PropertyArray;
	PropertyArray options_;	

	//! чтение свойства из конфигурационного файла
	template< class B > LPVOID Read_( std::string section, std::string key, B def_value )
	{
		B read_value;
		
		read_value = nrp::IniFile::Read( section, key, def_value, GetOption< std::string >( CONFIG_FILE ) );

		return (LPVOID)( new B( read_value ) );
	}

	//! загрузка свойств объекта
	virtual void Load_( char* file_name ) = 0;

public:
	INrpConfig( CLASS_NAME className, SYSTEM_NAME sysName ) : INrpObject( className, sysName )
	{

	}

	//! имя класса объекта
	CLASS_NAME GetType() { return ClassName(); }

	//! получение свойства объекта
	template< class B > B& GetOption( std::string name ) const
	{
		PropertyArray::const_iterator pIter = options_.find( name );
		
		if( pIter == options_.end() )
		{
#ifdef _DEBUG
			std::string errstr = "read: bad config param " + name + " \n"; 
			OutputDebugString( errstr.c_str() );
#endif
			throw "error"; 
		}
		else 
			return *(B*)(pIter->second);
	}

	template< class B > void SetOption( std::string name, B valuel ) 
	{ 
		PropertyArray::iterator pIter = options_.find( name );

		if( pIter == options_.end() )
		{
#ifdef _DEBUG
			OutputDebugString( "write: bad config param" );
#endif
			throw "write: bad config param";
		}
		else 
			*(B*)(pIter->second) = valuel; 
	}
};

} //namespace nrp