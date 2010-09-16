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
#include <typeinfo.h>
#include <stdio.h>
#include <algorithm>
#include <assert.h>

#include "INrpObject.h"
#include "IniFile.h"

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

OPTION_NAME SECTION_OPTIONS( "options" );

OPTION_NAME BALANCE("balance");
OPTION_NAME NAME("name");
OPTION_NAME QUALITY( "quality" );
OPTION_NAME TECHTYPE( "techType" );
OPTION_NAME READYWORKPERCENT( "readyWorkPercent" );
OPTION_NAME TECHNUMBER( "techNumber" );
OPTION_NAME USERNUMBER( "userNumber" );
OPTION_NAME COMPANIESNUMBER( "companiesNumber" );
OPTION_NAME COMPANYNAME( "company" );
OPTION_NAME SECTION_PROPERTIES( "properties" );
OPTION_NAME CODEVOLUME( "volumeCode" );
OPTION_NAME COMPONENTLIDER( "componentLider" );
OPTION_NAME STARTDATE( "startDate" );
OPTION_NAME ENDDATE( "endDate" );
OPTION_NAME PARENT( "parent" );
OPTION_NAME PARENTCOMPANY( "parentCompany" ); /*! компания */
OPTION_NAME RENTPRICE( "rentPrice" );
OPTION_NAME NUMBERDAY( "numberDay" );
OPTION_NAME LEVEL( "level" );
OPTION_NAME PRICEINDAY( "priceInDay" ); 
OPTION_NAME GAMENAME( "gameName" );
OPTION_NAME FINISHED( "finished" );
OPTION_NAME FAMOUS( "famous" );
OPTION_NAME GENRETECH( "genreTech" );
OPTION_NAME GENRE_MODULE_NUMBER( "genreModuleNumber" );
OPTION_NAME TECHGROUP( "techGroup" );
OPTION_NAME CODEPASSED( "codePassed" );
OPTION_NAME MONEYONDEVELOP( "moneyDevelop" );
OPTION_NAME PROJECTREADY( "projectReady" );
OPTION_NAME TEXTURENORMAL( "textureNormal" );
OPTION_NAME TEXTUREHOVER( "textureHover" );
OPTION_NAME ADVTECHNUMBER( "advTechNumber" );

void CheckClassesType( const std::string type1, const std::string type2 );

class INrpProperty
{
public:
	virtual std::string GetValueType() = 0;
	virtual ~INrpProperty()
	{

	}
};
typedef std::map< OPTION_NAME, INrpProperty* > PropertyArray;

class INrpConfig;

template< class ValClass > class CNrpProperty : public INrpProperty
{
	friend INrpConfig;
public:
	CNrpProperty( ValClass pValue )
	{
		type_ = typeid( ValClass ).name();
		SetValue( pValue );
	}

	virtual ~CNrpProperty() 
	{ 
	}

	std::string GetValueType() { return type_; }

	ValClass& ToggleValue()
	{
		value_ = !value_;
		return value_;
	}

	ValClass& AddValue( ValClass valuel ) { value_ += valuel; return value_; }

	ValClass& GetValue() 
	{
		CheckClassesType( type_, typeid( ValClass ).name() );
		return value_; 
	}

	void SetValue( ValClass valuel )
	{
		value_ = valuel ;
	}

private:
	std::string type_;
	ValClass value_;

	CNrpProperty() {};
	CNrpProperty( CNrpProperty& ) {};
};

class INrpConfig : public INrpObject

{
protected: 
	//! чтение свойства из конфигурационного файла
	template< class B > B Read_( std::string section, std::string key, B def_value )
	{
		B read_value;
		
		read_value = nrp::IniFile::Read( section, key, def_value, GetValue< std::string >( CONFIG_FILE ) );

		return (B)read_value;
	}

	//! загрузка свойств объекта
	virtual void Load_( char* file_name ) = 0;

public:
	INrpConfig( CLASS_NAME className, SYSTEM_NAME sysName ) : INrpObject( className, sysName )
	{

	}

	virtual ~INrpConfig() 
	{
		PropertyArray::iterator pIter = options_.begin();
		for( ; pIter != options_.end(); pIter++ )
			delete pIter->second;

		options_.clear();
	}

	//! имя класса объекта
	CLASS_NAME GetType() { return ClassName(); }

	PropertyArray& GetProperties() { return options_; }

	void EraseValue( std::string name )
	{
		std::transform( name.begin(), name.end(), name.begin(), tolower );
		PropertyArray::iterator pIter = options_.find( name );

		if( pIter == options_.end() )
		{
#ifdef _DEBUG
			std::string errstr = "erase: bad config param " + name + " \n"; 
			OutputDebugString( errstr.c_str() );
#endif
			throw "error"; 
		}
		else 
		{
			delete pIter->second;
			options_.erase( pIter );
		}
	}

	template< class B > void CreateValue( std::string name, B valuel )
	{
		std::transform( name.begin(), name.end(), name.begin(), tolower );
		if( options_.find( name ) == options_.end() )
			options_[ name ] = new CNrpProperty<B>( valuel );
		else
			SetValue<B>( name, valuel );
	}

	template< class B > B ToggleValue( std::string name, B defValue )
	{
		std::transform( name.begin(), name.end(), name.begin(), tolower );
		PropertyArray::iterator pIter = options_.find( name );
		if( pIter == options_.end() )
		{
			CreateValue<B>( name, defValue );
			return defValue;
		}
		else
			((CNrpProperty<B>*)pIter->second)->ToggleValue();

		return ((CNrpProperty<B>*)pIter->second)->GetValue();
	}

	//! получение свойства объекта
	template< class B > B& GetValue( std::string name ) const
	{
		std::transform( name.begin(), name.end(), name.begin(), tolower );
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
			return ((CNrpProperty<B>*)pIter->second)->GetValue();
	}

	template< class B > B& AddValue( std::string name, B valuel ) const
	{
		std::transform( name.begin(), name.end(), name.begin(), tolower );
		PropertyArray::const_iterator pIter = options_.find( name );
		if( pIter == options_.end() )
		{
#ifdef _DEBUG
			std::string errstr = "dec: bad config param " + name + " \n"; 
			OutputDebugString( errstr.c_str() );
#endif
			throw "error"; 
		}
		else 
			return ((CNrpProperty<B>*)pIter->second)->AddValue( valuel );
	}

	template< class B > void SetValue( std::string name, B valuel ) 
	{ 
		std::transform( name.begin(), name.end(), name.begin(), tolower );
		PropertyArray::iterator pIter = options_.find( name );
		if( pIter == options_.end() )
		{
#ifdef _DEBUG
			std::string text = "write: bad config param " + ClassName() + ":" + name + "\n";
			OutputDebugString( text.c_str() );
			throw text.c_str();
#endif
		}
		else 
			((CNrpProperty<B>*)pIter->second)->SetValue( valuel );
	}

	virtual void Save( std::string scetionName, std::string fileName );
	virtual void Load( std::string sectionName, std::string fileName );

private:
	//! определение массива свойств
	PropertyArray options_;
};

} //namespace nrp