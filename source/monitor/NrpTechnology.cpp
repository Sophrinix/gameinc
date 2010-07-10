#include "StdAfx.h"
#include "NrpTechnology.h"
#include "NrpGameProject.h"
#include "IUser.h"
#include "NrpCompany.h"
#include "IniFile.h"

#include <io.h>
#include <errno.h>
#include <assert.h>

namespace nrp
{

CNrpTechnology::CNrpTechnology( PROJECT_TYPE typen ) : INrpProject( "CNrpTechnology", "" )
{
	InitializeOptions_();

	SetValue<int>( TECHGROUP, typen );
}

void CNrpTechnology::InitializeOptions_()
{
	CreateValue<std::string>( NAME, "" );
	CreateValue<int>( TECHGROUP, 0 );
	CreateValue<int>( TECHTYPE, 0 );
	CreateValue<float>( BASE_CODE, 0 );
	CreateValue<float>( ENGINE_CODE, 0 );
	CreateValue<int>( LEVEL, 0 );
	CreateValue<std::string>( COMPANYNAME, "" );
	CreateValue<int>( QUALITY, 100 );
	CreateValue<PNrpCompany>( PARENTCOMPANY, NULL );
	CreateValue<std::string>( PARENT, "" );
	CreateValue<std::string>( BASEFILE, "" );
	CreateValue<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	CreateValue<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
	CreateValue<float>( PRICE, 0 );
	CreateValue<std::string>( TEXTURENORMAL, "" );
	CreateValue<std::string>( TEXTUREHOVER, "" );
	CreateValue<std::string>( DESCRIPTIONPATH, "" );
	CreateValue<float>( INTEREST, 1 );
	CreateValue<TECH_STATUS>( STATUS, TS_UNKNOWN );

	CreateValue<int>( NEXTTECHNUMBER, 0 );
	CreateValue<PNrpTechnology>( REQUIRETECH, NULL );
}


CNrpTechnology::~CNrpTechnology(void)
{
}

void CNrpTechnology::SetEngineTechRequire( int tech_type, int valuel )
{
	techRequires_[ tech_type ] = valuel;
}

void CNrpTechnology::SetEmployerSkillRequire( int skill_type, int valuel )
{
	skillRequires_[ skill_type ] = valuel;
}

int CNrpTechnology::GetEngineTechRequire( int tech_type )
{
	if( techRequires_.find( tech_type ) == techRequires_.end() )
		return 0;

	return techRequires_[ tech_type ];
}

int CNrpTechnology::GetEployerSkillRequire( int skil_require )
{
	if( skillRequires_.find( skil_require ) == skillRequires_.end() )
		return 0;

	return skillRequires_[ skil_require ];
}

void CNrpTechnology::Save( std::string saveFolder )
{
	if( _access( saveFolder.c_str(), 0 ) == -1 )
		CreateDirectory( saveFolder.c_str(), NULL );

	std::string fileName = saveFolder + GetValue<std::string>( NAME ) + ".tech";

	DeleteFile( fileName.c_str() );
	INrpProject::Save( PROPERTIES, fileName );
	SaveRequires_( fileName );

	for( size_t pos=0; pos < futureTech_.size(); pos++ )
	{
		std::string keyvalue = "";
		switch( futureTech_[ pos ]->GetValue<TECH_STATUS>( STATUS ) )
		{
		case TS_PROJECT:
			keyvalue = "project:" + futureTech_[ pos ]->GetValue<std::string>( BASEFILE );
			break;
		case TS_READY:
			keyvalue = "ready:" + futureTech_[ pos ]->GetValue<std::string>( NAME );
			break;
		case TS_INDEVELOP:
			keyvalue = "indevelop:" + futureTech_[ pos ]->GetValue<std::string>( NAME );
			break;
		default:
			throw "unresolved function";
		}

		IniFile::Write( "nexttechs", "tech_" + IntToStr( pos ), keyvalue, fileName );
	}
}

void CNrpTechnology::Load( std::string fileName )
{
	INrpProject::Load( PROPERTIES, fileName );
	LoadRequries_( fileName );

	if( GetValue<TECH_STATUS>( STATUS ) == TS_READY )
	{
		for( int cnt=0; cnt < GetValue<int>( NEXTTECHNUMBER ); cnt++ )
		{
			std::string tmpStr = IniFile::Read( "nexttechs", "tech_"+IntToStr( cnt ), std::string(""), fileName );
			std::string type = tmpStr.substr( 0, tmpStr.find( ':' ) );
			std::string name = tmpStr.substr( tmpStr.find( ':' )+1, MAX_PATH );

			if( type == "project" )
			{
				CNrpTechnology* pTech = new CNrpTechnology( PT_UNKNOWN );
				pTech->Load( name + "/item.tech" );
				pTech->SetValue<std::string>( BASEFILE, name );
				pTech->SetValue<std::string>( PARENT, GetValue<std::string>( NAME ) );
				pTech->SetValue<TECH_STATUS>( STATUS, TS_PROJECT );
				futureTech_.push_back( pTech );
			}
		}
	}
}

float CNrpTechnology::GetEmployerPosibility( IUser* ptrUser )
{
	if( !ptrUser )
		return 0;
	
	int minSkill = 40;
	int minSkillName = 0;
	float posibility = 0;
	REQUIRE_MAP::iterator sIter = skillRequires_.begin();
	for( ; sIter != skillRequires_.end(); sIter++ )
	{
		int skillValue = ptrUser->GetSkill( sIter->first );
		if( skillValue < minSkill )
		{
			minSkill = skillValue;
			minSkillName = sIter->first;
		}

		posibility += ( skillValue > 100 ? 100 : skillValue ) / 100.f;
		posibility /= 2.f;
	}

	return posibility;
}

void CNrpTechnology::LoadRequries_( const std::string& fileName )
{
	IniFile::ReadValueList_( "techRequire", techRequires_, fileName );
	IniFile::ReadValueList_( "skillRequire", skillRequires_, fileName );
}

void CNrpTechnology::SaveRequires_( const std::string& fileName )
{
	for( REQUIRE_MAP::iterator tIter = techRequires_.begin(); tIter != techRequires_.end(); tIter++ )
		IniFile::Write( "techRequire", IntToStr( tIter->first ), IntToStr( tIter->second ), fileName );

	for( REQUIRE_MAP::iterator sIter = skillRequires_.begin(); sIter != skillRequires_.end(); sIter++ )
		IniFile::Write( "skillRequire", IntToStr( sIter->first ), IntToStr( sIter->second ), fileName );
}

CNrpTechnology* CNrpTechnology::GetFutureTech( size_t index )
{
	return index < futureTech_.size() ? futureTech_[ index ] : NULL;
}

void CNrpTechnology::AddFutureTech( CNrpTechnology* tech )
{
	futureTech_.push_back( tech );
}
}//namespace nrp