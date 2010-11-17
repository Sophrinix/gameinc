#include "StdAfx.h"
#include "NrpConfigLooder.h"
#include "nrpConfig.h"
#include "IUser.h"
#include "IniFile.h"
#include "OpFileSystem.h"
#include "NrpTranslate.h"
#include "NrpApplication.h"
#include "NrpTechnology.h"

#include <memory>

namespace nrp
{

class KeyPair
{
	KeyPair() {};

	NrpText _name, _type, _value;
public:
	KeyPair( const NrpText& str, 
		wchar_t delim=L':' )
	{
		int empPos = str.findFirst( L'=' );
		assert( empPos >= 0 );
		if( empPos >= 0 )
		{
			_value = str.subString( empPos+1, 0xff );
			_name = str.subString( 0, empPos );
			_type = "unknown";
			int delimPos = _name.findFirst( delim );
			if( delimPos >= 0 )
			{
				_type = _name.subString( delimPos + 1, 0xff );
				_name = _name.subString( 0, delimPos );
			}
		}
	}

	NrpText& GetName() { return _name; }
	NrpText& GetValue() { return _value; }
	NrpText& GetType() { return _type; }
};

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

void CNrpConfigLooder::_WriteInt( const CNrpProperty* prop, const NrpText& key, IniFile* ini )
{
	ini->Set( SECTION_PROPERTIES, key + L":int", prop->As<int>() );
}

void CNrpConfigLooder::Save( const NrpText& fileName ) 
{
	std::auto_ptr<IniFile> ini( new IniFile( fileName ) );
	ini->Set( "test", "test", NrpText("test") );

	assert( OpFileSystem::IsExist( fileName ) );

	INrpConfig::PropertyArray::Iterator paIter = _config->_options.getIterator();

	for( ; !paIter.atEnd(); paIter++)
	{
		NrpText typesave = "";
		CNrpProperty* prop = paIter->getValue();
		assert( !prop->IsNull() );
		
		WRITERS_MAP::Node* wIter = _writers.find( prop->GetType() );

		if( wIter )
			_WriteUnknown( prop, paIter->getKey(), ini.get() );
		else
			(this->*(wIter->getValue()))( prop, paIter->getKey(), ini.get() );
	}
}

void CNrpConfigLooder::Load( const NrpText& fileName )
{
	assert( OpFileSystem::IsExist( fileName ) );
	const size_t MAX_BUFFER = 32000;
	wchar_t buffer[ MAX_BUFFER ];
	memset( buffer, 0, MAX_BUFFER );
	GetPrivateProfileSectionW( SECTION_PROPERTIES.ToWide(), buffer, MAX_BUFFER, fileName.ToWide() );

	NrpText readLine( buffer );
	while( readLine.size() )
	{
		KeyPair pairt( readLine );

		READERS_MAP::Node* rIter = _readers.find( pairt.GetType() );
		assert( rIter );
		if( rIter )
			(this->*(rIter->getValue()))( &pairt );
				
		memcpy( buffer, buffer + wcslen(buffer) + 1, MAX_BUFFER );  
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
	_readers[ "tech" ] = &CNrpConfigLooder::_ReadTechnology;
	_readers[ "unknown" ] = &CNrpConfigLooder::_ReadUnknown;
	_readers[ "dim2u" ] = &CNrpConfigLooder::_ReadDim2u;

}

void CNrpConfigLooder::_InitWriters()
{
	_writers[ typeid( int ).name() ] = &CNrpConfigLooder::_WriteInt;
	_writers[ L"enum" ] = &CNrpConfigLooder::_WriteInt;
	_writers[ typeid( stringw ).name() ] = &CNrpConfigLooder::_WriteString;
	_writers[ typeid( NrpText ).name() ] = &CNrpConfigLooder::_WriteString;
	_writers[ typeid( stringc ).name() ] = &CNrpConfigLooder::_WriteString;
	_writers[ typeid( bool ).name() ] =  &CNrpConfigLooder::_WriteBool;
	_writers[ typeid( SYSTEMTIME ).name() ] =  &CNrpConfigLooder::_WriteTime;
	_writers[ L"unknown" ] = &CNrpConfigLooder::_WriteUnknown;
	_writers[ typeid( float ).name() ] = &CNrpConfigLooder::_WriteFloat;
	_writers[ typeid( PUser ).name() ] = &CNrpConfigLooder::_WriteUser;
	_writers[ typeid( CNrpTechnology ).name() ] = &CNrpConfigLooder::_WriteTechnology;
}

void CNrpConfigLooder::_ReadInt( KeyPair* p )
{
	_config->Push<int>( p->GetName(), static_cast< int >( translate::GetNumber( p->GetValue() ) ) );	
}

void CNrpConfigLooder::_ReadFloat( KeyPair* p )
{
	_config->Push<float>( p->GetName(), static_cast< float >( translate::GetNumber( p->GetValue() ) ) );	
}

void CNrpConfigLooder::_ReadBool( KeyPair* p )
{
	_config->Push<bool>( p->GetName(), p->GetValue().ToBool() );
}

void CNrpConfigLooder::_ReadString( KeyPair* p )
{
	_config->Push<NrpText>( p->GetName(), translate::GetTranslate( p->GetValue() ) );
}

void CNrpConfigLooder::_ReadTime( KeyPair* p )
{
	_config->Push<SYSTEMTIME>( p->GetName(), p->GetValue().ToTime() );
}

void CNrpConfigLooder::_ReadUser( KeyPair* p )
{
	IUser* user = CNrpApplication::Instance().GetUser( p->GetValue() );
	assert( user != NULL );
	if( user )
		_config->Push<PUser>( p->GetName(), user );
}

void CNrpConfigLooder::_ReadTechnology( KeyPair* p )
{
	PNrpTechnology tech = CNrpApplication::Instance().GetTechnology( p->GetValue() );
	assert( tech != NULL );
	if( tech )
		_config->Push<PNrpTechnology>( p->GetName(), tech );
}

void CNrpConfigLooder::_WriteUnknown( const CNrpProperty* prop, const NrpText& key, IniFile* ini )
{
	NrpText dd = const_cast< CNrpProperty* >( prop )->GetType();
	if( dd.find( L"enum" ) == 0 )
	{
		_WriteInt( prop, key, ini );
	}
	else 
		ini->Set( SECTION_PROPERTIES, key + L":unknown", NrpText("") );	
}

void CNrpConfigLooder::_WriteTechnology( const CNrpProperty* prop, const NrpText& key, IniFile* ini )
{
	const CNrpTechnology& tech = *(prop->As<PNrpTechnology>());
	
	if( CNrpApplication::Instance().GetTechnology( tech[ NAME ].As<NrpText>() ) )
	{
		ini->Set( SECTION_PROPERTIES, key + L":tech", (NrpText)tech[ NAME ] );
	}
}

void CNrpConfigLooder::_WriteUser( const CNrpProperty* prop, const NrpText& key, 
								   IniFile* ini )
{
	const PUser user = prop->As<PUser>();
	if( user )
		ini->Set( SECTION_PROPERTIES, key + L":user", (NrpText)(*user)[ NAME ] );
}

void CNrpConfigLooder::_WriteFloat( const CNrpProperty* prop, const NrpText& key, IniFile* ini )
{
	ini->Set( SECTION_PROPERTIES, key + ":float", prop->As<float>() );
}

void CNrpConfigLooder::_WriteTime( const CNrpProperty* prop, const NrpText& key, IniFile* ini )
{
	ini->Set( SECTION_PROPERTIES, key + L":time", prop->As<SYSTEMTIME>() );
}

void CNrpConfigLooder::_WriteString( const CNrpProperty* prop, const NrpText& key, IniFile* ini)
{
	ini->Set( SECTION_PROPERTIES, key + L":string", prop->As<NrpText>() );
}

void CNrpConfigLooder::_WriteBool( const CNrpProperty* prop, const NrpText& key, 
								   IniFile* ini )
{
	ini->Set( SECTION_PROPERTIES, key + L":bool", prop->As<bool>() );
}

void CNrpConfigLooder::_ReadUnknown( KeyPair* p )
{

}

void CNrpConfigLooder::_ReadDim2u( KeyPair* p )
{
	_config->Push<core::dimension2du>( p->GetName(), p->GetValue().ToDim2du() );
}
}//end namespace nrp
