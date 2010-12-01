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

	Param( TECHGROUP ) = static_cast< int >( typen );
}

CNrpTechnology::CNrpTechnology( CNrpInvention* invention ) : INrpProject( CLASS_TECHNOLOGY, "" )
{
	_InitializeOptions();

	Param( STATUS ) = TS_READY;
	Load( invention->Text( BASEFILE ) );
	Param( PARENTCOMPANY ) = invention->Param( PARENTCOMPANY );
	Param( STARTDATE ) = invention->Param( ENDDATE );
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
	Add<NrpText>( NAME, "" );
	Add<NrpText>( INTERNAL_NAME, "" );
	Add<int>( TECHGROUP, 0 );
	Add<float>( BASE_CODE, 0 );
	Add<float>( ENGINE_CODE, 0 );
	Add<int>( LEVEL, 0 );
	Add<NrpText>( COMPANYNAME, "" );
	Add<int>( QUALITY, 100 );
	Add<PNrpCompany>( PARENTCOMPANY, NULL );
	Add<NrpText>( PARENT, "" );
	Add<NrpText>( BASEFILE, "" );
	Add<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	Add<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
	Add<float>( PRICE, 0 );
	Add<NrpText>( TEXTURENORMAL, "" );
	Add<NrpText>( TEXTUREHOVER, "" );
	Add<NrpText>( DESCRIPTIONPATH, "" );
	Add<float>( INTEREST, 1 );
	Add<int>( STATUS, TS_UNKNOWN );

	Add<int>( NEXTTECHNUMBER, 0 );
	Add<PNrpTechnology>( REQUIRETECH, NULL );
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

	NrpText fileName = OpFileSystem::CheckEndSlash( saveFolder ) + Text( INTERNAL_NAME ) + ".tech";
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
	Param( BASEFILE ) = fileName;
	IniFile rv( fileName );
	rv.Get( SECTION_REQUIRE_TECH, _techRequires );
	rv.Get( SECTION_REQUIRE_SKILL, _skillRequires );

	if( Param( STATUS ) == (int)TS_READY )
		rv.Get( SECTION_FUTURE_TECH, CreateKeyTech, (int)Param( NEXTTECHNUMBER ), _futureTech );
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