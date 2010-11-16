#include "StdAfx.h"
#include "NrpDevelopGame.h"
#include "NrpProjectModule.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "IUser.h"
#include "NrpScenario.h"
#include "NrpLicense.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "OpFileSystem.h"
#include "IniFile.h"

namespace nrp
{
CLASS_NAME CLASS_DEVELOPGAME("CNrpDevelopGame");

CNrpDevelopGame::~CNrpDevelopGame(void)
{
}

NrpText CNrpDevelopGame::ClassName()
{ 
	return CLASS_DEVELOPGAME; 
}

CNrpDevelopGame::CNrpDevelopGame() : INrpDevelopProject( CLASS_DEVELOPGAME, "" ) {}; 

void CNrpDevelopGame::_InitializeOptions( const NrpText& name )
{
	INrpDevelopProject::InitializeOptions_();

	CreateValue<int>( USERNUMBER, 0 );
	CreateValue<NrpText>( PROJECTSTATUS, "unknown" );
	CreateValue<CNrpCompany*>( PARENTCOMPANY, NULL );
	CreateValue<PNrpGameEngine>( GAME_ENGINE, NULL );
	CreateValue<NrpText>( PREV_GAME, "" );
	CreateValue<int>( BASE_CODEVOLUME, 0 );
	CreateValue<int>( BASEQUALITY, 0 );
	CreateValue<int>( QUALITY, 0 );
	CreateValue<int>( CODEVOLUME, 0 );
	CreateValue<CNrpScenario*>( SCENARIO, NULL );
	CreateValue<CNrpLicense*>( GLICENSE, NULL );
	CreateValue<int>( MONEYONDEVELOP, 0 );
	CreateValue<int>( PLATFORMNUMBER, 0 );
	CreateValue<int>( GENRE_MODULE_NUMBER, 0 );
	CreateValue<int>( LOCALIZATION, 0 );
	CreateValue<float>( FAMOUS, 0 );

	SetValue<NrpText>( NAME, name );
}

void CNrpDevelopGame::_AddModulesFrom( const CNrpTechnology* tech, int baseCode )
{
	if( tech )
	{
		CNrpProjectModule* module = new CNrpProjectModule( const_cast< CNrpTechnology* >( tech ), this );
		module->SetValue<int>( CODEVOLUME, static_cast< int >( baseCode * module->GetValue<float>( BASE_CODE ) ) );
		_modules.push_back( module );
	}
}

void CNrpDevelopGame::_AddModulesFrom( const TECHS& arrtech, int baseCode )
{
	for( u32 i=0; i < arrtech.size(); i++ )
	{
		CNrpTechnology* tech = arrtech[ i ];

		assert( tech );
		if( tech )
		{
			CNrpProjectModule* nTech = new CNrpProjectModule( tech, this );
			nTech->SetValue<int>( CODEVOLUME, static_cast< int >( baseCode * nTech->GetValue<float>( BASE_CODE )) );
			_modules.push_back( nTech );
		}
	}
}

CNrpDevelopGame::CNrpDevelopGame( CNrpGameProject* nProject, CNrpCompany* ptrCompany ) 
: INrpDevelopProject( CLASS_DEVELOPGAME, nProject->GetString( NAME ) )
{
	_InitializeOptions( nProject->GetString( NAME ) );
    //компания, которая делает это модуль
	SetValue<CNrpCompany*>( PARENTCOMPANY, ptrCompany );
	assert( nProject->GetValue<int>( PLATFORMNUMBER ) != 0 );
	SetValue<int>( PLATFORMNUMBER, nProject->GetValue<int>( PLATFORMNUMBER ) );
	SetValue<int>( GENRE_MODULE_NUMBER, nProject->GetValue<int>( GENRE_MODULE_NUMBER ) );
	int bcv = nProject->GetValue<int>( ENGINE_CODEVOLUME );

	PNrpGameEngine ge = nProject->GetValue<PNrpGameEngine>( GAME_ENGINE );
	SetValue<PNrpGameEngine>( GAME_ENGINE, ge );

	CNrpProjectModule* extEngine = new CNrpProjectModule( PT_ENGINDEEXTEND, this );
	extEngine->SetString( NAME, GetString( NAME ) + " Расширение движка" );
	extEngine->SetEmployerSkillRequire( SKL_CODING, ge->GetValue<int>( SKILL_CODING ) );
	extEngine->SetValue<int>( CODEVOLUME, nProject->GetValue<int>( ENGINE_CODEVOLUME) );
	extEngine->SetValue<float>( BASE_CODE, 1 );
	_modules.push_back( extEngine );
	
	CNrpProjectModule* langSupport = new CNrpProjectModule( PT_LANGSUPPORT, this );
	langSupport->SetString( NAME, "Локализация" );
	langSupport->SetEmployerSkillRequire( SKL_CODING, 10 );
	langSupport->SetValue<float>( BASE_CODE, nProject->GetValue<int>( LANGNUMBER ) * 0.05f );
	langSupport->SetValue<int>( CODEVOLUME, (int)(bcv * langSupport->GetValue<float>( BASE_CODE )) );
	_modules.push_back( langSupport );

	CNrpProjectModule* cpCode = new CNrpProjectModule( PT_PLATFORMSUPPORT, this );
	cpCode->SetString( NAME, "Кроссплатформенный код" );
	cpCode->SetEmployerSkillRequire( SKL_CODING, ge->GetValue<int>( SKILL_CODING ) );
	cpCode->SetValue<float>( BASE_CODE, nProject->GetValue<int>( PLATFORMNUMBER ) * 0.1f );
	cpCode->SetValue<int>( CODEVOLUME, (int)(bcv * cpCode->GetValue<float>( BASE_CODE )) );
	_modules.push_back( cpCode );

	SetString( PREV_GAME, nProject->GetString( PREV_GAME ) );
	SetValue<int>( BASE_CODEVOLUME, nProject->GetValue<int>( BASE_CODEVOLUME ) );
	SetValue<int>( CODEVOLUME, nProject->GetValue<int>( CODEVOLUME ) );
	SetValue<int>( TECHTYPE, nProject->GetValue<int>( TECHTYPE ) );
	SetValue<CNrpScenario*>( SCENARIO, nProject->GetValue<CNrpScenario*>( SCENARIO ) );
	SetValue<CNrpLicense*>( GLICENSE, nProject->GetValue<CNrpLicense*>( GLICENSE ) );
	SetValue<int>( BASEQUALITY, nProject->GetValue<int>( BASEQUALITY ) );
	SetValue<bool>( PROJECTREADY, false );
	SetValue<int>( QUALITY, 0 );

	_AddModulesFrom( nProject->GetValue<PNrpTechnology>( SCRIPTENGINE ), bcv );
	_AddModulesFrom( nProject->GetValue<PNrpTechnology>( MINIGAMEENGINE ), bcv );
	_AddModulesFrom( nProject->GetValue<PNrpTechnology>( PHYSICSENGINE ), bcv );
	_AddModulesFrom( nProject->GetValue<PNrpTechnology>( GRAPHICQUALITY ), bcv);
	_AddModulesFrom( nProject->GetValue<PNrpTechnology>( SOUNDQUALITY ), bcv );
	_AddModulesFrom( nProject->GetTechList(), bcv );
	_AddModulesFrom( nProject->GetGenreList(), bcv );
	_AddModulesFrom( nProject->GetVideoTechList(), bcv );
	_AddModulesFrom( nProject->GetSoundTechList(), bcv );

	SetString( PROJECTSTATUS, "develop" );
	SetValue<int>( MODULE_NUMBER, _modules.size() );
}

CNrpDevelopGame::CNrpDevelopGame( const NrpText& name, CNrpCompany* ptrCompany )
: INrpDevelopProject( CLASS_DEVELOPGAME, name )
{
	_InitializeOptions( name );
	SetValue<PNrpCompany>( PARENTCOMPANY, ptrCompany );
}

CNrpDevelopGame::CNrpDevelopGame( const NrpText& fileName ) 
: INrpDevelopProject( CLASS_DEVELOPGAME, fileName )
{
	_InitializeOptions( fileName );

	Load( fileName );
}

void CNrpDevelopGame::ModuleFinished( CNrpProjectModule* module )
{
	const USERS& uList = module->GetUsers();
	
	for( size_t k=0; k < uList.size(); k++ )
	{
		 SetDeveloper( uList[ k ] );
		 float growExp = module->GetValue<int>( CODEVOLUME ) / (float)GetValue<int>( BASE_CODEVOLUME );
		 int techType = GetGenre( 0 )->GetValue<PROJECT_TYPE>( TECHTYPE );
		 //опыт пользователя растет по мере выполнения компонентов
		 //а если у пользователя не было опыта в этом жанре, то он появляется
		 uList[ k ]->IncreaseExperience( techType, (int)growExp );
	}

	CNrpApplication::Instance().DoLuaFunctionsByType( APP_MODULE_FINISHED, module );
}

NrpText CNrpDevelopGame::Save( const NrpText& folderSave )
{
	assert( OpFileSystem::IsExist( folderSave ) );

	NrpText localFolder = OpFileSystem::CheckEndSlash( folderSave + GetString( NAME ) );

	OpFileSystem::CreateDirectory( localFolder );

	NrpText fileName = localFolder + "item.devgame";
	INrpDevelopProject::Save( fileName );
	IniFile sf( fileName );

	for( u32 i=0; i < _modules.size(); i++ )
	{
		_modules[ i ]->Save( localFolder );
		sf.Set( SECTION_MODULES, CreateKeyModule(i), _modules[ i ]->GetString( NAME ) );
	}

	if( GetValue<PNrpGameEngine>( GAME_ENGINE ) )
	{
		PNrpGameEngine engine = GetValue<PNrpGameEngine>( GAME_ENGINE );
		sf.Set( SECTION_PROPERTIES, GAME_ENGINE, engine->GetString( NAME ) );
	}

	if( PNrpScenario sxn = GetValue<PNrpScenario>( SCENARIO ) )
	{
		sf.Set( SECTION_PROPERTIES, SCENARIO, sxn->GetString(NAME) );
		sxn->Save( localFolder + SCENARIO + ".ini" );
	}

	if( PNrpLicense sln = GetValue<PNrpLicense>( GLICENSE ) )
	{
		sf.Set( SECTION_PROPERTIES, GLICENSE, sln->GetString(NAME) );
		sln->Save( localFolder + GLICENSE + ".ini");
	}

	return folderSave;
}

void CNrpDevelopGame::Load( const NrpText& loadFolder )
{
	assert( OpFileSystem::IsExist(loadFolder) );
	NrpText myFolder = OpFileSystem::CheckEndSlash(loadFolder);

	NrpText fileName = myFolder + "item.devgame";
	CNrpCompany* ptrCompany = GetValue<PNrpCompany>( PARENTCOMPANY );
	INrpProject::Load( fileName );
	IniFile rf( fileName );

	for( int i=0; i < GetValue<int>( MODULE_NUMBER ); ++i )
	{
		NrpText name = rf.Get( SECTION_MODULES, CreateKeyModule(i), NrpText("") );
		if( name.size() )
		{
			CNrpProjectModule* tech = new CNrpProjectModule( PROJECT_TYPE( 0 ), this );
			tech->Load( myFolder + name + ".devmod" );
			_modules.push_back( tech );		
		}
	}

	NrpText name = rf.Get( SECTION_PROPERTIES, GAME_ENGINE, NrpText("") );
	SetValue<PNrpGameEngine>( GAME_ENGINE, CNrpApplication::Instance().GetGameEngine( name ) );

	name = rf.Get( SECTION_PROPERTIES, SCENARIO, NrpText("") );
	PNrpScenario scenario = new CNrpScenario( name );
	scenario->Load( myFolder + SCENARIO + ".ini" ); 
	SetValue<PNrpScenario>( SCENARIO, scenario );

	name = rf.Get( SECTION_PROPERTIES, GLICENSE, NrpText("") );
	PNrpLicense license = new CNrpLicense( name );
	license->Load( myFolder + GLICENSE + ".ini" );
	SetValue<PNrpLicense>( GLICENSE, license );
}

bool CNrpDevelopGame::IsReady()
{
	bool ready = true;
	float workP = 0;
	for( u32 i=0; i < _modules.size(); i++)
	{
		assert( _modules[ i ] );
		if( _modules[ i ] )
		{
			float prc = _modules[ i ]->GetValue<float>( READYWORKPERCENT );
			if( prc < 1 )
				ready = false;		
	
			workP += prc;
			workP /= 2.f;
		}
	}

	SetValue<bool>( PROJECTREADY, ready );
	SetValue<float>( READYWORKPERCENT, workP );
	if( ready )
		SetString( PROJECTSTATUS, "produce" );

	return ready;
}

CNrpProjectModule* CNrpDevelopGame::GetGenre( size_t index )
{
	int position = 0;
	for( u32 i=0; i < _modules.size(); i++ )
	{
		 if( _modules[ i ]->GetValue<int>( TECHGROUP ) == PT_GENRE && position == index )
			 return _modules[ i ];
		 else
			 position++;
	}

	return NULL;
}

CNrpProjectModule* CNrpDevelopGame::GetModule( u32 index )
{
	return index < _modules.size() ? _modules[ index ] : NULL; 
}

CNrpProjectModule* CNrpDevelopGame::GetModule( const NrpText& name )
{
	for( u32 i=0; i < _modules.size(); i++ )
	{
		if( _modules[ i ]->Equale( name ) )
			return _modules[ i ];
	}

	return NULL;
}
}//end namespace nrp