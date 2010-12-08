#include "stdafx.h"
#include "nrpConfig.h"
#include "NrpConfigLooder.h"

namespace nrp
{
//�������� ���������� �����
bool CheckClassesType( const type_info& type1, const type_info& type2 )
{
	if( type1 != type2 ) 
	{
		Log(HW) << "warning: request type " << type2.name() << " but native typename is " << type1.name() << term;
		return false;
	}

	return true;
}

//�������� ���������� �� �����
void INrpConfig::Load( const NrpText& fileName )
{
	CNrpConfigLooder p( this );
	p.Load( fileName );
}

//���������� ������� � ����
NrpText INrpConfig::Save( const NrpText& fileName )
{
	CNrpConfigLooder p( this );
	p.Save( fileName );
	return fileName;
}

//�������� ������������� ���������
bool INrpConfig::IsExist(const NrpText& key) const 
{
	return _params.find( key.ToLower() ) != _params.end();
}

//�������� ��������� �� �����
unsigned INrpConfig::Remove(const NrpText& key) 
{
	PARAMS::iterator node = _params.find( key.ToLower() );
	if( node != _params.end() )
	{
		node->second->Reset();
		_params.erase( node );
	}

	return 0;
}

//��������� ������� � ���������
NParam& INrpConfig::operator[](OPTION_NAME& key) 
{
	PARAMS::iterator node = _params.find( key.ToLower() );
	if( node != _params.end() )
	{
		return *(node->second);
	}
	NrpText err = NrpText( "invalid key" ) + NrpText( key );
	Log(HW) << "error: invalid key " << ObjectTypeName() << ":" << key  << term;
	throw std::exception( err );
}

const NParam& INrpConfig::operator[](OPTION_NAME& key) const
{
	PARAMS::iterator node = _params.find( key.ToLower() );
	if( node != _params.end() )
	{
		return *(node->second);
	}
	NrpText err = NrpText( "invalid key" ) + NrpText( key );
	Log(HW) << "error: invalid key " << const_cast< INrpConfig* >( this )->ObjectTypeName()  << ":"  << key << term;
	throw std::exception( err );
}


//
NParam::NParam( const char* str )
{
	_value = std::auto_ptr< IParam >( new NProxyParam<NrpText>(str) );
}

//
bool NParam::IsNull() const
{
	return _value.get() == NULL;
}

//����������� �� ��������� ���������
void NParam::_CheckNotifications()
{
	for( NOTIFICATIONS::iterator pIter=_notifications.begin();
		 pIter != _notifications.end(); 
		 pIter++ )
	{
		try
		{
			pIter->second->Exec();
		}
		catch (...)
		{
			
		}
	}
}
}//end namespace nrp