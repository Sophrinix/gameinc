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
#include <typeinfo.h>
#include <assert.h>
#include <memory>

#include "INrpObject.h"
#include "OptionNames.h"
#include "nrptext.h"

#define _self (*this)

using namespace irr;

namespace nrp
{

bool CheckClassesType( const type_info& type1, const type_info& type2 );

class IParam 
{
public:
	/// Returns RTTI of current instance of Object.
	virtual const type_info& GetType() = 0;
	virtual std::auto_ptr<IParam> Duplicate() const = 0;
	virtual void Reset() = 0;
};

typedef std::auto_ptr<IParam> APtrParam;

class INotification
{
public:
	virtual void Exec() = 0;
};

class NParam;

//Класс для хранения калбэка обработчика изменения параметра
template< class T>
class NNotification : public INotification
{
	typedef void (T::*Method)( NParam& );

public:
	NNotification( NParam* parent, T* object, Method objMethod ) : 
		_object( object ), 
		_objMethod( objMethod ),
		_parent( parent )
	{

	}

	void Exec() { (_object->*_objMethod )( *_parent ); }
private:

	T* _object;
	Method _objMethod;
	NParam* _parent;
};

template<typename T>
class NProxyParam : public IParam 
{
public:
	NProxyParam(const T& value) : _value(value) {}
	NProxyParam(const NProxyParam& copy) : _value(copy._value){}
	/// Get the stored data
	operator T& () { return _value; }
	/// Get the stored data
	/**
	 * @return the reference to the stored data element
	 */
	T& Get() { return _value; }
	/// Get the stored data
	/**
	 * @return the constant reference to the stored data element
	 */
	const int& Get() const { return _value; }
	virtual const type_info& GetType() { return typeid(T); }

	IParam& operator=(const NProxyParam& copy) 
	{
		_value = copy._value;
		return *this;
	}

	IParam& operator+=(const NProxyParam& copy) 
	{
		_value += copy._value;
		return *this;
	}

	bool operator<(const NProxyParam& copy) 
	{
		return _value < copy._value;
	}

	bool operator==(const NProxyParam& copy) 
	{
		return _value == copy._value;
	}

	std::auto_ptr<IParam> Duplicate() const 
	{
		std::auto_ptr<IParam> sobjCopy( new NProxyParam<T>(_value) );
		return sobjCopy;
	}

	void Reset() {};

	~NProxyParam(){}
private:
	NProxyParam(){}
	T _value;
};

class NParam 
{
	NParam() {}

public:
	NParam(const NParam& copy) : _value(copy._value) {}

	NParam& operator=(const NParam& copy) 
	{
		assert( CheckClassesType( typeid( copy._value ) , typeid( _value ) ) );
		_value = copy._value->Duplicate();
		return *this;
	}

	NParam(const char* str );
	/// Template for constructing a Value object from any type.  Creates a copy of the object by using its copy constructor.
	template<typename T> NParam( const NrpText& name, const T& val ) : _name( name )
	{
		_value = std::auto_ptr< IParam >( new NProxyParam<T>(val) );
	}

	/// Assigns the Value instance with object of any type by using its copy constructor.
	template<typename T> NParam& operator=(const T& val)
	{
		if( _value.get() )
			assert( CheckClassesType( typeid(T), _value->GetType() ) );

		As<T>() = val;

		_CheckNotifications();
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
		assert( CheckClassesType( typeid(T), _value.get()->GetType() ) );
		if( typeid(T)==typeid(NParam) ) return *this;
	
		return ((NProxyParam<T>*)(_value.get()))->Get();
	}

	template<typename T> T& As() const
	{
		assert( _value.get() != NULL );
		assert( CheckClassesType( typeid(T), _value.get()->GetType() ) );
		return const_cast<NParam *>(this)->As<T>();
	}

	operator int() { return As<int>(); }
	operator const int&() const { return As<int>(); }
	operator bool() { return As<bool>(); }
	operator float() { return As<float>(); }
	operator const float&() const { return As<float>(); }
	operator const NrpText&() const { return As<NrpText>(); }

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

	/// Safe (no-exception-throwing) Value type checker
	/**
	 * Compares specified type info (template parameter) and type name with stored value type.
	 * @param typeName - serialized type name (e.g. "int", "double", "Structure", etc).
	 * @return true if type info <b>or</b> type name matches, and false if both checks fail.
	 */
	template<typename T>
	bool Is()
	{
		if( _value.get() )
		{
			if (typeid(T) == typeid(void)) return true;
			return false;
		}
		return ( _value->GetType() == typeid(T) );
	}

	bool IsNull() const;

	/// Erase current Value
	void Reset() 
	{
		_value.release();// = std::auto_ptr<INrpProperty>();
	}

	NrpText GetType()
	{
		if( _value.get() == NULL )
			return NrpText( L"void" );

		return NrpText( _value->GetType().name() );
	}

	const NrpText& GetName() { return _name; }

	template< class OBJ_CLASS >
	void AddNotification( const NrpText& key, OBJ_CLASS* obj, void (OBJ_CLASS::*Method)( NParam& ) )
	{
		_notifications[ key ] = new NNotification< OBJ_CLASS >( this, obj, Method );
	}
	
	virtual ~NParam(){}
private:
	bool operator < ( const NParam& a ) const { return false; }
	bool operator == ( const NParam& a ) const { return false; }

	mutable APtrParam _value;
	NrpText _name;

	void _CheckNotifications();

	typedef std::map< NrpText, INotification* > NOTIFICATIONS;
	NOTIFICATIONS _notifications;
};

class INrpConfig : public INrpObject
{
	friend class CNrpConfigLooder;
	typedef std::map< OPTION_NAME, NParam* > PARAMS;
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

	void UpdatedParam( NParam& ) {};

	NParam& Param( OPTION_NAME& key ) { return operator[](key); }
	NrpText Text( OPTION_NAME& key ) { return operator[](key).As<NrpText>(); }

protected:
	virtual NrpText Save( const NrpText& fileName );
	virtual void Load( const NrpText& fileName );

	template< class B > void Add( OPTION_NAME& name, const B& valuel )
	{
		NrpText fName = name.ToLower();

		if( _params.find( fName ) != _params.end() )
			*_params[ fName ] = valuel;
		else
			_params[ fName ] = new NParam( fName, valuel );
	}

	unsigned Remove(const NrpText& key);
	/// Erase the specified element of the Array
private:
	//! определение массива свойств
	INrpConfig() : INrpObject( "error", "error" ) {}
	mutable PARAMS _params;
};

} //namespace nrp