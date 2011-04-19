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

CNrpProjectModule::CNrpProjectModule( CNrpTechnology& refTech, INrpDevelopProject& pProject )
	: IWorkingModule( PROJECT_TYPE( (int)refTech[ TECHGROUP ] ), CLASS_PROJECTMODULE )
{
	InitializeOptions_();

	_self[ NAME ] = refTech[ NAME ];
	_self[ TECHGROUP ] = refTech[ TECHGROUP ];
	_self[ BASE_CODE ] = refTech[ BASE_CODE ];
	_self[ ENGINE_CODE ] = refTech[ ENGINE_CODE ];
	_self[ INTERNAL_NAME ] = refTech[ INTERNAL_NAME ];
	_self[ TEXTURENORMAL ] = refTech[ TEXTURENORMAL ];
	_self[ LEVEL ] = refTech[ LEVEL ];
	_self[ PARENT ] = &pProject;
	_self[ QUALITY ] = refTech[ QUALITY ];

	CopyMapTo( _techRequires, refTech.GetTechRequires() );
	CopyMapTo( _skillRequires, refTech.GetSkillRequires() );
}

CNrpProjectModule::CNrpProjectModule( PROJECT_TYPE type, INrpDevelopProject& pProject )
	: IWorkingModule( type, CLASS_PROJECTMODULE )
{
	InitializeOptions_();

	_self[ TECHGROUP ] = static_cast< int >( type );
	_self[ PARENT ] = &pProject;
}

CNrpProjectModule::CNrpProjectModule() : IWorkingModule( PROJECT_TYPE(0), CLASS_PROJECTMODULE )
{}

CNrpProjectModule::CNrpProjectModule( CNrpPlatform& platform, INrpDevelopProject& project )
: IWorkingModule( PROJECT_TYPE( (int)platform[ TECHGROUP ] ), CLASS_PROJECTMODULE )
{
	InitializeOptions_();

	_self[ NAME ] = NrpText::LuaString( "#PLATFORM_PORTING" ) + (NrpText)platform[ NAME ];
	_self[ TECHGROUP ] = platform[ TECHGROUP ];
	_self[ BASE_CODE ] = platform[ BASE_CODE ];
	_self[ ENGINE_CODE ] = platform[ ENGINE_CODE ];
	_self[ INTERNAL_NAME ] = platform[ INTERNAL_NAME ];
	_self[ TEXTURENORMAL ] = platform[ TEXTURENORMAL ];
	_self[ LEVEL ] = platform[ LEVEL ];
	_self[ PARENT ] = &project;
	_self[ QUALITY ] = platform[ QUALITY ];

	const TECHS& techs = platform.GetTechsList();
	for( u32 i=0; i < techs.size(); i++ )
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
	Add<CNrpUser*>( LASTWORKER, NULL );
	Add<CNrpUser*>( COMPONENTLIDER, NULL );
	Add<int>( CODEVOLUME, 0 );
	Add<int>( CODEPASSED, 0 );
	Add<int>( ERRORNUMBER, 0 );
	Add<int>( USERNUMBER, 0 );
	Add<int>( MONEYONDEVELOP, 0 );
	Remove( PARENT );
	Add<INrpDevelopProject*>( PARENT, NULL );
}

bool CNrpProjectModule::IsMyUser( CNrpUser& user )
{
	for( u32 i=0; i < _users.size(); i++ )
		if( *(_users[ i ]) == user )
			return true;

	return false;
}

int CNrpProjectModule::AddUser( CNrpUser& refUser )
{
	if( IsMyUser( refUser ) )
		return -1;

	if( (float)_self[ READYWORKPERCENT ] < 1.f )
	{
		_users.push_back( &refUser );
		_self[ USERNUMBER ] = static_cast< int >( _users.size() );
	}

	return ( (float)_self[ READYWORKPERCENT ] < 1.f);
}

void CNrpProjectModule::Update( CNrpUser& refUser, const NrpTime& time )
{
	INrpDevelopProject* parent = _self[ PARENT ].As<INrpDevelopProject*>();
	assert( parent != NULL );

	//пока не будет завершен весь модуль, над ним работают люди
	if( (int)_self[ CODEPASSED ] < (int)_self[ CODEVOLUME] )
	{
		int reqSkill = 0;
		KNOWLEDGE_MAP::Iterator sIter = _skillRequires.getIterator();
		float teamKoeff = _GetWorkKoeffForUser( refUser );
		for( ; !sIter.atEnd(); sIter++ )
			reqSkill += refUser.GetSkill( sIter->getKey() );

		reqSkill = (std::max)( reqSkill, 10 );

		//коэффициент команды нужен из-за того, что отимальный размер команды 5, а дальнейший рост 
		//приводит к ухудшению производительности каждого из членов команды,
		//хотя общая растет
		float genreSkill = (std::max)( 0.1f, teamKoeff * refUser.GetGenreExperience( _self[ INTERNAL_NAME ] ) / 100.f );
		float genrePref = (std::max)( 0.1f, teamKoeff * refUser.GetGenrePreferences( _self[ INTERNAL_NAME ] ) / 100.f );
		
		int codePassed = static_cast< int >( (int)_self[ CODEPASSED ] + reqSkill * (genrePref + genreSkill) );
		if( (int)_self[ CODEVOLUME ] <= codePassed )
			codePassed = _self[ CODEVOLUME ];

		_self[ CODEPASSED ] = codePassed;
		_self[ READYWORKPERCENT ] = codePassed / static_cast< float >( (int)_self[ CODEVOLUME ] );
		int quality = _self[ QUALITY ];

		//подсчитаем качество модуля и добавим затраты на рабработку
		_self[ QUALITY ] = static_cast< int >( quality + (int)refUser[ WORK_QUALITY ] / 2 );
		_self[ MONEYONDEVELOP ] += (int)refUser[ SALARY ] / (20*9); //работа двадцать дней по 9 часов

		//подсчитаем количество ошибок принесенных разрабом за этот этап разработки
		//это зависит от объема кода поделенного на опыт разработчика и его настроение 
		//в среднем на каждые 100 строк кода приходится 1 ошибка 
		//также на ошибки влияет настроение человека
		float koeffError = 1.f / (int)refUser[ MOOD ] * 0.01f;
		_self[ ERRORNUMBER ] += static_cast< int >( codePassed / ((int)refUser[ EXPERIENCE ] * koeffError ) );
	}

	if( _self[ READYWORKPERCENT ] >= 1.f )
		parent->ModuleFinished( this );
}

float CNrpProjectModule::_GetWorkKoeffForUser( CNrpUser& ptrUser )
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