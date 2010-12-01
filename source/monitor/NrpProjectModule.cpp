#include "StdAfx.h"
#include "NrpProjectModule.h"
#include "IUser.h"
#include "INrpDevelopProject.h"
#include "OpFileSystem.h"
#include "NrpPlatform.h"
#include "IniFile.h"

#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_PROJECTMODULE( "CNrpProjectModule" );

CNrpProjectModule::CNrpProjectModule( CNrpTechnology* pTech, INrpProject* pProject )
	: IWorkingModule( (*pTech)[ TECHGROUP ].As<PROJECT_TYPE>(), CLASS_PROJECTMODULE )
{
	InitializeOptions_();

	CNrpTechnology& refTech = *pTech;
	Param( NAME ) = refTech[ NAME ];
	Param( TECHGROUP ) = refTech[ TECHGROUP ];
	Param( TECHTYPE ) = refTech[ TECHTYPE ];
	Param( BASE_CODE ) = refTech[ BASE_CODE ];
	Param( ENGINE_CODE ) = refTech[ ENGINE_CODE ];
	Param( INTERNAL_NAME ) = refTech[ INTERNAL_NAME ];
	Param( TEXTURENORMAL ) = refTech[ TEXTURENORMAL ];
	Param( LEVEL ) = refTech[ LEVEL ];
	Param( PARENT ) = pProject;
	Param( QUALITY ) = refTech[ QUALITY ];

	CopyMapTo( _techRequires, pTech->GetTechRequires() );
	CopyMapTo( _skillRequires, pTech->GetSkillRequires() );
}

CNrpProjectModule::CNrpProjectModule( PROJECT_TYPE type, INrpDevelopProject* pProject )
	: IWorkingModule( type, CLASS_PROJECTMODULE )
{
	InitializeOptions_();
	Param( TECHTYPE ) = type;
	Param( PARENT ) = pProject;
}

CNrpProjectModule::CNrpProjectModule() : IWorkingModule( PROJECT_TYPE(0), CLASS_PROJECTMODULE )
{}

CNrpProjectModule::CNrpProjectModule( CNrpPlatform* platform, INrpProject* project )
: IWorkingModule( (*platform)[ TECHGROUP ].As<PROJECT_TYPE>(), CLASS_PROJECTMODULE )
{
	InitializeOptions_();

	CNrpPlatform& refPl = *platform;
	Param( NAME ) = NrpText::LuaString( "#PLATFORM_PORTING" ) + (NrpText)refPl[ NAME ];
	Param( TECHGROUP ) = refPl[ TECHGROUP ];
	Param( TECHTYPE ) = refPl[ TECHTYPE ];
	Param( BASE_CODE ) = refPl[ BASE_CODE ];
	Param( ENGINE_CODE ) = refPl[ ENGINE_CODE ];
	Param( INTERNAL_NAME ) = refPl[ INTERNAL_NAME ];
	Param( TEXTURENORMAL ) = refPl[ TEXTURENORMAL ];
	Param( LEVEL ) = refPl[ LEVEL ];
	Param( PARENT ) = project;
	Param( QUALITY ) = refPl[ QUALITY ];

	const TECHS& techs = platform->GetTechsList();
	for( s32 i=0; i < techs.size(); i++ )
		 _techRequires[ (int)(*techs[ i ])[  TECHTYPE ] ] = 100;
}

CNrpProjectModule::~CNrpProjectModule(void)
{
}

void CNrpProjectModule::InitializeOptions_()
{
	CNrpTechnology::_InitializeOptions();

	Add<IUser*>( LASTWORKER, NULL );
	Add<IUser*>( COMPONENTLIDER, NULL );
	Add<int>( CODEVOLUME, 0 );
	Add<int>( CODEPASSED, 0 );
	Add<int>( ERRORNUMBER, 0 );
	Add<int>( USERNUMBER, 0 );
	Remove( PARENT );
	Add<INrpDevelopProject*>( PARENT, NULL );
}

int CNrpProjectModule::AddUser( IUser* ptrUser )
{
	if( (float)Param( READYWORKPERCENT ) < 1.f )
	{
		_users.push_back( ptrUser );
		Param( USERNUMBER ) = static_cast< int >( _users.size() );
	}

	return ( (float)Param( READYWORKPERCENT ) < 1.f);
}

void CNrpProjectModule::Update( IUser* ptrUser )
{
	INrpDevelopProject* parent = Param( PARENT ).As<INrpDevelopProject*>();
	assert( parent != NULL );

	if( Param( CODEPASSED ) < Param( CODEVOLUME) )
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
		float genreSkill = teamKoeff * ptrUser->GetGenreExperience( Param( TECHTYPE ).As<GENRE_TYPE>() ) / 100.f;

		if( genreSkill < 0.1f )
			genreSkill = 0.1f;
		float genrePref = teamKoeff * ptrUser->GetGenrePreferences( Param( TECHTYPE ).As<GENRE_TYPE>() ) / 100.f;
		
		if( genrePref < 0.1f )
			genrePref = 0.1f;


		int codePassed = (int)Param( CODEPASSED ) + static_cast< int >(reqSkill * (genrePref + genreSkill));
		if( (int)Param( CODEVOLUME ) <= codePassed )
			codePassed = Param( CODEVOLUME);

		Param( CODEPASSED ) = codePassed;
		Param( READYWORKPERCENT ) = codePassed / static_cast< float >( (int)Param( CODEVOLUME ) );
		int quality = Param( QUALITY );
		Param( QUALITY ) = static_cast< int >( quality + (int)(*ptrUser)[ CODE_QUALITY ] / 2 );
		Param( MONEYONDEVELOP ) += (int)(*ptrUser)[ SALARY ] / (20*9);
	}

	if( Param( READYWORKPERCENT ) >= 1.f )
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

	NrpText fileName = saveFolder + Text( NAME ) + ".devmod";
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