#include "StdAfx.h"
#include "NrpConfigSyncer.h"
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

const NrpText CNrpConfigSyncer::t_int = L"int";
const NrpText CNrpConfigSyncer::t_float = L"float";
const NrpText CNrpConfigSyncer::t_bool = L"bool";
const NrpText CNrpConfigSyncer::t_string = L"string";
const NrpText CNrpConfigSyncer::t_time = L"time";
const NrpText CNrpConfigSyncer::t_user = L"user";
const NrpText CNrpConfigSyncer::t_tech = L"tech";
const NrpText CNrpConfigSyncer::t_enum = L"enum";
const NrpText CNrpConfigSyncer::t_unknown = L"unknown";
const NrpText CNrpConfigSyncer::t_dim2u = L"dim2u";
const NrpText CNrpConfigSyncer::t_path = L"path";

NrpText _d( const NrpText& text )
{
    return NrpText(":") + text;
}

CNrpConfigSyncer::CNrpConfigSyncer(void)
{
}

CNrpConfigSyncer::CNrpConfigSyncer( INrpConfig* ptrConfig ):
	_config( ptrConfig )
{
	_InitReaders();
	_InitWriters();
}

CNrpConfigSyncer::~CNrpConfigSyncer(void)
{
}

void CNrpConfigSyncer::_WriteInt( const NParam& prop, const NrpText& key, IniFile& ini, const NrpText& section )
{
	ini.Set( section, key + _d( t_int ), (int)prop );
}

void CNrpConfigSyncer::Save( const NrpText& fileName, SECTION_NAME& section ) 
{
	IniFile ini( fileName );
    Save( ini, section );

    ini.Save();
    assert( OpFileSystem::IsExist( ini.GetFileName() ) );
}

void CNrpConfigSyncer::Save( IniFile& ini, SECTION_NAME& section )
{
    INrpConfig::PARAMS::iterator paIter = _config->_params.begin();

    for( ; paIter != _config->_params.end(); paIter++)
    {
        NrpText typesave = "";
        NParam* prop = paIter->second;
        assert( !prop->IsNull() );

        WRITERS_MAP::Node* wIter = _writers.find( prop->GetType() );

        if( wIter )
            (this->*(wIter->getValue()))( *prop, paIter->first, ini, section );		
        else
            _WriteUnknown( *prop, paIter->first, ini, section );		
    }
}

void CNrpConfigSyncer::Load( const NrpText& fileName, SECTION_NAME& section )
{
    _fileName = fileName;
	assert( OpFileSystem::IsExist( fileName ) );

	IniFile ini( fileName );
    Load( ini, section );
}

