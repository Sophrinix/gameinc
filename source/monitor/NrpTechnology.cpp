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

CNrpTechnology::CNrpTechnology( CNrpInvention& invention ) : INrpProject( CLASS_TECHNOLOGY, "" )
{
	_InitializeOptions();

	_self[ STATUS ] = static_cast< int >( TS_READY );
	Load( invention[ BASEFILE ] );
	_self[ PARENTCOMPANY ] = invention[ PARENTCOMPANY ];
	_self[ STARTDATE ] = invention[ ENDDATE ];
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
	RegProperty<NrpText>( NAME, "" );
	RegProperty<NrpText>( INTERNAL_NAME, "" );
	RegProperty<int>( TECHGROUP, 0 );
	RegProperty<float>( BASE_CODE, 0 );
	RegProperty<float>( ENGINE_CODE, 0 );
	RegProperty<int>( LEVEL, 0 );
	RegProperty<NrpText>( COMPANYNAME, "" );
	RegProperty<int>( QUALITY, 100 );
	RegProperty<PNrpCompany>( PARENTCOMPANY, NULL );
	RegProperty<NrpText>( PARENT, "" );
	RegProperty<NrpText>( BASEFILE, "" );
	RegProperty( STARTDATE, NrpTime( 0. ) );
	RegProperty( ENDDATE, NrpTime( 0. ) );
	RegProperty<float>( PRICE, 0 );
	RegProperty<NrpText>( TEXTURENORMAL, "" );
	RegProperty<NrpText>( TEXTUREHOVER, "" );
	RegProperty<NrpText>( DESCRIPTIONPATH, "" );
	RegProperty<float>( INTEREST, 1 );
	RegProperty<int>( STATUS, TS_UNKNOWN );
	RegProperty<int>( BALANCE, 0 );
	RegProperty<int>( CODEPASSED, 0 );
	RegProperty<CNrpUser*>( COMPONENTLIDER, NULL );
	RegProperty<int>( ERRORNUMBER, 0 );
	RegProperty<float>( READYWORKPERCENT, 0.f );
	RegProperty<int>( CODEVOLUME, 0 );
	RegProperty<int>( NEXTTECHNUMBER, 0 );
	RegProperty<PNrpTechnology>( REQUIRETECH, NULL );
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
	
	INrpProject::Save( fileName );

	IniFile sv( fileName );
	sv.Set( SECTION_FUTURE_TECH, CreateKeyTech, _futureTech );
	sv.Set( SECTION_REQUIRE_TECH, _techRequires );
	sv.Set( SECTION_REQUIRE_SKILL, _skillRequires );

	sv.Save();

	return fileName;
}

void CNrpTechnology::Load( const NrpText& fileName )
{
	INrpProject::Load( fileName );
	_self[ BASEFILE ] = fileName;
	IniFile rv( fileName );
	rv.Get( SECTION_REQUIRE_TECH, _techRequires );
	rv.Get( SECTION_REQUIRE_SKILL, _skillRequires );

	rv.Get( SECTION_FUTURE_TECH, CreateKeyTech, -1, _futureTech );
	_self[ NEXTTECHNUMBER ] = static_cast< int >( _futureTech.size() );
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