#include "stdafx.h"
#include "nrpConfig.h"
#include "NrpTranslate.h"
#include "NrpConfigLooder.h"

namespace nrp
{

void CheckClassesType( const NrpText& type1, const NrpText& type2 )
{
	if( type1 != type2 ) 
		Log(HW) << L"warning: request type " << type2 << L" but native typename is " << type1 << term;
}

void INrpConfig::Load( const NrpText& fileName )
{
	CNrpConfigLooder p( this );
	p.Load( fileName );
}

NrpText INrpConfig::Save( const NrpText& fileName )
{
	CNrpConfigLooder p( this );
	p.Save( fileName );
	return fileName;
}

void INrpConfig::Pop( const NrpText& name )
{

}

bool INrpConfig::IsExist(const NrpText& key) const 
{
	return _options.find( key ) != NULL;
}

unsigned INrpConfig::Erase(const NrpText& key) 
{
	if( PropertyArray::Node* node = _options.find( key ) )
	{
		node->getValue()->Reset();
		_options.remove( key );
	}

	return 0;
}

CNrpProperty& INrpConfig::operator[](OPTION_NAME& key) 
{
	if( PropertyArray::Node* node = _options.find( key ) )
	{
		return *(node->getValue());
	}
	throw std::exception( "invalid key" );
}

const CNrpProperty& INrpConfig::operator[](OPTION_NAME& key) const
{
	if( PropertyArray::Node* node = _options.find( key ) )
	{
		return (*node->getValue());
	}
	throw std::exception( "invalid key" );
}

template<typename T>
CNrpProperty& CNrpProperty::operator=( const T& val )
{
	_value = std::auto_ptr< INrpProperty >( new CNrpProxyProperty<T>(val) );
	return *this;
}

CNrpProperty::CNrpProperty( const char* str )
{
	_value = std::auto_ptr< INrpProperty >( new CNrpProxyProperty<NrpText>(str) );
}

NrpText CNrpProperty::GetType() const
{
	if( _value.get() ) 
		return _value.get()->GetType();

	return typeid(void).name();
}

bool CNrpProperty::IsNull() const
{
	return _value.get() == NULL;
}
}//end namespace nrp