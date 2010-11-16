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

#include <irrMap.h>
#include <irrString.h>
#include <typeinfo.h>
#include <stdio.h>
#include <assert.h>

#include "INrpObject.h"
#include "SectionNames.h"
#include "OptionNames.h"
#include "nrptext.h"

using namespace irr;

namespace nrp
{

void CheckClassesType( const NrpText& type1, const NrpText& type2 );

class INrpProperty
{
public:
	virtual const NrpText& GetValueType() = 0;
	virtual ~INrpProperty()
	{

	}
};

template< class ValClass > 
class CNrpProperty : public INrpProperty
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

	const NrpText& GetValueType() { return type_; }

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
	NrpText type_;
	ValClass value_;

	CNrpProperty() {};
	CNrpProperty( CNrpProperty& ) {};
};

typedef core::map< OPTION_NAME, INrpProperty* > PropertyArray;

class INrpConfig : public INrpObject
{
	friend class CNrpConfigLooder;

public:

	template< class ValueType >
	class ConfigAccess
	{
		// Let map be the only one who can instantiate this class.
		friend class INrpConfig;

	public:

		// Assignment operator. Handles the myTree["Foo"] = 32; situation
		void operator=(const ValueType& value)
		{
			// Just use the Set method, it handles already exist/not exist situation
			_config.SetValue( value );
		}

		// ValueType operator
		operator ValueType()
		{
			return _config.GetValue();
		}

	private:

		ConfigAccess(CNrpProperty<ValueType>& conf) : _config(conf) {}

		ConfigAccess();

		CNrpProperty<ValueType>& _config;
	}; // AccessClass

	INrpConfig( CLASS_NAME className, SYSTEM_NAME sysName ) : INrpObject( className, sysName )
	{

	}

	virtual ~INrpConfig() 
	{
		PropertyArray::Iterator pIter = _options.getIterator();
		for( ; pIter.atEnd(); pIter++ )
			delete pIter->getValue();

		_options.clear();
	}

	template< class T > bool IsValueTypeA( const NrpText& name )
	{
		PropertyArray::Iterator pIter = _options.find( name.ToLower() );

		if( !pIter.atEnd() )
			return ( pIter->getValue()->GetValueType() == typeid( T ).name() );

		return false;
	}

	PropertyArray& GetProperties() { return _options; }

	template< class B > B ToggleValue( const NrpText& name, B defValue )
	{
		PropertyArray::Iterator pIter = _options.find( name.ToLower() );
		if( pIter.atEnd() )
		{
			CreateValue<B>( name, defValue );
			return defValue;
		}
		else
			((CNrpProperty<B>*)pIter->getValue())->ToggleValue();

		return ((CNrpProperty<B>*)pIter->getValue())->GetValue();
	}

	//! получение свойства объекта
	template< class B > B& GetValue( const NrpText& name ) const
	{
		PropertyArray::Iterator pIter = _options.find( name.ToLower() );
		
		if( pIter.atEnd() )
		{
#ifdef _DEBUG
			Log(HW) << L"read: bad config param " << name << term;
#endif
			throw "error"; 
		}
		else 
			return ((CNrpProperty<B>*)pIter->getValue())->GetValue();
	}

/*
	template< class B > 
	CNrpProperty<B> operator[]( OPTION_NAME name )
	{
		name.make_lower();
		PropertyArray::Iterator pIter = _options.find( name );
		
		if( pIter.atEnd() )
		{
#ifdef _DEBUG
			Log(HW) << L"read: bad config param " << name.c_str() << term;
#endif
			throw "error"; 
		}
		else 
			return ConfigAccess((CNrpProperty<B>*)pIter->getValue());
	}
*/

	NrpText& GetString( const NrpText& name )
	{
		return GetValue<NrpText>( name );
	}

	void SetString( const NrpText& name, const NrpText& valuel )
	{
		SetValue<NrpText>( name, valuel );
	}

	template< class B > B& AddValue( const NrpText& name, B valuel ) const
	{
		PropertyArray::Iterator pIter = _options.find( name.ToLower() );
		if( pIter.atEnd() )
		{
#ifdef _DEBUG
			Log(HW) << L"add: bad config param " << name << term;
#endif
			throw "error"; 
		}
		else 
			return ((CNrpProperty<B>*)pIter->getValue())->AddValue( valuel );
	}

	template< class B > void SetValue( const NrpText& name, B valuel ) 
	{ 
		PropertyArray::Iterator pIter = _options.find( name.ToLower() );
		if( pIter.atEnd() )
		{
#ifdef _DEBUG
			Log(HW) << L"write: bad config param " << ObjectTypeName() << L":" << name << term;
#endif
		}
		else 
			((CNrpProperty<B>*)pIter->getValue())->SetValue( valuel );
	}

	bool IsValueExist( NrpText name )
	{
		return ( _options.find( name.ToLower() ) != NULL );
	}

protected:
	virtual NrpText Save( const NrpText& fileName );
	virtual void Load( const NrpText& fileName );

	void EraseValue( NrpText name );

	template< class B > void CreateValue( const NrpText& name, B valuel )
	{
		NrpText fName = name.ToLower();
		if( _options.find( fName ) == NULL )
			_options[ fName ] = new CNrpProperty<B>( valuel );
		else
			SetValue<B>( fName, valuel );
	}

private:
	//! определение массива свойств
	PropertyArray _options;
};

} //namespace nrp