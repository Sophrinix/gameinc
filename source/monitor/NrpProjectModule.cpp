#include "StdAfx.h"
#include "NrpProjectModule.h"
#include "IUser.h"
#include "INrpDevelopProject.h"
#include "OpFileSystem.h"
#include "IniFile.h"

#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_PROJECTMODULE( "CNrpProjectModule" );

CNrpProjectModule::CNrpProjectModule( CNrpTechnology* pTech, INrpProject* pProject )
	: IWorkingModule( pTech->GetValue<PROJECT_TYPE>( TECHGROUP ), CLASS_PROJECTMODULE )
{
	InitializeOptions_();

	SetValue<NrpText>( NAME, pTech->GetString( NAME ) );
	SetValue<int>( TECHGROUP, pTech->GetValue<int>( TECHGROUP ) );
	SetValue<int>( TECHTYPE, pTech->GetValue<int>( TECHTYPE ) );
	SetValue<float>( BASE_CODE, pTech->GetValue<float>( BASE_CODE ) );
	SetValue<float>( ENGINE_CODE, pTech->GetValue<float>( ENGINE_CODE ) );
	SetValue<NrpText>( INTERNAL_NAME, pTech->GetString( INTERNAL_NAME) );
	SetValue<NrpText>( TEXTURENORMAL, pTech->GetString( TEXTURENORMAL ) );
	SetValue<int>( LEVEL, pTech->GetValue<int>( LEVEL ) );
	SetValue<INrpProject*>( PARENT, pProject );
	SetValue<int>( QUALITY, pTech->GetValue<int>( QUALITY ) );

	CopyMapTo( _techRequires, pTech->GetTechRequires() );
	CopyMapTo( _skillRequires, pTech->GetSkillRequires() );
}

CNrpProjectModule::CNrpProjectModule( PROJECT_TYPE type, INrpDevelopProject* pProject )
	: IWorkingModule( type, CLASS_PROJECTMODULE )
{
	InitializeOptions_();
	SetValue<int>( TECHTYPE, type );
	SetValue<INrpDevelopProject*>( PARENT, pProject );
}

CNrpProjectModule::CNrpProjectModule() : IWorkingModule( PROJECT_TYPE(0), CLASS_PROJECTMODULE )
{

}

CNrpProjectModule::~CNrpProjectModule(void)
{
}

void CNrpProjectModule::InitializeOptions_()
{
	CNrpTechnology::_InitializeOptions();

	Push<IUser*>( LASTWORKER, NULL );
	Push<IUser*>( COMPONENTLIDER, NULL );
	Push<int>( CODEVOLUME, 0 );
	Push<int>( CODEPASSED, 0 );
	Push<int>( ERRORNUMBER, 0 );
	Push<int>( USERNUMBER, 0 );
	Pop( PARENT );
	Push<INrpDevelopProject*>( PARENT, NULL );
}

int CNrpProjectModule::AddUser( IUser* ptrUser )
{
	if( GetValue<float>( READYWORKPERCENT ) < 1 )
	{
		_users.push_back( ptrUser );
		SetValue( USERNUMBER, _users.size() );
	}

	return (GetValue<float>( READYWORKPERCENT ) < 1);
}

void CNrpProjectModule::Update( IUser* ptrUser )
{
	INrpDevelopProject* parent = GetValue<INrpDevelopProject*>( PARENT );
	assert( parent != NULL );

	if( GetValue<int>( CODEPASSED ) < GetValue<int>( CODEVOLUME) )
	{
		int reqSkill = 0;
		REQUIRE_MAP::Iterator sIter = _skillRequires.getIterator();
		float teamKoeff = _GetWorkKoeffForUser( ptrUser );
		for( ; !sIter.atEnd(); sIter++ )
			reqSkill += ptrUser->GetSkill( sIter->getKey() );

		if( reqSkill < 10 )
			reqSkill = 10;

		//коэффициент команды нужен из-за того, что отимальный размер команды 5, а дальнейший рост 
		//приводит к ухудшению производительности каждого из членов команды,
		//хотя общая растет
		float genreSkill = teamKoeff * ptrUser->GetGenreExperience( GetValue<PROJECT_TYPE>( TECHTYPE ) ) / 100.f;

		if( genreSkill < 0.1f )
			genreSkill = 0.1f;
		float genrePref = teamKoeff * ptrUser->GetGenrePreferences( GetValue<PROJECT_TYPE>( TECHTYPE ) ) / 100.f;
		
		if( genrePref < 0.1f )
			genrePref = 0.1f;


		int codePassed = GetValue<int>( CODEPASSED ) + (int)(reqSkill * (genrePref + genreSkill));
		if( codePassed >= GetValue<int>( CODEVOLUME ) )
			codePassed = GetValue<int>( CODEVOLUME);

		SetValue<int>( CODEPASSED, codePassed );
		SetValue<float>( READYWORKPERCENT, codePassed / (float)GetValue<int>( CODEVOLUME ) );
		int quality = GetValue<int>( QUALITY );
		SetValue<int>( QUALITY, (quality + ptrUser->GetValue<int>( CODE_QUALITY )) / 2 );

		parent->AddValue<int>( MONEYONDEVELOP, ptrUser->GetValue<int>( SALARY ) / (20*9) );
	}

	if( GetValue<float>( READYWORKPERCENT ) >= 1 )
		parent->ModuleFinished( this );
}

float CNrpProjectModule::_GetWorkKoeffForUser( IUser* ptrUser )
{
	float teamKoef[10] = { 1.f/*1*/, 4.f/*2*/, 3.f/*3*/, 2.f/*4*/, 1.f/*5*/, 0.8f/*6*/, 0.65f/*7*/, 0.5f/*8*/, 0.25f/*9*/, 0.1f/*10 and more*/};
	return teamKoef[ _users.size() >= 9 ? 9 : _users.size() ];
}

NrpText CNrpProjectModule::Save( const NrpText& saveFolder )
{
	OpFileSystem::CreateDirectory( saveFolder );

	NrpText fileName = saveFolder + GetString( NAME ) + ".devmod";
	assert( !OpFileSystem::IsExist( fileName ) );

	INrpProject::Save( fileName );
	IniFile sv( fileName );

	sv.Set( SECTION_REQUIRE_TECH, _techRequires );
	sv.Set( SECTION_REQUIRE_SKILL, _skillRequires );

	return saveFolder;
}

void CNrpProjectModule::Load( const NrpText& fileName )
{
	assert( OpFileSystem::IsExist( fileName ) );
		
	INrpProject::Load( fileName );
	IniFile rv( fileName );
	rv.Get( SECTION_REQUIRE_TECH, _techRequires );
	rv.Get( SECTION_REQUIRE_SKILL, _skillRequires );
}

int CNrpProjectModule::RemoveUser( const NrpText& userName )
{
	for( u32 i=0; i < _users.size(); i++ )
		if( _users[ i ]->Equale( userName ) )
		{
			_users.erase( i );
			return 1;
		}

	return 0;
}

NrpText CNrpProjectModule::ClassName()
{
	return CLASS_PROJECTMODULE;
}

}//end namespace nrp