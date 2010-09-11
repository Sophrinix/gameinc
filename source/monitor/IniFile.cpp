#include "stdafx.h"
#include <stdio.h>
#include <sstream>
#include "IniFile.h"
#include "StrConversation.h"
#include "NrpTranslate.h"

namespace nrp
{

BOOL IniFile::Write( const std::string& section, 
						   const std::string& key, 
						   const int& amount, 
						   const std::string& file_name )
{
	char buf[ MAX_PATH ];

	_itoa_s( amount, buf, 10 );
	return WritePrivateProfileStringA( section.c_str(), 
									   key.c_str(), 
									   buf, 
									   file_name.c_str() );
}
//////////////////////////////////////////////////////////////////////////

std::string IniFile::Read( const std::string& section, 
								 const std::string& key, 
								 const std::string& def_str, 
								 const std::string& file_name )
{
	char buf[ MAX_PATH ];

	GetPrivateProfileStringA( section.c_str(),
							  key.c_str(), 
							  def_str.c_str(), 
							  buf, 
							  MAX_PATH, 
							  file_name.c_str() );

	return std::string( buf );
}
//////////////////////////////////////////////////////////////////////////

int IniFile::Read( const std::string& section, 
						 const std::string& key, 
						 const int& def_value, 
						 const std::string& file_name )
{
	return GetPrivateProfileIntA( section.c_str(),
							 	  key.c_str(), 
								  def_value,
								  file_name.c_str() );
}
//////////////////////////////////////////////////////////////////////////

irr::core::dimension2di IniFile::Read( const std::string& section, 
										     const std::string& key, 
											 const irr::core::dimension2di& def_value, 
											 const std::string& file_name )
{
	irr::core::dimension2di result( def_value );

	std::string str_pars = Read( section, key, std::string( "novalue" ), file_name );
	if( str_pars != "novalue" )
		sscanf_s( str_pars.c_str(), "%d,%d", &def_value.Width, &def_value.Height );

	return result;
}
//////////////////////////////////////////////////////////////////////////

irr::core::dimension2df IniFile::Read( const std::string& section, 
											 const std::string& key, 
											 const irr::core::dimension2df& def_value, 
											 const std::string& file_name )
{
	irr::core::dimension2df result( def_value );

	std::string str_pars = Read( section, key, std::string( "novalue" ), file_name );
	if( str_pars != "novalue" )
		sscanf_s( str_pars.c_str(), "%f,%f", &result.Width, &result.Height );

	return result;
}
//////////////////////////////////////////////////////////////////////////

irr::core::vector3df IniFile::Read( const std::string& section, 
										  const std::string& key, 
										  const irr::core::vector3df& def_value, 
										  const std::string& file_name )
{
	irr::core::vector3df result( def_value );

	std::string str_pars = Read( section, key, std::string( "novalue" ), file_name );
	if( str_pars != "novalue" )
		sscanf_s( str_pars.c_str(), "%f,%f,%f", &result.X, &result.Y, &result.Z );

	return result;
}
//////////////////////////////////////////////////////////////////////////

irr::core::recti IniFile::Read( const std::string& section,
									  const std::string& key,
									  const irr::core::recti& def_value,
									  const std::string& file_name )
{
	std::string str_pars = Read( section, key, std::string( "novalue" ), file_name );

	irr::core::recti rct( def_value );
	if( str_pars != "novalue" )
	sscanf_s( str_pars.c_str(), "%d,%d,%d,%d", &rct.UpperLeftCorner.X, &rct.UpperLeftCorner.Y, 
												&rct.LowerRightCorner.X, &rct.LowerRightCorner.Y  );

	return rct;
}
//////////////////////////////////////////////////////////////////////////

void IniFile::Write(const std::string& section, 
						  const std::string& key, 
						  const irr::core::recti& value, 
						  const std::string& file_name)
{
	std::ostringstream rectStrStream;
	rectStrStream << value.UpperLeftCorner.X << ", " << value.UpperLeftCorner.Y << ", " << value.LowerRightCorner.X << ", " << value.LowerRightCorner.Y;
	
	IniFile::Write(section, key, rectStrStream.str(), file_name);
}

//////////////////////////////////////////////////////////////////////////

void IniFile::Write( const std::string& section, 
						   const std::string& key,
						   const std::string& str_value, 
						   const std::string& file_name )
{
	WritePrivateProfileStringA( section.c_str(),
							    key.c_str(),
							    str_value.c_str(),
							    file_name.c_str() );
}
//////////////////////////////////////////////////////////////////////////

float IniFile::Read( const std::string& section,
						   const std::string& key, 
						   const float& def_value, 
						   const std::string& file_name )
{
	std::string str_pars = Read( section, key, std::string( "novalue" ), file_name );

	float result = def_value;
	if( str_pars != "novalue" )
	    sscanf_s( str_pars.c_str(), "%f", &result );

	return result;
}
//////////////////////////////////////////////////////////////////////////

irr::core::dimension2du IniFile::Read( const std::string& section, 
											 const std::string& key, 
											 const irr::core::dimension2du& def_value, 
											 const std::string& file_name )
{
	irr::core::dimension2du result( def_value );

	std::string str_pars = Read( section, key, std::string( "0,0" ), file_name );
	sscanf_s( str_pars.c_str(), "%d,%d", &result.Width, &result.Height );

	return result;
}
//////////////////////////////////////////////////////////////////////////

bool IniFile::Read( const std::string& section, 
						  const std::string& key, 
						  const bool& def_value, 
						  const std::string& file_name )
{
	std::string result = Read( section, key, std::string( "null" ), file_name );

	if( result == "0" || result == "false" )
		return false;

	if( result == "1" || result == "true" )
		return true;

	return def_value;
}

SYSTEMTIME IniFile::Read( const std::string& section, 
							const std::string& key, 
							const SYSTEMTIME& def_value, 
							const std::string& fileName )
{
	SYSTEMTIME result( def_value );

	std::string str_pars = Read( section, key, std::string( "y=0 m=0 d=0 h=0 mi=0 s=0" ), fileName );
	sscanf_s( str_pars.c_str(), "y=%04d m=%02d d=%02d h=%02d mi=%02d s=%02d", 
								&result.wYear, &result.wMonth, &result.wDay,
								&result.wHour, &result.wMinute, &result.wSecond );
	result.wSecond = 0;
	result.wMilliseconds = 0;

	return result;	
}
//////////////////////////////////////////////////////////////////////////

void IniFile::Write( const std::string& section, 
						   const std::string& key, 
						   const irr::core::vector3df& amount, 
						   const std::string& file_name )
{
	char str[ MAX_PATH ];

	sprintf_s( str, "%.3f, %.3f, %.3f", amount.X, amount.Y, amount.Z );

	Write( section, key, str, file_name );
}

void IniFile::Write( const std::string& section, 
						   const std::string& key, 
						   const SYSTEMTIME& amount, 
						   const std::string& fileName )
{
	char str[ MAX_PATH ];

	sprintf_s( str, "y=%04d m=%02d d=%02d h=%02d mi=%02d s=0", 
					amount.wYear, amount.wMonth, amount.wDay,
					amount.wHour, amount.wMinute );

	Write( section, key, str, fileName );
}

void IniFile::Write( const std::string& section, const std::string& key, const float& valuel, const std::string& file_name )
{
	std::ostringstream rectStrStream;
	rectStrStream << valuel;

	IniFile::Write(section, key, rectStrStream.str(), file_name);
}

void IniFile::ReadValueList_( std::string sectionName, REQUIRE_MAP& mapt, std::string fileName )
{
	char buffer[ 32000 ];
	memset( buffer, 0, 32000 );
	GetPrivateProfileSection( sectionName.c_str(), buffer, 32000, fileName.c_str() );

	std::string readLine = buffer;
	while( readLine != "" )
	{
		std::string name, valuel;
		name = readLine.substr( 0, readLine.find( '=' ) );
		valuel = readLine.substr( readLine.find( '=' ) + 1, 0xff );
		int keey = static_cast< int >( translate::GetNumber( name.c_str() ) );
		mapt[ keey ] = StrToInt( valuel.c_str() );

		memcpy( buffer, buffer + strlen(buffer) + 1, 32000 );  
		readLine = buffer;
	}
}

} //namespace nrp