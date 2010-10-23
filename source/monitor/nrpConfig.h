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
#include "SectionNames.h"
#include "OptionNames.h"

namespace nrp
{

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

template< class ValClass > class CNrpProperty : public INrpProperty
{
	friend class INrpConfig;
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

	template< class T > bool IsValueTypeA( std::string name )
	{
		std::transform( name.begin(), name.end(), name.begin(), tolower );
		PropertyArray::iterator pIter = options_.find( name );

		if( pIter == options_.end() )
			return typeid( T ).name() == pIter->second->GetValueType();

		return false;
	}

	PropertyArray& GetProperties() { return options_; }

	void EraseValue( std::string name )
	{
		std::transform( name.begin(), name.end(), name.begin(), tolower );
		PropertyArray::iterator pIter = options_.find( name );

		if( pIter == options_.end() )
		{
#ifdef _DEBUG
			Log(HW) << "erase: bad config param " << name << term;
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
			Log(HW) << "read: bad config param " << name << term;
#endif
			throw "error"; 
		}
		else 
			return ((CNrpProperty<B>*)pIter->second)->GetValue();
	}

	std::string GetString( const std::string& name )
	{
		return GetValue<std::string>( name );
	}

	void SetString( std::string name, std::string valuel )
	{
		SetValue<std::string>( name, valuel );
	}

	template< class B > B& AddValue( std::string name, B valuel ) const
	{
		std::transform( name.begin(), name.end(), name.begin(), tolower );
		PropertyArray::const_iterator pIter = options_.find( name );
		if( pIter == options_.end() )
		{
#ifdef _DEBUG
			Log(HW) << "add: bad config param " << name << term;
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
			Log(HW) << "write: bad config param " << ObjectTypeName() << ":" << name << term;
#endif
		}
		else 
			((CNrpProperty<B>*)pIter->second)->SetValue( valuel );
	}

protected:
	virtual std::string Save( const std::string& fileName );
	virtual void Load( const std::string& fileName );

	//! чтение свойства из конфигурационного файла
	template< class B > B Read_( std::string section, std::string key, B def_value )
	{
		B read_value;

		read_value = nrp::IniFile::Read( section, key, def_value, GetString( CONFIG_FILE ) );

		return (B)read_value;
	}
private:
	//! определение массива свойств
	PropertyArray options_;
};

} //namespace nrp