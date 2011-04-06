#include "stdafx.h"
#include <stdio.h>
#include <sstream>
#include "IniFile.h"
#include "NrpText.h"

namespace nrp
{

IniFile::IniFile( const NrpText& fileName )
{
	_fileName = fileName;
}

bool IniFile::Set( const NrpText& section, 
						   const NrpText& key, 
						   int amount )
{
	return WritePrivateProfileStringW( section.ToWide(), key.ToWide(), NrpText( amount ).ToWide(), _fileName.ToWide() ) == TRUE;
}
//////////////////////////////////////////////////////////////////////////

NrpText IniFile::Get( const NrpText& section, const NrpText& key, const NrpText& def_str )
{
	wchar_t buf[ MAX_PATH ];

	GetPrivateProfileStringW( section.ToWide(),
							  key.ToWide(), 
							  def_str.ToWide(), 
							  buf, 
							  MAX_PATH, 
							  _fileName.ToWide() );

	return NrpText( buf );
}
//////////////////////////////////////////////////////////////////////////

int IniFile::Get( const NrpText& section, 
						 const NrpText& key, 
						 int def_value)
{
	return GetPrivateProfileIntW( section.ToWide(),
							 	  key.ToWide(), 
								  def_value,
								  _fileName.ToWide() );
}
//////////////////////////////////////////////////////////////////////////

irr::core::dimension2di IniFile::Get( const NrpText& section, 
										     const NrpText& key, 
											 const irr::core::dimension2di& def_value)
{
	return Get( section, key, NrpText( def_value ) ).ToDim2di();
}
//////////////////////////////////////////////////////////////////////////

irr::core::dimension2df IniFile::Get( const NrpText& section, 
											 const NrpText& key, 
											 const irr::core::dimension2df& def_value )
{
	return Get( section, key, NrpText( def_value ) ).ToDim2df();
}
//////////////////////////////////////////////////////////////////////////

irr::core::vector3df IniFile::Get( const NrpText& section, 
										  const NrpText& key, 
										  const irr::core::vector3df& def_value )
{
	return Get( section, key, NrpText( def_value ) ).ToVector3df();
}
//////////////////////////////////////////////////////////////////////////

irr::core::recti IniFile::Get( const NrpText& section,
									  const NrpText& key,
									  const irr::core::recti& def_value )
{
	return Get( section, key, NrpText( def_value ) ).ToRect();
}
//////////////////////////////////////////////////////////////////////////

bool IniFile::Set(const NrpText& section, 
						  const NrpText& key, 
						  const irr::core::recti& value)
{
	return Set( section, key, NrpText( value ) );
}

//////////////////////////////////////////////////////////////////////////

bool IniFile::Set( const NrpText& section, 
						   const NrpText& key,
						   const NrpText& str_value )
{
	return WritePrivateProfileStringW( section.ToWide(),
							    key.ToWide(),
							    str_value.ToWide(),
							    _fileName.ToWide() ) == TRUE;
}
//////////////////////////////////////////////////////////////////////////

float IniFile::Get( const NrpText& section,
					const NrpText& key, 
					float def_value )
{
	return Get( section, key, NrpText( def_value ) ).ToFloat();
}
//////////////////////////////////////////////////////////////////////////

irr::core::dimension2du IniFile::Get( const NrpText& section, 
											 const NrpText& key, 
											 const irr::core::dimension2du& def_value )
{
	return Get( section, key, NrpText( def_value ) ).ToDim2du();
}
//////////////////////////////////////////////////////////////////////////

bool IniFile::Get( const NrpText& section, 
						  const NrpText& key, 
						  bool def_value )
{
	NrpText result = Get( section, key, NrpText( "null" ) );

	if( result == L"0" || result == L"false" )
		return false;

	if( result == L"1" || result == L"true" )
		return true;

	return def_value;
}

NrpTime IniFile::Get( const NrpText& section, 
							const NrpText& key, 
							const NrpTime& def_value )
{
	NrpTime result( def_value );

	NrpText str_pars = Get( section, key, NrpText( L"y=0 m=0 d=0 h=0 mi=0 s=0" ) );
	swscanf_s( str_pars.ToWide(), L"y=%04d m=%02d d=%02d h=%02d mi=%02d s=%02d", 
								  &result.RYear(), &result.RMonth(), &result.RDay(),
								  &result.RHour(), &result.RMinute(), &result.RSecond() );
	result.RSecond() = 0;

	return result;	
}
//////////////////////////////////////////////////////////////////////////

bool IniFile::Set( const NrpText& section, 
						   const NrpText& key, 
						   const irr::core::vector3df& amount )
{
	wchar_t str[ MAX_PATH ];

	swprintf_s( str, MAX_PATH, L"%.3f, %.3f, %.3f", amount.X, amount.Y, amount.Z );

	return Set( section, key, NrpText( str ) );
}

bool IniFile::Set( const NrpText& section, 
						   const NrpText& key, 
						   const NrpTime& amount )
{
	wchar_t str[ MAX_PATH ];

	swprintf_s( str, MAX_PATH, L"y=%04d m=%02d d=%02d h=%02d mi=%02d s=0", 
					amount.RYear(), amount.RMonth(), amount.RDay(),
					amount.RHour(), amount.RMinute() );

	return Set( section, key, NrpText( str ) );
}

bool IniFile::Set( const NrpText& section, const NrpText& key, float valuel )
{
	return IniFile::Set(section, key, NrpText( valuel ) );
}

void IniFile::Get( const NrpText& sectionName, KNOWLEDGE_MAP& mapt )
{
	wchar_t buffer[ 32000 ];
	memset( buffer, 0, 32000 );
	GetPrivateProfileSectionW( sectionName.ToWide(), buffer, 32000, _fileName.ToWide() );

	NrpText readLine = buffer;
	while( readLine.size() )
	{
		NrpText name, valuel;
		name = readLine.subString( 0, readLine.findFirst( L'=' ) );
		valuel = readLine.subString( readLine.findFirst( L'=' ) + 1, 0xff );
		mapt[ NrpText::LuaString( name ) ] = valuel.ToInt();

		memcpy( buffer, buffer + wcslen(buffer) + 1, 32000 );  
		readLine = buffer;
	}
}

void IniFile::Get( const NrpText& section, NrpText (*func)(int), u32 maxNum, STRINGS& art )
{
	for( u32 i=0; i < maxNum; i++ )
		art.push_back( Get( section, func( i ), NrpText() ) );
}

void IniFile::Set( const NrpText& section, NrpText (*func)( int), const STRINGS& art )
{
	for( u32 i=0; i < art.size(); i++ )
		Set( section, func( i ), art[ i ] );
}

void IniFile::Set( const NrpText& section, const KNOWLEDGE_MAP& rmap )
{
	for( KNOWLEDGE_MAP::Iterator tIter = const_cast< KNOWLEDGE_MAP& >( rmap ).getIterator(); !tIter.atEnd() ; tIter++ )
		 Set( section, NrpText( tIter->getKey() ), NrpText( tIter->getValue() ) );
}

} //namespace nrp