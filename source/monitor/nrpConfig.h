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
#include <typeinfo.h>
#include <assert.h>
#include <memory>

#include "INrpObject.h"
#include "SectionNames.h"
#include "OptionNames.h"
#include "nrptext.h"

using namespace irr;

namespace nrp
{

static void CheckClassesType( const NrpText& type1, const NrpText& type2 );

class INrpProperty 
{
public:
	/// Returns RTTI of current instance of Object.
	virtual const NrpText GetType() const = 0;
	virtual void Reset() = 0;
};

template<typename T>
class CNrpProxyProperty : public INrpProperty {
public:
	CNrpProxyProperty(const T& value) : value_(value) {}
	CNrpProxyProperty(const CNrpProxyProperty& copy) : value_(copy.value_){}
	/// Get the stored data
	operator T& () { return value_; }
	/// Get the stored data
	/**
	 * @return the reference to the stored data element
	 */
	T& Get() { return value_; }
	/// Get the stored data
	/**
	 * @return the constant reference to the stored data element
	 */
	const T& Get() const { return value_; }
	virtual const NrpText GetType() const { return typeid(T).name(); }

	INrpProperty& operator=(const CNrpProxyProperty& copy) {
		value_=copy.value_;
		return *this;
	}

	void Reset() {};

	~CNrpProxyProperty(){}
private:
	CNrpProxyProperty(){}
	T value_;
};

class CNrpProperty 
{
public:
	CNrpProperty() {}
	CNrpProperty(const CNrpProperty& copy) : _value(copy._value) {}

	CNrpProperty& operator=(const CNrpProperty& copy) 
	{
		_value=copy._value;
		return *this;
	}

	CNrpProperty(const char* str );
	/// Template for constructing a Value object from any type.  Creates a copy of the object by using its copy constructor.
	template<typename T> CNrpProperty(const T& val) 
	{
		_value = std::auto_ptr< INrpProperty >( new CNrpProxyProperty<T>(val) );
	}

	/// Assigns the Value instance with object of any type by using its copy constructor.
	template<typename T> CNrpProperty& operator=(const T& val);
	/// Assigns the Value instance with object of type std::string constructed from char*.
	/**
	 * Note, that in spite of type of the parameter (const char*) it's std::string which is really
	 * stored in the current Value! Type char* doesn't match the requirements to types which can be
	 * stored inside Value
	 */
	CNrpProperty& operator=(const char* val);

	/// Accessor to the Structure element by its name.
	/**
	 * If current Value is empty, it is initialised as a Structure.
	 * If it is not empty and it's not a Structure, an exception of type Exception arises.
	 * If Structure doesn't contain an entry with specified key, it is created
	 * and initialised with an empty Value.
	 * @param key - unique name (identifier) of the element
	 * @return reference to Value stored in the Structure
	 */
		/// These two casts are necessary only for old gcc 4.1.2 (fc8)
	operator CNrpProperty&() { return *this; }
	operator const CNrpProperty&() const { return *this; }
	/// Makes a copy of the current instance. Returns an instance of Value that references to a newly created copy.

	template<typename T> T& As() 
	{
		if( typeid(T)==typeid(CNrpProperty) ) return *this;
	
		return ((CNrpProxyProperty<T>*)(_value.get()))->Get();
	}

	template<typename T> const T& As() const;

	/// Implicitly convert Value to the specified datatype.
	/**
	 * If a Value contains serialized (not restored) data ( Data::RawObject ), it is
	 * restored first.
	 * @return data stored in the Value. If the Value is empty  or T doesn't match
	 * the type of Value's data, an Exception is thrown.
	 */
	operator bool() const {	return As<bool>(); }
	operator NrpText() const { return As<NrpText>(); }
	operator float() const { return As<float>(); }
	operator int() const { return As<int>(); }

	template<typename T> bool operator ==( const T& a ) const { return As<T>() == a; }
	template<typename T> operator T() const { return As<T>(); }
	template<typename T> operator const T&() const { return As<T>(); }
	template<typename T> operator const T&() { return As<T>(); }
	template<typename T> operator T&() { return As<T>(); }

	template<typename T> CNrpProperty& operator +=( const T& a ) 
	{ 
		((CNrpProxyProperty<T>*)(_value.get()))->Get() += a;
		return *this; 
	}

	NrpText GetType() const;
	/// Safe (no-exception-throwing) Value type checker
	/**
	 * Compares specified type info (template parameter) and type name with stored value type.
	 * @param typeName - serialized type name (e.g. "int", "double", "Structure", etc).
	 * @return true if type info <b>or</b> type name matches, and false if both checks fail.
	 */
	template<typename T>
	bool Is(const NrpText& typeName) const
	{
		assert( typeName.size() );
		if( !_value )
		{
			if (typeid(T) == typeid(void)) return true;

			if (!typeName.size()) return true;

			return false;
		}
		return ( _value->GetType() == typeid(T));
	}

	bool IsNull() const;

	/// Erase current Value
	void Reset() 
	{
		_value.release();// = std::auto_ptr<INrpProperty>();
	}
	
	virtual ~CNrpProperty(){}
private:
	mutable std::auto_ptr<INrpProperty> _value;
};

class INrpConfig : public INrpObject
{
	friend class CNrpConfigLooder;
	typedef core::map< OPTION_NAME, CNrpProperty* > PropertyArray;
public:
	INrpConfig( CLASS_NAME className, SYSTEM_NAME sysName ) : INrpObject( className, sysName )
	{

	}

	INrpConfig(const INrpConfig& copy);

	virtual CNrpProperty& operator[](OPTION_NAME& key);
	
	/// Const accessor to the Structure element by its name.
	virtual const CNrpProperty& operator[](OPTION_NAME& key) const;

	bool IsExist(const NrpText& key) const;
	/// Tests whether the index is in bounds of the Array.

	unsigned Erase(const NrpText& key);
	/// Erase the specified element of the Array

	CNrpProperty& Get( OPTION_NAME& key ) { return operator[](key); }

protected:
	virtual NrpText Save( const NrpText& fileName );
	virtual void Load( const NrpText& fileName );

	void Pop( const NrpText& name );
	
	template< class B > void Push( OPTION_NAME& name, const B& valuel )
	{
		NrpText fName = name.ToLower();

		if( _options.find( fName ) == NULL )
			_options[ fName ] = new CNrpProperty( valuel );
		else
			*_options[ fName ] = valuel;
	}

private:
	//! определение массива свойств
	INrpConfig() : INrpObject( "error", "error" ) {}
	mutable PropertyArray _options;
};

} //namespace nrp