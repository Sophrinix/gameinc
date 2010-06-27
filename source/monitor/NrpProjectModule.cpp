#include "StdAfx.h"
#include "NrpProjectModule.h"
#include "IUser.h"
#include "INrpDevelopProject.h"
#include <assert.h>
#include <io.h>

namespace nrp
{

CNrpProjectModule::CNrpProjectModule(void) : CNrpTechnology( PROJECT_TYPE(0) )
{
}

CNrpProjectModule::CNrpProjectModule( CNrpTechnology* pTech, INrpProject* pProject )
: CNrpTechnology( pTech->GetValue<PROJECT_TYPE>( TECHGROUP ) )
{
	InitializeOptions_();

	SetValue<std::string>( NAME, pTech->GetValue<std::string>( NAME ) );
	SetValue<int>( TECHGROUP, pTech->GetValue<int>( TECHGROUP ) );
	SetValue<int>( TECHTYPE, pTech->GetValue<int>( TECHTYPE ) );
	SetValue<float>( BASE_CODE, pTech->GetValue<float>( BASE_CODE ) );
	SetValue<float>( ENGINE_CODE, pTech->GetValue<float>( ENGINE_CODE ) );
	SetValue<int>( LEVEL, pTech->GetValue<int>( LEVEL ) );
	SetValue<INrpProject*>( PARENT, pProject );
	SetValue<int>( QUALITY, pTech->GetValue<int>( QUALITY ) );

	techRequires_ = pTech->GetTechRequires();
	skillRequires_ = pTech->GetSkillRequires();
}

CNrpProjectModule::CNrpProjectModule( PROJECT_TYPE type, INrpProject* pProject )
: CNrpTechnology( type )
{
	InitializeOptions_();
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
	CreateValue<float>( READYWORKPERCENT, 0 );
	CreateValue<int>( ERRORNUMBER, 0 );
	CreateValue<INrpProject*>( PARENT, NULL );
}

void CNrpProjectModule::SetLider( IUser* ptrUser )
{
	IUser* currentLider = GetValue<IUser*>( COMPONENTLIDER );
	IUser* lastLider = GetValue<IUser*>( LASTWORKER );

	//есть текущий исполнитель
	SetValue<IUser*>( LASTWORKER, currentLider );
	SetValue<IUser*>( COMPONENTLIDER, ptrUser  );

	if( lastLider && ptrUser && (lastLider != ptrUser) )
		SetValue<int>( CODEPASSED,(int)(GetValue<int>( CODEPASSED ) * 0.75f) );
}

void CNrpProjectModule::Update( IUser* ptrUser )
{
	if( GetValue<int>( CODEPASSED ) < GetValue<int>( CODEVOLUME) )
	{
		int reqSkill = 0;
		REQUIRE_MAP::iterator sIter = skillRequires_.begin();
		for( ; sIter != skillRequires_.end(); sIter++ )
			reqSkill += ptrUser->GetSkill( sIter->first );

		float genreSkill = ptrUser->GetGenreExperience( GetValue<int>( TECHTYPE ) ) / 100.f;

		if( genreSkill < 0.1f )
			genreSkill = 0.1f;
		float genrePref = ptrUser->GetGenrePreferences( GetValue<int>( TECHTYPE ) ) / 100.f;
		if( genrePref < 0.1f )
			genrePref = 0.1f;

		int codePassed = GetValue<int>( CODEPASSED ) + (int)(reqSkill * (genrePref + genreSkill));
		if( codePassed >= GetValue<int>( CODEVOLUME ) )
			codePassed = GetValue<int>( CODEVOLUME);

		SetValue<int>( CODEPASSED, codePassed );
		SetValue<float>( READYWORKPERCENT, codePassed / (float)GetValue<int>( CODEVOLUME ) );
		int quality = GetValue<int>( QUALITY );
		SetValue<int>( QUALITY, (quality + ptrUser->GetValueA<int>( CODE_QUALITY )) / 2 );
	}

	if( GetValue<float>( READYWORKPERCENT ) >= 1 )
	{
		INrpDevelopProject* parent = GetValue<INrpDevelopProject*>( PARENT );
		parent->ModuleFinished( this, ptrUser );
	}
}

void CNrpProjectModule::Save( std::string saveFolder )
{
	if( _access( saveFolder.c_str(), 0 ) == -1 )
		CreateDirectory( saveFolder.c_str(), NULL );

	std::string fileName = saveFolder + GetValue<std::string>( NAME ) + ".devmod";

	DeleteFile( fileName.c_str() );
	INrpProject::Save( PROPERTIES, fileName );
	SaveRequires_( fileName );
}

void CNrpProjectModule::Load( std::string fileName )
{

}
}//end namespace nrp