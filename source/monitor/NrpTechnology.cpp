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

	_self[ STATUS ] = static_cast< int >( TS_READY );
	Load( (*invention)[ BASEFILE ] );
	_self[ PARENTCOMPANY ] = (*invention)[ PARENTCOMPANY ];
	_self[ STARTDATE ] = (*invention)[ ENDDATE ];
}

CNrpTechnology::CNrpTechnology( const NrpText& fileTech ) : INrpProject( CLASS_TECHNOLOGY, "" )
{
	_InitializeOptions();

	Load( fileTech );

	assert( ((NrpText)_self[ INTERNAL_NAME ]).size() );
}

CNrpTechnology::CNrpTechnology( const CNrpTechnology& p ) : INrpProject( CLASS_TECHNOLOGY, "" )
{
	int k=0;
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
	Add( STARTDATE, NrpTime( 0. ) );
	Add( ENDDATE, NrpTime( 0. ) );
	Add<float>( PRICE, 0 );
	Add<NrpText>( TEXTURENORMAL, "" );
	Add<NrpText>( TEXTUREHOVER, "" );
	Add<NrpText>( DESCRIPTIONPATH, "" );
	Add<float>( INTEREST, 1 );
	Add<int>( STATUS, TS_UNKNOWN );
	Add<int>( BALANCE, 0 );
	Add<int>( CODEPASSED, 0 );
	Add<CNrpUser*>( COMPONENTLIDER, NULL );
	Add<int>( ERRORNUMBER, 0 );
	Add<float>( READYWORKPERCENT, 0.f );
	Add<int>( CODEVOLUME, 0 );
	Add<int>( NEXTTECHNUMBER, 0 );
	Add<PNrpTechnology>( REQUIRETECH, NULL );
}

CNrpTechnology::~CNrpTechnology(void)
{
}

void CNrpTechnology::SetEngineTechRequire( const NrpText& techName, int valuel )
{
	_techRequires[ techName ] = valuel;
}

void CNrpTechnology::SetEmployerSkillRequire( const NrpText& skillName, int valuel )
{
	_skillRequires[ skillName ] = valuel;
}

int CNrpTechnology::GetEngineTechRequire( const NrpText& name )
{
	KNOWLEDGE_MAP::Node* node = _techRequires.find( name );
	return node ? node->getValue() : 0;
}

int CNrpTechnology::GetEployerSkillRequire( const NrpText& name )
{
	KNOWLEDGE_MAP::Node* node = _skillRequires.find( name );
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
	_self[ BASEFILE ] = fileName;
	IniFile rv( fileName );
	rv.Get( SECTION_REQUIRE_TECH, _techRequires );
	rv.Get( SECTION_REQUIRE_SKILL, _skillRequires );

	if( _self[ STATUS ] == (int)TS_READY )
		rv.Get( SECTION_FUTURE_TECH, CreateKeyTech, (int)_self[ NEXTTECHNUMBER ], _futureTech );
}

float CNrpTechnology::GetEmployerPosibility( CNrpUser* ptrUser )
{
	if( !ptrUser )
		return 0;
	
	int minSkill = 40;
	NrpText minSkillName = 0;
	float posibility = 0;
	KNOWLEDGE_MAP::Iterator sIter = _skillRequires.getIterator();
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