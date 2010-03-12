#include "stdafx.h"
#include <stdio.h>
#include <sstream>
#include "IniFile.h"

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

	std::string str_pars = Read( section, key, std::string( "0,0" ), file_name );
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

	std::string str_pars = Read( section, key, std::string( "0,0" ), file_name );
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

	std::string str_pars = Read( section, key, std::string( "0,0,0" ), file_name );
	sscanf_s( str_pars.c_str(), "%f,%f,%f", &result.X, &result.Y, &result.Z );

	return result;
}
//////////////////////////////////////////////////////////////////////////

irr::core::recti IniFile::Read( const std::string& section,
									  const std::string& key,
									  const irr::core::recti& def_value,
									  const std::string& file_name )
{
	std::string str_pars = Read( section, key, std::string( "0,0,0,0" ), file_name );

	irr::core::recti rct( def_value );
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
	std::string str_pars = Read( section, key, std::string( "0.0" ), file_name );

	float result = def_value;
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
//////////////////////////////////////////////////////////////////////////

} //namespace nrp