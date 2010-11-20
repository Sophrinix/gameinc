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

bool CheckClassesType( const NrpText& type1, const NrpText& type2 );

class IParam 
{
public:
	/// Returns RTTI of current instance of Object.
	virtual const NrpText GetType() const = 0;
	virtual void Reset() = 0;
};

template<typename T>
class NProxyParam : public IParam 
{
public:
	NProxyParam(const T& value) : value_(value) {}
	NProxyParam(const NProxyParam& copy) : value_(copy.value_){}
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

	IParam& operator=(const NProxyParam& copy) 
	{
		value_ = copy.value_;
		return *this;
	}

	IParam& operator+=(const NProxyParam& copy) 
	{
		value_ += copy.value_;
		return *this;
	}

	bool operator<(const NProxyParam& copy) 
	{
		return value_ < copy.value_;
	}

	bool operator==(const NProxyParam& copy) 
	{
		return value_ == copy.value_;
	}

	void Reset() {};

	~NProxyParam(){}
private:
	NProxyParam(){}
	T value_;
};

class NParam 
{
public:
	NParam() {}
	NParam(const NParam& copy) : _value(copy._value) {}

	NParam& operator=(const NParam& copy) 
	{
		assert( CheckClassesType( copy.GetType(), GetType() ) );
		_value=copy._value;
		return *this;
	}

	NParam(const char* str );
	/// Template for constructing a Value object from any type.  Creates a copy of the object by using its copy constructor.
	template<typename T> NParam(const T& val) 
	{
		_value = std::auto_ptr< IParam >( new NProxyParam<T>(val) );
	}

	/// Assigns the Value instance with object of any type by using its copy constructor.
	template<typename T> NParam& operator=(const T& val)
	{
		_value = std::auto_ptr< IParam >( new NProxyParam<T>(val) );
		return *this;
	}
	/// Assigns the Value instance with object of type std::string constructed from char*.
	/**
	 * Note, that in spite of type of the parameter (const char*) it's std::string which is really
	 * stored in the current Value! Type char* doesn't match the requirements to types which can be
	 * stored inside Value
	 */
	NParam& operator=(const char* val);

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
	operator NParam&() { return *this; }
	operator const NParam&() const { return *this; }
	/// Makes a copy of the current instance. Returns an instance of Value that references to a newly created copy.

	template<typename T> T& As() 
	{
		assert( _value.get() != NULL );
		assert( CheckClassesType( NrpText( typeid(T).name() ), GetType() ) );
		if( typeid(T)==typeid(NParam) ) return *this;
	
		return ((NProxyParam<T>*)(_value.get()))->Get();
	}

	template<typename T> T& As() const
	{
		assert( _value.get() != NULL );
		assert( CheckClassesType( NrpText( typeid(T).name() ), GetType() ) );
		return const_cast<NParam *>(this)->As<T>();
	}

	operator bool() { return As<bool>(); }
	operator bool() const { return const_cast<NParam *>(this)->As<bool>(); }
	operator int() { return As<int>(); }
	operator int() const { return const_cast<NParam *>(this)->As<int>(); }
	operator float() { return As<float>(); }
	operator float() const { return const_cast<NParam *>(this)->As<float>(); }
	operator NrpText() { return As<NrpText>(); }
	operator NrpText() const { return const_cast<NParam *>(this)->As<NrpText>(); }
	bool operator < ( const NParam& a ) const { return _value.get() < a._value.get(); }
	bool operator == ( const NParam& a ) const { return _value.get() == a._value.get(); }

	template<typename T> bool operator !=( const T& a ) const { return As<T>() != a; }
	template<typename T> bool operator >=( const T& b ) const { return As<T>() >= b; }

	template<typename T> T operator*( const T& b ) const { return As<T>() * b; }
	template<typename T> T operator-( const T& b ) const { return As<T>() - b; }

	template<typename T> operator T() const { return As<T>(); }
	template<typename T> operator const T&() const { return As<T>(); }
	template<typename T> operator const T&() { return As<T>(); }
	template<typename T> operator T&() { return As<T>(); }

	template<typename T> bool operator ==( const T& a ) const { return As<T>() == a; }
	template<typename T> NParam& operator +=( const T& a ) { As<T>() += a; return *this; }
	template<typename T> NParam& operator -=( const T& a ) { As<T>() -= a; return *this; }

	NrpText GetType() const;
	/// Safe (no-exception-throwing) Value type checker
	/**
	 * Compares specified type info (template parameter) and type name with stored value type.
	 * @param typeName - serialized type name (e.g. "int", "double", "Structure", etc).
	 * @return true if type info <b>or</b> type name matches, and false if both checks fail.
	 */
	template<typename T>
	bool Is() const
	{
		if( _value.get() )
		{
			if (typeid(T) == typeid(void)) return true;
			return false;
		}
		return ( _value->GetType() == typeid(T).name() );
	}

	bool IsNull() const;

	/// Erase current Value
	void Reset() 
	{
		_value.release();// = std::auto_ptr<INrpProperty>();
	}
	
	virtual ~NParam(){}
private:
	mutable std::auto_ptr<IParam> _value;
};

class INrpConfig : public INrpObject
{
	friend class CNrpConfigLooder;
	typedef core::map< OPTION_NAME, NParam* > PARAMS;
public:
	INrpConfig( CLASS_NAME className, SYSTEM_NAME sysName ) : INrpObject( className, sysName )
	{

	}

	INrpConfig(const INrpConfig& copy);

	virtual NParam& operator[](OPTION_NAME& key);
	
	/// Const accessor to the Structure element by its name.
	virtual const NParam& operator[](OPTION_NAME& key) const;

	bool IsExist(const NrpText& key) const;
	/// Tests whether the index is in bounds of the Array.

	NParam& Param( OPTION_NAME& key ) { return operator[](key); }
	NrpText Text( OPTION_NAME& key ) { return operator[](key).As<NrpText>(); }

protected:
	virtual NrpText Save( const NrpText& fileName );
	virtual void Load( const NrpText& fileName );

	template< class B > void Push( OPTION_NAME& name, const B& valuel )
	{
		NrpText fName = name.ToLower();

		if( _params.find( fName ) == NULL )
			_params[ fName ] = new NParam( valuel );
		else
			*_params[ fName ] = valuel;
	}

	unsigned Pop(const NrpText& key);
	/// Erase the specified element of the Array

private:
	//! определение массива свойств
	INrpConfig() : INrpObject( "error", "error" ) {}
	mutable PARAMS _params;
};

} //namespace nrp