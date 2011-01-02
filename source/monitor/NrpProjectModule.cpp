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

CNrpProjectModule::CNrpProjectModule( CNrpTechnology* pTech, INrpDevelopProject* pProject )
	: IWorkingModule( PROJECT_TYPE( (int)(*pTech)[ TECHGROUP ] ), CLASS_PROJECTMODULE )
{
	InitializeOptions_();

	CNrpTechnology& refTech = *pTech;
	_self[ NAME ] = refTech[ NAME ];
	_self[ TECHGROUP ] = refTech[ TECHGROUP ];
	_self[ BASE_CODE ] = refTech[ BASE_CODE ];
	_self[ ENGINE_CODE ] = refTech[ ENGINE_CODE ];
	_self[ INTERNAL_NAME ] = refTech[ INTERNAL_NAME ];
	_self[ TEXTURENORMAL ] = refTech[ TEXTURENORMAL ];
	_self[ LEVEL ] = refTech[ LEVEL ];
	_self[ PARENT ] = pProject;
	_self[ QUALITY ] = refTech[ QUALITY ];

	CopyMapTo( _techRequires, pTech->GetTechRequires() );
	CopyMapTo( _skillRequires, pTech->GetSkillRequires() );
}

CNrpProjectModule::CNrpProjectModule( PROJECT_TYPE type, INrpDevelopProject* pProject )
	: IWorkingModule( type, CLASS_PROJECTMODULE )
{
	InitializeOptions_();
	_self[ TECHGROUP ] = static_cast< int >( type );
	_self[ PARENT ] = pProject;
}

CNrpProjectModule::CNrpProjectModule() : IWorkingModule( PROJECT_TYPE(0), CLASS_PROJECTMODULE )
{}

CNrpProjectModule::CNrpProjectModule( CNrpPlatform* platform, INrpDevelopProject* project )
: IWorkingModule( PROJECT_TYPE( (int)(*platform)[ TECHGROUP ] ), CLASS_PROJECTMODULE )
{
	InitializeOptions_();

	CNrpPlatform& refPl = *platform;
	_self[ NAME ] = NrpText::LuaString( "#PLATFORM_PORTING" ) + (NrpText)refPl[ NAME ];
	_self[ TECHGROUP ] = refPl[ TECHGROUP ];
	_self[ BASE_CODE ] = refPl[ BASE_CODE ];
	_self[ ENGINE_CODE ] = refPl[ ENGINE_CODE ];
	_self[ INTERNAL_NAME ] = refPl[ INTERNAL_NAME ];
	_self[ TEXTURENORMAL ] = refPl[ TEXTURENORMAL ];
	_self[ LEVEL ] = refPl[ LEVEL ];
	_self[ PARENT ] = project;
	_self[ QUALITY ] = refPl[ QUALITY ];

	const TECHS& techs = platform->GetTechsList();
	for( s32 i=0; i < techs.size(); i++ )
	{
		const NrpText& name = (*techs[ i ])[ INTERNAL_NAME ];
		 _techRequires[ name ] = 100;
	}
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
	Add<int>( MONEYONDEVELOP, 0 );
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

	if( (int)_self[ CODEPASSED ] < (int)_self[ CODEVOLUME] )
	{
		int reqSkill = 0;
		KNOWLEDGE_MAP::Iterator sIter = _skillRequires.getIterator();
		float teamKoeff = _GetWorkKoeffForUser( ptrUser );
		for( ; !sIter.atEnd(); sIter++ )
			reqSkill += ptrUser->GetSkill( sIter->getKey() );

		if( reqSkill < 10 )
			reqSkill = 10;

		//коэффициент команды нужен из-за того, что отимальный размер команды 5, а дальнейший рост 
		//приводит к ухудшению производительности каждого из членов команды,
		//хотя общая растет
		float genreSkill = teamKoeff * ptrUser->GetGenreExperience( _self[ INTERNAL_NAME ] ) / 100.f;

		if( genreSkill < 0.1f )
			genreSkill = 0.1f;
		float genrePref = teamKoeff * ptrUser->GetGenrePreferences( _self[ INTERNAL_NAME ] ) / 100.f;
		
		if( genrePref < 0.1f )
			genrePref = 0.1f;


		int codePassed = (int)_self[ CODEPASSED ] + static_cast< int >(reqSkill * (genrePref + genreSkill));
		if( (int)_self[ CODEVOLUME ] <= codePassed )
			codePassed = _self[ CODEVOLUME ];

		_self[ CODEPASSED ] = codePassed;
		_self[ READYWORKPERCENT ] = codePassed / static_cast< float >( (int)_self[ CODEVOLUME ] );
		int quality = Param( QUALITY );
		_self[ QUALITY ] = static_cast< int >( quality + (int)(*ptrUser)[ CODE_QUALITY ] / 2 );
		_self[ MONEYONDEVELOP ] += (int)(*ptrUser)[ SALARY ] / (20*9);
	}

	if( _self[ READYWORKPERCENT ] >= 1.f )
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

	NrpText fileName = saveFolder + Text( INTERNAL_NAME ) + ".devmod";
	assert( !OpFileSystem::IsExist( fileName ) );

	INrpProject::Save( fileName );
	IniFile sv( fileName );

	sv.Set( SECTION_REQUIRE_TECH, _techRequires );
	sv.Set( SECTION_REQUIRE_SKILL, _skillRequires );

	return fileName;
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