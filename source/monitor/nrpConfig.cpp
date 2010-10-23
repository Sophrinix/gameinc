#include "stdafx.h"
#include "nrpConfig.h"
#include "NrpTranslate.h"
#include "IUser.h"
#include "NrpApplication.h"
#include "OpFileSystem.h"

namespace nrp
{

void CheckClassesType( const std::string type1, const std::string type2 )
{
//	assert( type1 == type2 );
	if( type1 != type2 ) 
		Log(HW) << "warning: request type " << type2 << " but native typename is " << type1 << term;
}

void INrpConfig::Load( const std::string& fileName )
{
	assert( OpFileSystem::IsExist( fileName ) );
	char buffer[ 32000 ];
	memset( buffer, 0, 32000 );
	GetPrivateProfileSection( SECTION_PROPERTIES.c_str(), buffer, 32000, fileName.c_str() );

	std::string readLine = buffer;
	while( readLine != "" )
	{
		std::string name, valuel, type;
		name = readLine.substr( 0, readLine.find( '=' ) );
		type = name.substr( name.find( ':' ) + 1, 0xff );
		name = name.substr( 0, name.find( ':' ) );
		valuel = readLine.substr( readLine.find( '=' ) + 1, 0xff );
		if( type == "int" )
			CreateValue<int>( name, static_cast< int >( translate::GetNumber( valuel.c_str() ) ) );
		else if( type == "float" )
			CreateValue<float>( name, static_cast< float >( translate::GetNumber( valuel.c_str() ) ) ); 
		else if( type == "bool" )
			CreateValue<bool>( name, valuel == "true" );
		else if( type == "string" )
			CreateValue<std::string>( name, translate::GetTranslate( valuel.c_str() ) );
		else if( type == "time" )
			CreateValue<SYSTEMTIME>( name, StrToTime( valuel.c_str()) );
		else if( type == "user" )
		{
			IUser* user = CNrpApplication::Instance().GetUser( valuel );
			if( user )
				CreateValue<PUser>( name, user );
		}

		memcpy( buffer, buffer + strlen(buffer) + 1, 32000 );  
		readLine = buffer;
	}
}

std::string INrpConfig::Save( const std::string& fileName )
{
	IniFile::Write( "test", "test", std::string("test"), fileName );
	assert( OpFileSystem::IsExist( fileName ) );

	PropertyArray::iterator paIter = options_.begin();
	for( ; paIter != options_.end(); paIter++)
	{
		INrpProperty* prop = paIter->second;
		if( prop->GetValueType() == typeid( int ).name() || prop->GetValueType().find( "enum" ) == 0 )
			IniFile::Write( SECTION_PROPERTIES, paIter->first + ":int", 
							((CNrpProperty<int>*)prop)->GetValue(), fileName );
		else
			if( prop->GetValueType() == typeid( std::string ).name() )
				IniFile::Write( SECTION_PROPERTIES, paIter->first + ":string", 
								((CNrpProperty<std::string>*)prop)->GetValue(), fileName );
		else
			if( prop->GetValueType() == typeid( bool ).name() )
				IniFile::Write( SECTION_PROPERTIES, paIter->first + ":bool", 
								std::string(((CNrpProperty<bool>*)prop)->GetValue() ? "true" : "false"), fileName );
		else
			if( prop->GetValueType() == typeid( SYSTEMTIME ).name() )
				IniFile::Write( SECTION_PROPERTIES, paIter->first + ":time", 
								((CNrpProperty<SYSTEMTIME>*)prop)->GetValue(), fileName );
		else 
			if( prop->GetValueType() == typeid( float ).name() )
				IniFile::Write( SECTION_PROPERTIES, 
								paIter->first + ":float", ((CNrpProperty<float>*)prop)->GetValue(), fileName );
		else
			if( prop->GetValueType() == typeid( PUser ).name())
			{
				PUser user = ((CNrpProperty<PUser>*)prop)->GetValue();
				if( user )
					IniFile::Write( SECTION_PROPERTIES, paIter->first + ":user", user->GetValue<std::string>( NAME ), fileName );
			}
		else
			IniFile::Write( SECTION_PROPERTIES, paIter->first + ":unknown", std::string(""), fileName );			
	}

	return fileName;
}

}//end namespace nrp