#include "StdAfx.h"
#include "NrpConfigLooder.h"
#include "nrpConfig.h"
#include "IUser.h"
#include "IniFile.h"
#include "OpFileSystem.h"
#include "NrpTranslate.h"
#include "NrpApplication.h"

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

void CNrpConfigLooder::_WriteInt( const INrpProperty* prop, const std::string& key, 
								  const std::string& fileName )
{
	IniFile::Write( SECTION_PROPERTIES, key + ":int", ((CNrpProperty<int>*)prop)->GetValue(), fileName );
}

void CNrpConfigLooder::Save( const std::string& fileName ) 
{
	IniFile::Write( "test", "test", std::string("test"), fileName );
	assert( OpFileSystem::IsExist( fileName ) );

	PropertyArray::const_iterator paIter = _config->options_.begin();

	for( ; paIter != _config->options_.end(); paIter++)
	{
		std::string typesave = "";
		INrpProperty* prop = paIter->second;
		
		WRITERS_MAP::const_iterator wIter = _writers.find( prop->GetValueType() );

		if( wIter != _writers.end() )
			(this->*(wIter->second))( prop, paIter->first, fileName );
		else
		{
			WRITERS_MAP::const_iterator unkIter = _writers.find( "unknown" );
			if( unkIter != _writers.end() )
				(this->*(unkIter->second) )( prop, paIter->first, fileName );
		}
	}
}

void CNrpConfigLooder::Load( const std::string& fileName )
{
	assert( OpFileSystem::IsExist( fileName ) );
	const size_t MAX_BUFFER = 32000;
	char buffer[ MAX_BUFFER ];
	memset( buffer, 0, MAX_BUFFER );
	GetPrivateProfileSection( SECTION_PROPERTIES.c_str(), buffer, MAX_BUFFER, fileName.c_str() );

	std::string readLine = buffer;
	while( readLine != "" )
	{
		KeyPair pairt( readLine );

		READERS_MAP::iterator rIter = _readers.find( pairt.GetType() );
		if( rIter != _readers.end() )
			(this->*(rIter->second))( &pairt );
				
		memcpy( buffer, buffer + strlen(buffer) + 1, MAX_BUFFER );  
		readLine = buffer;
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
}

void CNrpConfigLooder::_InitWriters()
{
	_writers[ typeid( int ).name() ] = &CNrpConfigLooder::_WriteInt;
	_writers[ "enum" ] = &CNrpConfigLooder::_WriteInt;
	_writers[ typeid( std::string ).name() ] = &CNrpConfigLooder::_WriteString;
	_writers[ typeid( bool ).name() ] =  &CNrpConfigLooder::_WriteBool;
	_writers[ typeid( SYSTEMTIME ).name() ] =  &CNrpConfigLooder::_WriteTime;
	_writers[ "unknown" ] = &CNrpConfigLooder::_WriteUnknown;
	_writers[ typeid( float ).name() ] = &CNrpConfigLooder::_WriteFloat;
	_writers[ typeid( PUser ).name() ] = &CNrpConfigLooder::_WriteUser;
}

void CNrpConfigLooder::_ReadInt( KeyPair* p )
{
	_config->CreateValue<int>( p->GetName(), static_cast< int >( translate::GetNumber( p->GetValue().c_str() ) ) );	
}

void CNrpConfigLooder::_ReadFloat( KeyPair* p )
{
	_config->CreateValue<float>( p->GetName(), static_cast< float >( translate::GetNumber( p->GetValue().c_str() ) ) );	
}

void CNrpConfigLooder::_ReadBool( KeyPair* p )
{
	_config->CreateValue<bool>( p->GetName(), conv::ToBool( p->GetValue().c_str() ) );
}

void CNrpConfigLooder::_ReadString( KeyPair* p )
{
	_config->CreateValue<std::string>( p->GetName(), translate::GetTranslate( p->GetValue().c_str() ) );
}

void CNrpConfigLooder::_ReadTime( KeyPair* p )
{
	_config->CreateValue<SYSTEMTIME>( p->GetName(), conv::ToTime( p->GetValue().c_str()) );
}

void CNrpConfigLooder::_ReadUser( KeyPair* p )
{
	IUser* user = CNrpApplication::Instance().GetUser( p->GetValue() );
	assert( user != NULL );
	if( user )
		_config->CreateValue<PUser>( p->GetName(), user );
}

void CNrpConfigLooder::_WriteUnknown( const INrpProperty* prop, const std::string& key, const std::string& fileName )
{
	std::string dd = const_cast< INrpProperty* >( prop )->GetValueType();
	if( dd.find( "enum" ) == 0 )
	{
		_WriteInt( prop, key, fileName );
	}
	else 
		IniFile::Write( SECTION_PROPERTIES, key + ":unknown", std::string(""), fileName );	
}

void CNrpConfigLooder::_WriteUser( const INrpProperty* prop, const std::string& key, 
								   const std::string& fileName )
{
	PUser user = ((CNrpProperty<PUser>*)prop)->GetValue();
	//assert( user );

	if( user )
		IniFile::Write( SECTION_PROPERTIES, key + ":user", user->GetString( NAME ), fileName );
}

void CNrpConfigLooder::_WriteFloat( const INrpProperty* prop, const std::string& key, 
								   const std::string& fileName )
{
	IniFile::Write( SECTION_PROPERTIES, key + ":float", ((CNrpProperty<float>*)prop)->GetValue(), fileName );
}

void CNrpConfigLooder::_WriteTime( const INrpProperty* prop, const std::string& key, 
								   const std::string& fileName )
{
	IniFile::Write( SECTION_PROPERTIES, key + ":time", ((CNrpProperty<SYSTEMTIME>*)prop)->GetValue(), fileName );
}

void CNrpConfigLooder::_WriteString( const INrpProperty* prop, const std::string& key, 
									 const std::string& fileName )
{
	IniFile::Write( SECTION_PROPERTIES, key + ":string", ((CNrpProperty<std::string>*)prop)->GetValue(), fileName );
}

void CNrpConfigLooder::_WriteBool( const INrpProperty* prop, const std::string& key, 
								   const std::string& fileName )
{
	IniFile::Write( SECTION_PROPERTIES, key + ":bool", ((CNrpProperty<bool>*)prop)->GetValue(), fileName );
}
}//end namespace nrp