void CNrpConfigSyncer::Load( IniFile& ini, SECTION_NAME& section )
{
    const IniSection::KeyIndexA& indexes = ini.GetSection( section )->GetKeys();

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

void CNrpConfigSyncer::_InitReaders()
{
	_readers[ t_int ] = &CNrpConfigSyncer::_ReadInt;
	_readers[ t_float ] = &CNrpConfigSyncer::_ReadFloat;
	_readers[ t_bool ] = &CNrpConfigSyncer::_ReadBool;
	_readers[ t_string ] = &CNrpConfigSyncer::_ReadString;
	_readers[ t_time ] = &CNrpConfigSyncer::_ReadTime;
	_readers[ t_user ] = &CNrpConfigSyncer::_ReadUser;
	_readers[ t_tech ] = &CNrpConfigSyncer::_ReadTechnology;
	_readers[ t_unknown ] = &CNrpConfigSyncer::_ReadUnknown;
	_readers[ t_dim2u ] = &CNrpConfigSyncer::_ReadDim2u;
	_readers[ t_path ] = &CNrpConfigSyncer::_ReadPath;
}

void CNrpConfigSyncer::_InitWriters()
{
	_writers[ typeid( int ).name() ] = &CNrpConfigSyncer::_WriteInt;
	_writers[ t_enum ] = &CNrpConfigSyncer::_WriteInt;
	_writers[ typeid( stringw ).name() ] = &CNrpConfigSyncer::_WriteString;
	_writers[ typeid( NrpText ).name() ] = &CNrpConfigSyncer::_WriteString;
	_writers[ typeid( stringc ).name() ] = &CNrpConfigSyncer::_WriteString;
	_writers[ typeid( bool ).name() ] =  &CNrpConfigSyncer::_WriteBool;
	_writers[ typeid( NrpTime ).name() ] =  &CNrpConfigSyncer::_WriteTime;
	_writers[ t_unknown ] = &CNrpConfigSyncer::_WriteUnknown;
	_writers[ typeid( float ).name() ] = &CNrpConfigSyncer::_WriteFloat;
	_writers[ typeid( PUser ).name() ] = &CNrpConfigSyncer::_WriteUser;
	_writers[ typeid( CNrpTechnology ).name() ] = &CNrpConfigSyncer::_WriteTechnology;
}

void CNrpConfigSyncer::_ReadPath( IniKey* p )
{
	if( p->GetValue().c_str()[ 0 ] == '$' )
	{
		//yet not work
		assert( false );
	}

	NrpText myDir = OpFileSystem::UpDir( _fileName );

	(*_config)[ p->GetShortKey().c_str() ] = OpFileSystem::CheckFile( myDir,  p->GetValue().c_str() );	
}

void CNrpConfigSyncer::_ReadInt( IniKey* p )
{
	(*_config)[ p->GetShortKey().c_str() ] = static_cast< int >( NrpText::LuaNumber( p->GetValue().c_str() ) );	
}

void CNrpConfigSyncer::_ReadFloat( IniKey* p )
{
	(*_config)[ p->GetShortKey().c_str() ] = static_cast< float >( NrpText::LuaNumber( p->GetValue().c_str() ) );	
}

void CNrpConfigSyncer::_ReadBool( IniKey* p )
{
	(*_config)[ p->GetShortKey().c_str() ] = NrpText( p->GetValue().c_str() ).ToBool();
}

void CNrpConfigSyncer::_ReadString( IniKey* p )
{
	(*_config)[ p->GetShortKey().c_str() ] = NrpText::LuaString( p->GetValue().c_str() );
}

void CNrpConfigSyncer::_ReadTime( IniKey* p )
{
	(*_config)[ p->GetShortKey().c_str() ] = NrpTime( NrpText( p->GetValue().c_str() ) );
}

void CNrpConfigSyncer::_ReadUser( IniKey* p )
{
	CNrpUser* user = _nrpLaborMarkt.GetUser( p->GetValue().c_str() );

	if( user )
		(*_config)[ p->GetShortKey().c_str() ] = user;
	else
		Log(HW) << "User NULL from " << p->GetValue();
}

void CNrpConfigSyncer::_ReadTechnology( IniKey* p )
{
	PNrpTechnology tech = _nrpApp.GetTechnology( p->GetValue().c_str() );
	assert( tech != NULL );
	if( tech )
		(*_config)[ p->GetShortKey().c_str() ] = tech;
}

void CNrpConfigSyncer::_WriteUnknown( const NParam& prop, const NrpText& key, IniFile& ini, const NrpText& section )
{
	NrpText dd = const_cast< NParam& >( prop ).GetType();
	if( dd.find( L"enum" ) == 0 )
	{
		_WriteInt( prop, key, ini, section );
	}
	else 
		ini.Set( section, key + _d( t_unknown ), NrpText("") );	
}

void CNrpConfigSyncer::_WriteTechnology( const NParam& prop, const NrpText& key, IniFile& ini, const NrpText& section )
{
	const CNrpTechnology& tech = *(prop.As<PNrpTechnology>());
	
	if( _nrpApp.GetTechnology( tech[ NAME ].As<NrpText>() ) )
	{
		ini.Set( section, key + _d( t_tech ), (NrpText)tech[ NAME ] );
	}
}

void CNrpConfigSyncer::_WriteUser( const NParam& prop, const NrpText& key, IniFile& ini, const NrpText& section )
{
	const PUser user = prop.As<PUser>();
	if( user )
		ini.Set( section, key + _d( t_user ), (NrpText)(*user)[ NAME ] );
}

void CNrpConfigSyncer::_WriteFloat( const NParam& prop, const NrpText& key, IniFile& ini, const NrpText& section )
{
	ini.Set( section, key + _d( t_float ), prop.As<float>() );
}

void CNrpConfigSyncer::_WriteTime( const NParam& prop, const NrpText& key, IniFile& ini, const NrpText& section )
{
	ini.Set( section, key + _d( t_time ), prop.As<NrpTime>() );
}

void CNrpConfigSyncer::_WriteString( const NParam& prop, const NrpText& key, IniFile& ini, const NrpText& section )
{
	ini.Set( section, key + _d( t_string ), prop.As<NrpText>() );
}

void CNrpConfigSyncer::_WriteBool( const NParam& prop, const NrpText& key, IniFile& ini, const NrpText& section )
{
	ini.Set( section, key + _d( t_bool ), prop.As<bool>() );
}

void CNrpConfigSyncer::_ReadUnknown( IniKey* p )
{

}

void CNrpConfigSyncer::_ReadDim2u( IniKey* p )
{
	_config->RegProperty<core::dimension2du>( p->GetShortKey().c_str(), NrpText( p->GetValue().c_str() ).ToDim2du() );
}
}//end namespace nrp
