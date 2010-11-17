#include "StdAfx.h"
#include "NrpTechnology.h"
#include "NrpGameProject.h"
#include "NrpInvention.h"
#include "IUser.h"
#include "NrpCompany.h"
#include "IniFile.h"
#include "OpFileSystem.h"

#include <errno.h>
#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_TECHNOLOGY( "CNrpTechnology" );

CNrpTechnology::CNrpTechnology( PROJECT_TYPE typen, const CLASS_NAME& className ) : INrpProject( className.size() ? className : CLASS_TECHNOLOGY, "" )
{
	assert( className.size() );
	_InitializeOptions();

	SetValue<int>( TECHGROUP, typen );
}

CNrpTechnology::CNrpTechnology( CNrpInvention* invention ) : INrpProject( CLASS_TECHNOLOGY, "" )
{
	_InitializeOptions();

	SetValue<TECH_STATUS>( STATUS, TS_READY );
	Load( invention->GetString( BASEFILE ) );
	SetValue<PNrpCompany>( PARENTCOMPANY, invention->GetValue<PNrpCompany>( PARENTCOMPANY ) );
	SetValue<SYSTEMTIME>( STARTDATE, invention->GetValue<SYSTEMTIME>( ENDDATE ) );
}

CNrpTechnology::CNrpTechnology( const NrpText& fileTech ) : INrpProject( CLASS_TECHNOLOGY, "" )
{
	_InitializeOptions();

	Load( fileTech );
}

CNrpTechnology::CNrpTechnology( const CNrpTechnology& p ) : INrpProject( CLASS_TECHNOLOGY, "" )
{

}
void CNrpTechnology::_InitializeOptions()
{
	Push<NrpText>( NAME, "" );
	Push<NrpText>( INTERNAL_NAME, "" );
	Push<int>( TECHGROUP, 0 );
	Push<int>( TECHTYPE, 0 );
	Push<float>( BASE_CODE, 0 );
	Push<float>( ENGINE_CODE, 0 );
	Push<int>( LEVEL, 0 );
	Push<NrpText>( COMPANYNAME, "" );
	Push<int>( QUALITY, 100 );
	Push<PNrpCompany>( PARENTCOMPANY, NULL );
	Push<NrpText>( PARENT, "" );
	Push<NrpText>( BASEFILE, "" );
	Push<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	Push<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
	Push<float>( PRICE, 0 );
	Push<NrpText>( TEXTURENORMAL, "" );
	Push<NrpText>( TEXTUREHOVER, "" );
	Push<NrpText>( DESCRIPTIONPATH, "" );
	Push<float>( INTEREST, 1 );
	Push<TECH_STATUS>( STATUS, TS_UNKNOWN );

	Push<int>( NEXTTECHNUMBER, 0 );
	Push<PNrpTechnology>( REQUIRETECH, NULL );
}

CNrpTechnology::~CNrpTechnology(void)
{
}

void CNrpTechnology::SetEngineTechRequire( int tech_type, int valuel )
{
	_techRequires[ tech_type ] = valuel;
}

void CNrpTechnology::SetEmployerSkillRequire( int skill_type, int valuel )
{
	_skillRequires[ skill_type ] = valuel;
}

int CNrpTechnology::GetEngineTechRequire( int tech_type )
{
	REQUIRE_MAP::Node* node = _techRequires.find( tech_type );
	return node ? node->getValue() : 0;
}

int CNrpTechnology::GetEployerSkillRequire( int skil_require )
{
	REQUIRE_MAP::Node* node = _skillRequires.find( skil_require );
	return node ? node->getValue() : 0;
}

NrpText CNrpTechnology::Save( const NrpText& saveFolder )
{
	OpFileSystem::CreateDirectory( saveFolder );

	NrpText fileName = OpFileSystem::CheckEndSlash( saveFolder ) + GetString( INTERNAL_NAME ) + ".tech";
	//не должно быть файла с такимже именем в директории
	assert( !OpFileSystem::IsExist( fileName ) );

	IniFile sv( fileName );
	sv.Set( SECTION_FUTURE_TECH, CreateKeyTech, _futureTech );
	sv.Set( SECTION_REQUIRE_TECH, _techRequires );
	sv.Set( SECTION_REQUIRE_SKILL, _skillRequires );

	return INrpProject::Save( fileName );
}

void CNrpTechnology::Load( const NrpText& fileName )
{
	INrpProject::Load( fileName );
	SetString( BASEFILE, fileName );
	IniFile rv( fileName );
	rv.Get( SECTION_REQUIRE_TECH, _techRequires );
	rv.Get( SECTION_REQUIRE_SKILL, _skillRequires );

	if( GetValue<TECH_STATUS>( STATUS ) == TS_READY )
		rv.Get( SECTION_FUTURE_TECH, CreateKeyTech, GetValue<int>( NEXTTECHNUMBER ), _futureTech );
}

float CNrpTechnology::GetEmployerPosibility( IUser* ptrUser )
{
	if( !ptrUser )
		return 0;
	
	int minSkill = 40;
	int minSkillName = 0;
	float posibility = 0;
	REQUIRE_MAP::Iterator sIter = _skillRequires.getIterator();
	for( ; !sIter.atEnd(); sIter++ )
	{
		int skillValue = ptrUser->GetSkill( sIter->getKey() );
		if( skillValue < minSkill )
		{
			minSkill = skillValue;
			minSkillName = sIter->getKey();
		}

		posibility += ( skillValue > 100 ? 100 : skillValue ) / 100.f;
		posibility /= 2.f;
	}

	return posibility;
}

const NrpText CNrpTechnology::GetFutureTech( size_t index )
{
	return index < _futureTech.size() ? _futureTech[ index ] : "";
}

void CNrpTechnology::AddFutureTech( const NrpText& name )
{
	_futureTech.push_back( name );
}

NrpText CNrpTechnology::ClassName()
{
	return CLASS_TECHNOLOGY;
}
}//namespace nrp