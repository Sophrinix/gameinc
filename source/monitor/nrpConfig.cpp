#include "stdafx.h"
#include "nrpConfig.h"
#include "NrpTranslate.h"
#include "NrpConfigLooder.h"

namespace nrp
{

bool CheckClassesType( const NrpText& type1, const NrpText& type2 )
{
	if( type1 != type2 ) 
	{
		Log(HW) << L"warning: request type " << type2 << L" but native typename is " << type1 << term;
		return false;
	}

	return true;
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

bool INrpConfig::IsExist(const NrpText& key) const 
{
	return _params.find( key ) != NULL;
}

unsigned INrpConfig::Pop(const NrpText& key) 
{
	if( PARAMS::Node* node = _params.find( key ) )
	{
		node->getValue()->Reset();
		_params.remove( key );
	}

	return 0;
}

NParam& INrpConfig::operator[](OPTION_NAME& key) 
{
	if( PARAMS::Node* node = _params.find( key.ToLower() ) )
	{
		return *(node->getValue());
	}
	throw std::exception( "invalid key" );
}

const NParam& INrpConfig::operator[](OPTION_NAME& key) const
{
	if( PARAMS::Node* node = _params.find( key ) )
	{
		return (*node->getValue());
	}
	throw std::exception( "invalid key" );
}

NParam::NParam( const char* str )
{
	_value = std::auto_ptr< IParam >( new NProxyParam<NrpText>(str) );
}

NrpText NParam::GetType() const
{
	if( _value.get() ) 
		return _value.get()->GetType();

	return typeid(void).name();
}

bool NParam::IsNull() const
{
	return _value.get() == NULL;
}
}//end namespace nrp