#include "StdAfx.h"
#include "NrpProjectModule.h"
#include "IUser.h"
#include "INrpDevelopProject.h"
#include <assert.h>
#include "OpFileSystem.h"

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

CNrpProjectModule::CNrpProjectModule( PROJECT_TYPE type, INrpDevelopProject* pProject )
	: IWorkingModule( type, CLASS_PROJECTMODULE )
{
	InitializeOptions_();
	SetValue<int>( TECHTYPE, type );
	SetValue<INrpDevelopProject*>( PARENT, pProject );
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
	CreateValue<int>( USERNUMBER, 0 );
	EraseValue( PARENT );
	CreateValue<INrpDevelopProject*>( PARENT, NULL );
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
		REQUIRE_MAP::iterator sIter = skillRequires_.begin();
		float teamKoeff = _GetWorkKoeffForUser( ptrUser );
		for( ; sIter != skillRequires_.end(); sIter++ )
			reqSkill += ptrUser->GetSkill( sIter->first );

		if( reqSkill < 10 )
			reqSkill = 10;

		//����������� ������� ����� ��-�� ����, ��� ���������� ������ ������� 5, � ���������� ���� 
		//�������� � ��������� ������������������ ������� �� ������ �������,
		//���� ����� ������
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

std::string CNrpProjectModule::Save( const std::string& saveFolder )
{
	OpFileSystem::CreateDirectory( saveFolder );

	std::string fileName = saveFolder + GetValue<std::string>( NAME ) + ".devmod";

	DeleteFile( fileName.c_str() );
	INrpProject::Save( fileName );
	SaveRequires_( fileName );

	return saveFolder;
}

void CNrpProjectModule::Load( const std::string& fileName )
{
	assert( OpFileSystem::IsExist( fileName ) );
		
	INrpProject::Load( fileName );
	LoadRequries_( fileName );
}

int CNrpProjectModule::RemoveUser( const std::string& userName )
{
	for( USER_LIST::iterator pIter=_users.begin(); pIter != _users.end(); pIter++ )
		if( (*pIter)->GetValue<std::string>( NAME ) == userName )
		{
			_users.erase( pIter );
			return 1;
		}

	return 0;
}
}//end namespace nrp