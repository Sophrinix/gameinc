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
#include <io.h>

namespace nrp
{

CNrpDevelopGame::~CNrpDevelopGame(void)
{
}

void CNrpDevelopGame::InitializeOptions_( const std::string& name )
{
	INrpDevelopProject::InitializeOptions_();

	CreateValue<int>( USERNUMBER, 0 );
	CreateValue<std::string>( PROJECTSTATUS, "unknown" );
	CreateValue<CNrpCompany*>( PARENTCOMPANY, NULL );
	CreateValue<PNrpGameEngine>( GAME_ENGINE, NULL );
	CreateValue<std::string>( PREV_GAME, "" );
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

	SetValue<std::string>( NAME, name );
}

CNrpDevelopGame::CNrpDevelopGame( CNrpGameProject* nProject, CNrpCompany* ptrCompany ) 
: INrpDevelopProject( CLASS_DEVELOPGAME, nProject->GetValue<std::string>( NAME ) )
{
	InitializeOptions_( nProject->GetValue<std::string>( NAME ) );
    //компания, которая делает это модуль
	SetValue<CNrpCompany*>( PARENTCOMPANY, ptrCompany );
	assert( nProject->GetValue<int>( PLATFORMNUMBER ) != 0 );
	SetValue<int>( PLATFORMNUMBER, nProject->GetValue<int>( PLATFORMNUMBER ) );
	SetValue<int>( GENRE_MODULE_NUMBER, nProject->GetValue<int>( GENRE_MODULE_NUMBER ) );
	int bcv = nProject->GetValue<int>( ENGINE_CODEVOLUME );

	PNrpGameEngine ge = nProject->GetValue<PNrpGameEngine>( GAME_ENGINE );
	SetValue<PNrpGameEngine>( GAME_ENGINE, ge );

	CNrpProjectModule* extEngine = new CNrpProjectModule( PT_ENGINDEEXTEND, this );
	extEngine->SetValue<std::string>( NAME, GetValue<std::string>( NAME ) + " Расширение движка" );
	extEngine->SetEmployerSkillRequire( SKL_CODING, ge->GetValue<int>( SKILL_CODING ) );
	extEngine->SetValue<int>( CODEVOLUME, nProject->GetValue<int>( ENGINE_CODEVOLUME) );
	extEngine->SetValue<float>( BASE_CODE, 1 );
	gameModules_.push_back( extEngine );
	
	CNrpProjectModule* langSupport = new CNrpProjectModule( PT_LANGSUPPORT, this );
	langSupport->SetValue<std::string>( NAME, "Локализация" );
	langSupport->SetEmployerSkillRequire( SKL_CODING, 10 );
	langSupport->SetValue<float>( BASE_CODE, nProject->GetValue<int>( LANGNUMBER ) * 0.05f );
	langSupport->SetValue<int>( CODEVOLUME, (int)(bcv * langSupport->GetValue<float>( BASE_CODE )) );
	gameModules_.push_back( langSupport );

	CNrpProjectModule* cpCode = new CNrpProjectModule( PT_PLATFORMSUPPORT, this );
	cpCode->SetValue<std::string>( NAME, "Кроссплатформенный код" );
	cpCode->SetEmployerSkillRequire( SKL_CODING, ge->GetValue<int>( SKILL_CODING ) );
	cpCode->SetValue<float>( BASE_CODE, nProject->GetValue<int>( PLATFORMNUMBER ) * 0.1f );
	cpCode->SetValue<int>( CODEVOLUME, (int)(bcv * cpCode->GetValue<float>( BASE_CODE )) );
	gameModules_.push_back( cpCode );

	SetValue<std::string>( PREV_GAME, nProject->GetValue<std::string>( PREV_GAME ) );
	SetValue<int>( BASE_CODEVOLUME, nProject->GetValue<int>( BASE_CODEVOLUME ) );
	SetValue<int>( CODEVOLUME, nProject->GetValue<int>( CODEVOLUME ) );
	SetValue<int>( TECHTYPE, nProject->GetValue<int>( TECHTYPE ) );
	SetValue<CNrpScenario*>( SCENARIO, nProject->GetValue<CNrpScenario*>( SCENARIO ) );
	SetValue<CNrpLicense*>( GLICENSE, nProject->GetValue<CNrpLicense*>( GLICENSE ) );

	if( nProject->GetValue<PNrpTechnology>( SCRIPTENGINE ) )
	{
		PNrpProjectModule module = new CNrpProjectModule( nProject->GetValue<PNrpTechnology>( SCRIPTENGINE ), this );
		module->SetValue<int>( CODEVOLUME, static_cast< int >( bcv * module->GetValue<float>( BASE_CODE ) ) );
		gameModules_.push_back( module );
	}

	if( nProject->GetValue<PNrpTechnology>( MINIGAMEENGINE ) )
	{
		PNrpProjectModule module = new CNrpProjectModule( nProject->GetValue<PNrpTechnology>( MINIGAMEENGINE ), this );
		module->SetValue<int>( CODEVOLUME, static_cast< int >( bcv * module->GetValue<float>( BASE_CODE ) ) );
		gameModules_.push_back( module );
	}

	if( nProject->GetValue<PNrpTechnology>( PHYSICSENGINE ) )
	{
		PNrpProjectModule module = new CNrpProjectModule( nProject->GetValue<PNrpTechnology>( PHYSICSENGINE ), this );
		module->SetValue<int>( CODEVOLUME, static_cast<int>( bcv * module->GetValue<float>( BASE_CODE ) ) );
		gameModules_.push_back( module );
	}

	if( nProject->GetValue<PNrpTechnology>( GRAPHICQUALITY ) )
	{
		PNrpProjectModule module = new CNrpProjectModule( nProject->GetValue<PNrpTechnology>( GRAPHICQUALITY ), this );
		module->SetValue<int>( CODEVOLUME, bcv * module->GetValue<float>( BASE_CODE ) );
		gameModules_.push_back( module );
	}

	if( nProject->GetValue<PNrpTechnology>( SOUNDQUALITY ) )
	{
		PNrpProjectModule module = new CNrpProjectModule( nProject->GetValue<PNrpTechnology>( SOUNDQUALITY ), this );
		module->SetValue<int>( CODEVOLUME, bcv * module->GetValue<float>( BASE_CODE ) );
		gameModules_.push_back( module );
	}

	SetValue<int>( BASEQUALITY, nProject->GetValue<int>( BASEQUALITY ) );
	SetValue<bool>( PROJECTREADY, false );
	SetValue<int>( QUALITY, 0 );

	for( TECH_LIST::const_iterator pIter = nProject->GetTechList().begin(); 
		pIter != nProject->GetTechList().end(); 
		pIter++ )
	{
		CNrpProjectModule* nTech = new CNrpProjectModule( *pIter, this );
		nTech->SetValue<int>( CODEVOLUME, (int)(bcv * nTech->GetValue<float>( BASE_CODE )) );
		gameModules_.push_back( nTech );
	}

	for( TECH_LIST::const_iterator pIter = nProject->GetGenreList().begin();
		 pIter != nProject->GetGenreList().end(); 
		 pIter++ )
	{
		if( *pIter == NULL )
			continue;

		CNrpProjectModule* nTech = new CNrpProjectModule( *pIter, this );
		nTech->SetValue<int>( CODEVOLUME, (int)(bcv * nTech->GetValue<float>( BASE_CODE )) );
		gameModules_.push_back( nTech );
	}

	for( TECH_LIST::const_iterator pIter = nProject->GetVideoTechList().begin();
		 pIter != nProject->GetVideoTechList().end(); 
		 pIter++ )
	{
		CNrpProjectModule* nTech = new CNrpProjectModule( *pIter, this );
		nTech->SetValue<int>( CODEVOLUME, (int)(bcv * nTech->GetValue<float>( BASE_CODE )) );
		gameModules_.push_back( nTech );
	}

	for( TECH_LIST::const_iterator pIter = nProject->GetSoundTechList().begin();
		 pIter != nProject->GetSoundTechList().end(); 
		 pIter++ )
	{
		CNrpProjectModule* nTech = new CNrpProjectModule( *pIter, this );
		nTech->SetValue<int>( CODEVOLUME, (int)(bcv * nTech->GetValue<float>( BASE_CODE )) );
		gameModules_.push_back( nTech );
	}

	SetValue<std::string>( PROJECTSTATUS, std::string("develop") );
	SetValue<int>( MODULE_NUMBER, gameModules_.size() );
}

CNrpDevelopGame::CNrpDevelopGame( std::string name, CNrpCompany* ptrCompany )
: INrpDevelopProject( CLASS_DEVELOPGAME, name )
{
	InitializeOptions_( name );
	SetValue<PNrpCompany>( PARENTCOMPANY, ptrCompany );
}

void CNrpDevelopGame::ModuleFinished( CNrpProjectModule* module, IUser* ptrUser )
{
	SetDeveloper( ptrUser );
	float growExp = module->GetValue<int>( CODEVOLUME ) / (float)GetValue<int>( BASE_CODEVOLUME );

	int techType = GetGenre( 0 )->GetValue<PROJECT_TYPE>( TECHTYPE );

	//опыт пользователя растет по мере выполнения компонентов
	//а если у пользователя не было опыта в этом жанре, то он появляется
	ptrUser->IncreaseExperience( techType, (int)growExp );

	if( IsReady() )
	{
		OutputDebugString( ("Закончен проект " + GetValue<std::string>( NAME ) + "\n").c_str() );
	}
}

void CNrpDevelopGame::Save( std::string folderSave )
{
	if( _access( folderSave.c_str(), 0 ) == -1 )
		CreateDirectory( folderSave.c_str(), NULL );

	std::string localFolder = folderSave + GetValue<std::string>( NAME ) + "/";

	if( _access( localFolder.c_str(), 0 ) == -1 )
		CreateDirectory( localFolder.c_str(), NULL );

	std::string fileName = localFolder + "item.devgame";
	INrpDevelopProject::Save( fileName );

	MODULE_LIST::iterator tIter = gameModules_.begin();
	for( int i=0; tIter != gameModules_.end(); tIter++, i++ )
	{
		(*tIter)->Save( localFolder + "modules/" );
		IniFile::Write( "modules", "module" + IntToStr(i), (*tIter)->GetValue<std::string>( NAME ), fileName );
	}

	if( GetValue<PNrpGameEngine>( GAME_ENGINE ) )
	{
		PNrpGameEngine engine = GetValue<PNrpGameEngine>( GAME_ENGINE );
		IniFile::Write( SECTION_PROPERTIES, GAME_ENGINE, engine->GetValue<std::string>( NAME ), fileName );
	}

	if( GetValue<PNrpScenario>( SCENARIO ) )
	{
		IniFile::Write( SECTION_PROPERTIES, SCENARIO, GetValue<PNrpScenario>( SCENARIO )->GetValue<std::string>(NAME), fileName );
		GetValue<PNrpScenario>( SCENARIO )->Save( SECTION_PROPERTIES, localFolder + SCENARIO + ".ini" );
	}

	if( GetValue<PNrpLicense>( GLICENSE ) )
	{
		IniFile::Write( SECTION_PROPERTIES, GLICENSE, GetValue<PNrpLicense>( GLICENSE )->GetValue<std::string>(NAME), fileName );
		GetValue<PNrpLicense>( GLICENSE )->Save( SECTION_PROPERTIES, localFolder + GLICENSE + ".ini");
	}
}

void CNrpDevelopGame::Load( std::string loadFolder )
{
	if( loadFolder[ loadFolder.length() - 1 ] != '/' && 
		loadFolder[ loadFolder.length() - 1 ] != '\\' )
		loadFolder += "/";

	std::string fileName = loadFolder + "item.devgame";
	CNrpCompany* ptrCompany = GetValue<PNrpCompany>( PARENTCOMPANY );
	INrpProject::Load( SECTION_PROPERTIES, fileName );

	for( int i=0; i < GetValue<int>( MODULE_NUMBER ); ++i )
	{
		std::string name = IniFile::Read( "modules", "module" + IntToStr(i), std::string(""), fileName );
		if( !name.empty() )
		{
			CNrpProjectModule* tech = new CNrpProjectModule( PROJECT_TYPE( 0 ), this );
			tech->Load( loadFolder + "modules/" + name + ".devmod" );
			gameModules_.push_back( tech );		
		}
	}

	std::string name = IniFile::Read( SECTION_PROPERTIES, GAME_ENGINE, std::string(""), fileName );
	SetValue<PNrpGameEngine>( GAME_ENGINE, CNrpApplication::Instance().GetGameEngine( name ) );

	name = IniFile::Read( SECTION_PROPERTIES, SCENARIO, std::string(""), fileName );
	PNrpScenario scenario = new CNrpScenario( name );
	scenario->Load( SECTION_PROPERTIES, loadFolder + SCENARIO + ".ini" ); 
	SetValue<PNrpScenario>( SCENARIO, scenario );

	name = IniFile::Read( SECTION_PROPERTIES, GLICENSE, std::string(""), fileName );
	PNrpLicense license = new CNrpLicense( name );
	license->Load( SECTION_PROPERTIES, loadFolder + GLICENSE + ".ini" );
	SetValue<PNrpLicense>( GLICENSE, license );

	for( int i=0; i < GetValue<int>( USERNUMBER ); i++ )
		developers_.push_back( IniFile::Read( "developers", "user_" + IntToStr( i ), std::string(""), fileName ) );
}

bool CNrpDevelopGame::IsReady()
{
	bool ready = true;
	for( MODULE_LIST::iterator pIter = gameModules_.begin(); 
		 pIter != gameModules_.end(); 
		 pIter++)
		if( (*pIter) && (*pIter)->GetValue<float>( READYWORKPERCENT ) < 1 )
		{
			ready = false;		
			break;
		}

	SetValue<bool>( PROJECTREADY, ready );
	if( ready )
		SetValue<std::string>( PROJECTSTATUS, "produce" );

	return ready;
}

void CNrpGameProject::UpdateDevelopmentMoney()
{
	TECH_LIST rt;
	GetAllTech_( rt );

	int money = GetValue<int>( MONEYONDEVELOP );
	CNrpCompany* cmp = GetValue<PNrpCompany>( PARENTCOMPANY );
	TECH_LIST::iterator pIter = rt.begin();
	for( ; pIter != rt.end(); pIter++ )
	{
		std::string name = (*pIter)->GetValue<std::string>( COMPONENTLIDER );
		if( !name.empty() )
		{
			PUser ptrUser = cmp->GetUser( name );
			if( ptrUser )
				money += (int)(ptrUser->GetValue<int>( SALARY ) / 30.f);
		}
	}

	SetValue<int>( MONEYONDEVELOP, money );
}

CNrpProjectModule* CNrpDevelopGame::GetGenre( size_t index )
{
	MODULE_LIST::iterator pIter = gameModules_.begin();
	int i=0;
	for( ; pIter != gameModules_.end(); pIter++ )
	{
		 if( (*pIter)->GetValue<int>( TECHGROUP ) == PT_GENRE )
			 if( i == index )
				 return *pIter;
			 else
				 i++;
	}

	return NULL;
}

CNrpProjectModule* CNrpDevelopGame::GetModule( size_t index )
{
	return index < gameModules_.size() ? gameModules_[ index ] : NULL; 
}
}//end namespace nrp