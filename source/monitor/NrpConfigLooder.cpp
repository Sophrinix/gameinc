#include "StdAfx.h"
#include "NrpConfigLooder.h"
#include "nrpConfig.h"
#include "IUser.h"
#include "IniFile.h"
#include "OpFileSystem.h"
#include "NrpApplication.h"
#include "NrpLaborMarket.h"
#include "NrpTechnology.h"

#include <memory>

namespace nrp
{


CNrpConfigLooder::CNrpConfigLooder(void)
{
}

CNrpConfigLooder::CNrpConfigLooder( INrpConfig* ptrConfig ):
	_config( ptrConfig )
{
	_InitReaders();
	_InitWriters();
}

CNrpConfigLooder::~CNrpConfigLooder(void)
{
}

void CNrpConfigLooder::_WriteInt( const NParam& prop, const NrpText& key, IniFile& ini )
{
	ini.Set( SECTION_PROPERTIES, key + L":int", (int)prop );
}

void CNrpConfigLooder::Save( const NrpText& fileName ) 
{
	IniFile ini( fileName );
	_fileName = fileName;

	INrpConfig::PARAMS::iterator paIter = _config->_params.begin();

	for( ; paIter != _config->_params.end(); paIter++)
	{
		NrpText typesave = "";
		NParam* prop = paIter->second;
		assert( !prop->IsNull() );
		
		WRITERS_MAP::Node* wIter = _writers.find( prop->GetType() );

		if( wIter )
			(this->*(wIter->getValue()))( *prop, paIter->first, ini );		
		else
			_WriteUnknown( *prop, paIter->first, ini );		
	}

	ini.Save();
	assert( OpFileSystem::IsExist( fileName ) );
}

void CNrpConfigLooder::Load( const NrpText& fileName )
{
	assert( OpFileSystem::IsExist( fileName ) );

	_fileName = fileName;
	IniFile ini( fileName );

	const IniSection::KeyIndexA& indexes = ini.GetSection( SECTION_PROPERTIES )->GetKeys();

	for( IniSection::KeyIndexA::const_iterator pIter = indexes.begin(); pIter != indexes.end(); pIter++ )
	{
		if( (*pIter)->GetKey().find( ";" ) != 0 )
		{
			READERS_MAP::Node* rIter = _readers.find( (*pIter)->GetType().c_str() );
			assert( rIter );
			if( rIter )
				(this->*(rIter->getValue()))( *pIter );
		}
	}
}

void CNrpConfigLooder::_InitReaders()
{
	_readers[ "int" ] = &CNrpConfigLooder::_ReadInt;
	_readers[ "float" ] = &CNrpConfigLooder::_ReadFloat;
	_readers[ "bool" ] = &CNrpConfigLooder::_ReadBool;
	_readers[ "string" ] = &CNrpConfigLooder::_ReadString;
	_readers[ "time" ] = &CNrpConfigLooder::_ReadTime;
	_readers[ "user" ] = &CNrpConfigLooder::_ReadUser;
	_readers[ "tech" ] = &CNrpConfigLooder::_ReadTechnology;
	_readers[ "unknown" ] = &CNrpConfigLooder::_ReadUnknown;
	_readers[ "dim2u" ] = &CNrpConfigLooder::_ReadDim2u;
	_readers[ "path" ] = &CNrpConfigLooder::_ReadPath;
}

void CNrpConfigLooder::_InitWriters()
{
	_writers[ typeid( int ).name() ] = &CNrpConfigLooder::_WriteInt;
	_writers[ L"enum" ] = &CNrpConfigLooder::_WriteInt;
	_writers[ typeid( stringw ).name() ] = &CNrpConfigLooder::_WriteString;
	_writers[ typeid( NrpText ).name() ] = &CNrpConfigLooder::_WriteString;
	_writers[ typeid( stringc ).name() ] = &CNrpConfigLooder::_WriteString;
	_writers[ typeid( bool ).name() ] =  &CNrpConfigLooder::_WriteBool;
	_writers[ typeid( NrpTime ).name() ] =  &CNrpConfigLooder::_WriteTime;
	_writers[ L"unknown" ] = &CNrpConfigLooder::_WriteUnknown;
	_writers[ typeid( float ).name() ] = &CNrpConfigLooder::_WriteFloat;
	_writers[ typeid( PUser ).name() ] = &CNrpConfigLooder::_WriteUser;
	_writers[ typeid( CNrpTechnology ).name() ] = &CNrpConfigLooder::_WriteTechnology;
}

void CNrpConfigLooder::_ReadPath( IniKey* p )
{
	if( _fileName.ToWide()[ 0 ] == L'$' )
	{
		//yet not work
		assert( false );
	}

	NrpText myDir = OpFileSystem::UpDir( _fileName );

	(*_config)[ p->GetShortKey().c_str() ] = OpFileSystem::CheckFile( myDir,  p->GetValue().c_str() );	
}

void CNrpConfigLooder::_ReadInt( IniKey* p )
{
	(*_config)[ p->GetShortKey().c_str() ] = static_cast< int >( NrpText::LuaNumber( p->GetValue().c_str() ) );	
}

void CNrpConfigLooder::_ReadFloat( IniKey* p )
{
	(*_config)[ p->GetShortKey().c_str() ] = static_cast< float >( NrpText::LuaNumber( p->GetValue().c_str() ) );	
}

void CNrpConfigLooder::_ReadBool( IniKey* p )
{
	(*_config)[ p->GetShortKey().c_str() ] = NrpText( p->GetValue().c_str() ).ToBool();
}

void CNrpConfigLooder::_ReadString( IniKey* p )
{
	(*_config)[ p->GetShortKey().c_str() ] = NrpText::LuaString( p->GetValue().c_str() );
}

void CNrpConfigLooder::_ReadTime( IniKey* p )
{
	(*_config)[ p->GetShortKey().c_str() ] = NrpTime( NrpText( p->GetValue().c_str() ) );
}

void CNrpConfigLooder::_ReadUser( IniKey* p )
{
	CNrpUser* user = _nrpLaborMarkt.GetUser( p->GetValue().c_str() );

	if( user )
		(*_config)[ p->GetShortKey().c_str() ] = user;
	else
		Log(HW) << "User NULL from " << p->GetValue();
}

void CNrpConfigLooder::_ReadTechnology( IniKey* p )
{
	PNrpTechnology tech = _nrpApp.GetTechnology( p->GetValue().c_str() );
	assert( tech != NULL );
	if( tech )
		(*_config)[ p->GetShortKey().c_str() ] = tech;
}

void CNrpConfigLooder::_WriteUnknown( const NParam& prop, const NrpText& key, IniFile& ini )
{
	NrpText dd = const_cast< NParam& >( prop ).GetType();
	if( dd.find( L"enum" ) == 0 )
	{
		_WriteInt( prop, key, ini );
	}
	else 
		ini.Set( SECTION_PROPERTIES, key + L":unknown", NrpText("") );	
}

void CNrpConfigLooder::_WriteTechnology( const NParam& prop, const NrpText& key, IniFile& ini )
{
	const CNrpTechnology& tech = *(prop.As<PNrpTechnology>());
	
	if( _nrpApp.GetTechnology( tech[ NAME ].As<NrpText>() ) )
	{
		ini.Set( SECTION_PROPERTIES, key + L":tech", (NrpText)tech[ NAME ] );
	}
}

void CNrpConfigLooder::_WriteUser( const NParam& prop, const NrpText& key, IniFile& ini )
{
	const PUser user = prop.As<PUser>();
	if( user )
		ini.Set( SECTION_PROPERTIES, key + L":user", (NrpText)(*user)[ NAME ] );
}

void CNrpConfigLooder::_WriteFloat( const NParam& prop, const NrpText& key, IniFile& ini )
{
	ini.Set( SECTION_PROPERTIES, key + ":float", prop.As<float>() );
}

void CNrpConfigLooder::_WriteTime( const NParam& prop, const NrpText& key, IniFile& ini )
{
	ini.Set( SECTION_PROPERTIES, key + L":time", prop.As<NrpTime>() );
}

void CNrpConfigLooder::_WriteString( const NParam& prop, const NrpText& key, IniFile& ini)
{
	ini.Set( SECTION_PROPERTIES, key + L":string", prop.As<NrpText>() );
}

void CNrpConfigLooder::_WriteBool( const NParam& prop, const NrpText& key, IniFile& ini )
{
	ini.Set( SECTION_PROPERTIES, key + L":bool", prop.As<bool>() );
}

void CNrpConfigLooder::_ReadUnknown( IniKey* p )
{

}

void CNrpConfigLooder::_ReadDim2u( IniKey* p )
{
	_config->Add<core::dimension2du>( p->GetShortKey().c_str(), NrpText( p->GetValue().c_str() ).ToDim2du() );
}
}//end namespace nrp
