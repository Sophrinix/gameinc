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
	_native.Load( fileName );
}

void IniFile::Set( NrpText section, NrpText key, int amount )
{
	_native.SetKeyValue( section, key, NrpText( amount ) );
}
//////////////////////////////////////////////////////////////////////////

NrpText IniFile::Get( NrpText section, NrpText key, NrpText def_str )
{
	IniKey* kr = _native.GetKey( section, key);
	return kr ? kr->GetValue().c_str() : def_str;
}
//////////////////////////////////////////////////////////////////////////

int IniFile::Get( NrpText section, NrpText key, int def_value)
{
	IniKey* kr = _native.GetKey( section, key);
	return kr ? NrpText( kr->GetValue().c_str() ).ToInt() : def_value;
}

irr::core::dimension2di IniFile::Get( NrpText section, NrpText key, 
									  const irr::core::dimension2di& def_value)
{
	IniKey* kr = _native.GetKey( section, key );
	return kr ? NrpText( kr->GetValue().c_str() ).ToDim2di() : def_value;
}

irr::core::dimension2df IniFile::Get( NrpText section, NrpText key, 
									  const irr::core::dimension2df& def_value )
{
	IniKey* kr = _native.GetKey( section, key );
	return kr ? NrpText( kr->GetValue().c_str() ).ToDim2df() : def_value;
}

irr::core::vector3df IniFile::Get( NrpText section, NrpText key, 
								   const irr::core::vector3df& def_value )
{
	IniKey* kr = _native.GetKey( section, key );
	return kr ? NrpText( kr->GetValue().c_str() ).ToVector3df() : def_value;
}

irr::core::recti IniFile::Get( NrpText section, NrpText key, const irr::core::recti& def_value )
{
	IniKey* kr = _native.GetKey( section, key );
	return kr ? NrpText( kr->GetValue().c_str() ).ToRect() : def_value;
}

void IniFile::Set( NrpText section, NrpText key, const irr::core::recti& valuel )
{
	_native.SetKeyValue( section, key, NrpText( valuel ) );
}

void IniFile::Set( NrpText section, NrpText key, NrpText str_value )
{
	_native.SetKeyValue( section, key, str_value );
}

float IniFile::Get( NrpText section, NrpText key, float def_value )
{
	IniKey* kr = _native.GetKey( section, key );
	return kr ? NrpText( kr->GetValue().c_str() ).ToFloat() : def_value;
}

irr::core::dimension2du IniFile::Get( NrpText section, NrpText key, 
									  const irr::core::dimension2du& def_value )
{
	IniKey* kr = _native.GetKey( section, key );
	return kr ? NrpText( kr->GetValue().c_str() ).ToDim2du() : def_value;
}

bool IniFile::Get( NrpText section, NrpText key, bool def_value )
{
	NrpText result = Get( section, key, NrpText( "null" ) );

	if( result == L"0" || result == L"false" )
		return false;

	if( result == L"1" || result == L"true" )
		return true;

	return def_value;
}

NrpTime IniFile::Get( NrpText section, NrpText key, const NrpTime& def_value )
{
	NrpTime result( def_value );

	NrpText str_pars = Get( section, key, NrpText( L"y=0 m=0 d=0 h=0 mi=0 s=0" ) );
	swscanf_s( str_pars.ToWide(), L"y=%04d m=%02d d=%02d h=%02d mi=%02d s=%02d", 
								  &result.RYear(), &result.RMonth(), &result.RDay(),
								  &result.RHour(), &result.RMinute(), &result.RSecond() );
	result.RSecond() = 0;

	return result;	
}

void IniFile::Set( NrpText section, NrpText key, const irr::core::vector3df& amount )
{
	wchar_t str[ MAX_PATH ];

	swprintf_s( str, MAX_PATH, L"%.3f, %.3f, %.3f", amount.X, amount.Y, amount.Z );

	return Set( section, key, NrpText( str ) );
}

void IniFile::Set( NrpText section, NrpText key, const NrpTime& amount )
{
	wchar_t str[ MAX_PATH ];

	swprintf_s( str, MAX_PATH, L"y=%04d m=%02d d=%02d h=%02d mi=%02d s=0", 
					amount.RYear(), amount.RMonth(), amount.RDay(),
					amount.RHour(), amount.RMinute() );

	return Set( section, key, NrpText( str ) );
}

void IniFile::Set( NrpText section, NrpText key, float valuel )
{
	return _native.SetKeyValue( section, key, NrpText( valuel ) );
}

void IniFile::Get( NrpText sectionName, KNOWLEDGE_MAP& mapt )
{
	IniSection* section = _native.GetSection( sectionName );

	if( section )
	{
		typedef IniSection::KeyIndexA INDEXES;
		const INDEXES& indexes = section->GetKeys();
		
		for( INDEXES::const_iterator pIter=indexes.begin(); pIter != indexes.end(); pIter++ )
			mapt[ NrpText::LuaString( (*pIter)->GetKey().c_str() ) ] = NrpText( (*pIter)->GetValue().c_str() ).ToInt();
	}
}

void IniFile::Get( NrpText section, NrpText (*func)(int), u32 maxNum, STRINGS& art )
{
	for( u32 i=0; i < maxNum; i++ )
	{
		if( IniKey* key = GetKey( section, func( i ) ) )
			if( key->GetValue().size() > 0 )
				art.push_back( key->GetValue().c_str() );
			else
				Log( HW ) << "Value is null for key=" << key->GetKey().c_str() << " in file=" << _fileName << term;
		else
			break;
	}
}

void IniFile::Set( NrpText section, NrpText (*func)( int), const STRINGS& art )
{
	for( u32 i=0; i < art.size(); i++ )
		Set( section, func( i ), const_cast< STRINGS& >( art )[ i ] );
}

void IniFile::Set( NrpText section, const KNOWLEDGE_MAP& rmap )
{
	for( KNOWLEDGE_MAP::Iterator tIter = const_cast< KNOWLEDGE_MAP& >( rmap ).getIterator(); !tIter.atEnd() ; tIter++ )
		 Set( section, NrpText( tIter->getKey() ), NrpText( tIter->getValue() ) );
}

IniFile::~IniFile()
{
}

void IniFile::Save()
{
	_native.Save( _fileName );
}

IniKey* IniFile::GetKey( NrpText section, NrpText key )
{
	return _native.GetKey( section.ToStr(), key.ToStr() );
}

IniSection* IniFile::GetSection( NrpText section )
{
	return _native.GetSection( section.ToStr() );
}

} //namespace nrp