#include "StdAfx.h"
#include "NrpProjectModule.h"
#include "IUser.h"
#include "INrpDevelopProject.h"
#include <assert.h>
#include <io.h>

namespace nrp
{

CNrpProjectModule::CNrpProjectModule( CNrpTechnology* pTech, INrpProject* pProject )
	: IWorkingModule( pTech->GetValue<PROJECT_TYPE>( TECHGROUP ), CLASS_PROJECTMODULE )
{
	InitializeOptions_();

	SetValue<std::string>( NAME, pTech->GetValue<std::string>( NAME ) );
	SetValue<int>( TECHGROUP, pTech->GetValue<int>( TECHGROUP ) );
	SetValue<int>( TECHTYPE, pTech->GetValue<int>( TECHTYPE ) );
	SetValue<float>( BASE_CODE, pTech->GetValue<float>( BASE_CODE ) );
	SetValue<float>( ENGINE_CODE, pTech->GetValue<float>( ENGINE_CODE ) );
	SetValue<std::string>( TEXTURENORMAL, pTech->GetValue<std::string>( TEXTURENORMAL ) );
	SetValue<int>( LEVEL, pTech->GetValue<int>( LEVEL ) );
	SetValue<INrpProject*>( PARENT, pProject );
	SetValue<int>( QUALITY, pTech->GetValue<int>( QUALITY ) );

	techRequires_ = pTech->GetTechRequires();
	skillRequires_ = pTech->GetSkillRequires();
}

CNrpProjectModule::CNrpProjectModule( PROJECT_TYPE type, INrpProject* pProject )
	: IWorkingModule( type, CLASS_PROJECTMODULE )
{
	InitializeOptions_();
	SetValue<int>( TECHTYPE, type );
	SetValue<PNrpProject>( PARENT, pProject );
}

CNrpProjectModule::~CNrpProjectModule(void)
{
}

void CNrpProjectModule::InitializeOptions_()
{
	CNrpTechnology::InitializeOptions_();

	CreateValue<IUser*>( LASTWORKER, NULL );
	CreateValue<IUser*>( COMPONENTLIDER, NULL );
	CreateValue<int>( CODEVOLUME, 0 );
	CreateValue<int>( CODEPASSED, 0 );
	CreateValue<int>( ERRORNUMBER, 0 );
	CreateValue<INrpDevelopProject*>( PARENT, NULL );
}

int CNrpProjectModule::AddUser( IUser* ptrUser )
{
	IUser* currentLider = GetValue<IUser*>( COMPONENTLIDER );
	IUser* lastLider = GetValue<IUser*>( LASTWORKER );

	//���� ������� �����������
	SetValue<IUser*>( LASTWORKER, currentLider );
	SetValue<IUser*>( COMPONENTLIDER, ptrUser  );

	if( lastLider && ptrUser && (lastLider != ptrUser) )
		SetValue<int>( CODEPASSED,(int)(GetValue<int>( CODEPASSED ) * 0.75f) );

	return 1;
}

void CNrpProjectModule::Update( IUser* ptrUser )
{
	INrpDevelopProject* parent = GetValue<INrpDevelopProject*>( PARENT );
	assert( parent != NULL );

	if( GetValue<int>( CODEPASSED ) < GetValue<int>( CODEVOLUME) )
	{
		int reqSkill = 0;
		REQUIRE_MAP::iterator sIter = skillRequires_.begin();
		for( ; sIter != skillRequires_.end(); sIter++ )
			reqSkill += ptrUser->GetSkill( sIter->first );

		float genreSkill = ptrUser->GetGenreExperience( GetValue<PROJECT_TYPE>( TECHTYPE ) ) / 100.f;

		if( genreSkill < 0.1f )
			genreSkill = 0.1f;
		float genrePref = ptrUser->GetGenrePreferences( GetValue<PROJECT_TYPE>( TECHTYPE ) ) / 100.f;
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
		parent->ModuleFinished( this, ptrUser );
}

void CNrpProjectModule::Save( std::string saveFolder )
{
	if( _access( saveFolder.c_str(), 0 ) == -1 )
		CreateDirectory( saveFolder.c_str(), NULL );

	std::string fileName = saveFolder + GetValue<std::string>( NAME ) + ".devmod";

	DeleteFile( fileName.c_str() );
	INrpProject::Save( SECTION_PROPERTIES, fileName );
	SaveRequires_( fileName );
}

void CNrpProjectModule::Load( std::string fileName )
{
	assert( _access( fileName.c_str(), 0 ) == 0 );
	if( _access( fileName.c_str(), 0 ) == -1 )
		OutputDebugString( ("��������� ���� �� ����������" + fileName).c_str() );
		
	INrpProject::Load( SECTION_PROPERTIES, fileName );
	LoadRequries_( fileName );

	PUser user = GetValue<PUser>( COMPONENTLIDER );
	if( user )
		user->AddWork( this, true );
}

int CNrpProjectModule::RemoveUser( const std::string& userName )
{
	return 1;
}
}//end namespace nrp