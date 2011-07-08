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
#include "NrpGameMarket.h"

namespace nrp
{
CLASS_NAME CLASS_DEVELOPGAME("CNrpDevelopGame");

const NrpText CNrpDevelopGame::saveTemplate = L"item.devgame";
const NrpText CNrpDevelopGame::extNameTemplate = L"game_";

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

	RegProperty<NrpText>( CLASSOBJECT, CLASS_DEVELOPGAME );
	RegProperty( USERNUMBER, 0 );
	RegProperty<NrpText>( PROJECTSTATUS, "unknown" );
	RegProperty<CNrpCompany*>( PARENTCOMPANY, NULL );
	RegProperty<PNrpGameEngine>( GAME_ENGINE, NULL );
	RegProperty<NrpText>( GAME_ENGINE_NAME, "" );
	RegProperty<NrpText>( PREV_GAME, "" );
	RegProperty( BASE_CODEVOLUME, 0 );
	RegProperty( BASEQUALITY, 0 );
	RegProperty( QUALITY, 0 );
	RegProperty( CODEVOLUME, 0 );
	RegProperty<PNrpScenario>( SCENARIO, NULL );
	RegProperty<PNrpLicense>( GLICENSE, NULL );
	RegProperty( MONEYONDEVELOP, 0 );
	RegProperty( PLATFORMNUMBER, 0 );
	RegProperty( GENRE_MODULE_NUMBER, 0 );
	RegProperty<float>( FAMOUS, 0 );
	RegProperty<NrpText>( LICENSE_NAME, "" );
	RegProperty<NrpText>( SCENARIO_NAME, "" );

	_self[ NAME ] = name;
	_self[ TECHGROUP ] = static_cast< int >( PT_GAME );
}

void CNrpDevelopGame::_AddModulesFrom( const CNrpTechnology& tech, int baseCode )
{
	CNrpProjectModule* module = new CNrpProjectModule( const_cast< CNrpTechnology& >( tech ), *this );
	(*module)[ CODEVOLUME ] = static_cast< int >( baseCode * (*module)[ BASE_CODE ].As<float>() );
	_modules.push_back( module );
}

template< typename T, typename TARRAY >
void CNrpDevelopGame::_AddModulesFrom( const TARRAY& arrtech, int baseCode )
{
	for( u32 i=0; i < arrtech.size(); i++ )
	{
		T* ptr = arrtech[ i ];

		assert( ptr );
		if( ptr )
		{
			CNrpProjectModule* nTech = new CNrpProjectModule( *ptr, *this );
			(*nTech)[ CODEVOLUME ] = static_cast< int >( baseCode * (float)(*nTech)[ BASE_CODE ] );
			_modules.push_back( nTech );
		}
	}
}

CNrpDevelopGame::CNrpDevelopGame( CNrpGameProject& refPr, CNrpCompany& ptrCompany ) 
: INrpDevelopProject( CLASS_DEVELOPGAME, refPr[ NAME ] )
{
	_InitializeOptions( refPr[ NAME ] );
    //компания, которая делает это модуль
	_self[ PARENTCOMPANY ] = &ptrCompany;
    _self[ INTERNAL_NAME ] =  extNameTemplate + ((NrpText)refPr[ NAME ]).Translit();
	assert( refPr[ PLATFORMNUMBER ] != (int)0 );
	_self[ PLATFORMNUMBER ] = refPr[ PLATFORMNUMBER ];
	_self[ GENRE_MODULE_NUMBER ] = refPr[ GENRE_MODULE_NUMBER ];
	int bcv = refPr[ ENGINE_CODEVOLUME ];

	CNrpGameEngine& ge = *(refPr[ GAME_ENGINE ].As<PNrpGameEngine>());
	_self[ GAME_ENGINE_NAME ] = ge[ INTERNAL_NAME ];
	_self[ GAME_ENGINE ] = &ge;

	CNrpProjectModule* extEngine = new CNrpProjectModule( PT_PLUGIN, *this );
	(*extEngine)[ NAME ] = Text( NAME ) + " Расширение движка";
	(*extEngine)[ INTERNAL_NAME ] = (NrpText)_self[ INTERNAL_NAME ] + "_extent";
	(*extEngine).SetEmployerSkillRequire( SKILL_CODING, ge[ SKILL_CODING ] );
	(*extEngine)[ CODEVOLUME ] = refPr[ ENGINE_CODEVOLUME ];
	(*extEngine)[ BASE_CODE ] = 1.f;
	_modules.push_back( extEngine );
	
	_self[ PREV_GAME ] = refPr[ PREV_GAME ];
	_self[ BASE_CODEVOLUME ] = refPr[ BASE_CODEVOLUME ];
	_self[ CODEVOLUME ] = refPr[ CODEVOLUME ];
	_self[ SCENARIO ] = refPr[ SCENARIO ];
	_self[ GLICENSE ] = refPr[ GLICENSE ];
	_self[ BASEQUALITY ] = refPr[ BASEQUALITY ];
	_self[ PROJECTREADY ] = false;
	_self[ QUALITY ] = int(0);

	_AddModulesFrom( *refPr[ SCRIPTENGINE ].As<CNrpTechnology*>(), bcv );
	_AddModulesFrom( *refPr[ MINIGAMEENGINE ].As<CNrpTechnology*>(), bcv );
	_AddModulesFrom( *refPr[ PHYSICSENGINE ].As<CNrpTechnology*>(), bcv );
	_AddModulesFrom( *refPr[ GRAPHICQUALITY ].As<CNrpTechnology*>(), bcv);
	_AddModulesFrom( *refPr[ SOUNDQUALITY ].As<CNrpTechnology*>(), bcv );
	_AddModulesFrom< CNrpTechnology >( refPr.GetTechs( PT_ADVTECH ), bcv );
	_AddModulesFrom< CNrpTechnology >( refPr.GetTechs( PT_GENRE ), bcv );
	_AddModulesFrom< CNrpTechnology >( refPr.GetTechs( PT_VIDEOTECH ), bcv );
	_AddModulesFrom< CNrpTechnology >( refPr.GetTechs( PT_SOUNDTECH ), bcv );
	_AddModulesFrom< CNrpTechnology >( refPr.GetTechs( PT_LANGUAGE ), bcv );
	_AddModulesFrom< CNrpPlatform >( refPr.GetPlatforms(), bcv );

	_self[ PROJECTSTATUS ] = NrpText( "develop" );
	_self[ MODULE_NUMBER ] = static_cast< int >( _modules.size() );
}

CNrpDevelopGame::CNrpDevelopGame( const NrpText& name, CNrpCompany* ptrCompany )
: INrpDevelopProject( CLASS_DEVELOPGAME, name )
{
	_InitializeOptions( name );
	_self[ PARENTCOMPANY ] = ptrCompany;
    _self[ INTERNAL_NAME ] =  extNameTemplate + name.Translit();
}

CNrpDevelopGame::CNrpDevelopGame( const NrpText& fileName ) 
: INrpDevelopProject( CLASS_DEVELOPGAME, fileName )
{
	_InitializeOptions( "" );

	Load( fileName );
}

//завершили разработку модуля
void CNrpDevelopGame::ModuleFinished( IWorkingModule& module )
{
	USERS uList = module.GetUsers();
	
	for( size_t k=0; k < uList.size(); k++ )
	{
		 SetDeveloper( *uList[ k ] );
		 int growExp = (int)module[ CODEVOLUME ] / (int)_self[ BASE_CODEVOLUME ];
		 //опыт разработчика растет по мере выполнения компонентов
		 //а если у разработчкиа не было опыта в этом жанре, то он появляется
		 if( CNrpTechnology* genre = GetGenre( 0 ) )
			 uList[ k ]->IncreaseExperience( (*genre)[ INTERNAL_NAME ], growExp );
		 else
			 assert( genre && "Genre must exists ff:CNrpDevelopGame::ModuleFinished()" );
	}

	//вызовем функцию скрипта, которая отвечает за обработку события завершения обработки модуля
	_nrpApp.PCall( APP_MODULE_FINISHED, this, &module );
}

NrpText CNrpDevelopGame::Save( const NrpText& folderSave )
{
	assert( OpFileSystem::IsExist( folderSave ) );

	NrpText localFolder = OpFileSystem::CheckEndSlash( folderSave + (NrpText)_self[ INTERNAL_NAME ] );

	OpFileSystem::CreateDirectory( localFolder );

	NrpText fileName = localFolder + saveTemplate;

	//Запомним где лежит сценарий
	if( PNrpScenario sxn = _self[ SCENARIO ].As<PNrpScenario>() )
		_self[ SCENARIO_NAME ] = sxn->Save( localFolder );

	//Запомним где лежит лицензия
	if( PNrpLicense sln = _self[ GLICENSE ].As<PNrpLicense>() )
		_self[ LICENSE_NAME ] = sln->Save( localFolder );

	INrpDevelopProject::Save( fileName );

	IniFile sf( fileName );

	for( u32 i=0; i < _modules.size(); i++ )
	{
		NrpText saveFile = _modules[ i ]->Save( localFolder );
		sf.Set( SECTION_MODULES, CreateKeyModule(i), saveFile );
	}

	sf.Save();
	return localFolder;
}

void CNrpDevelopGame::Load( const NrpText& loadFolder )
{
	assert( OpFileSystem::IsExist(loadFolder) );
	NrpText myFolder = OpFileSystem::CheckEndSlash(loadFolder);

	NrpText fileName = myFolder + saveTemplate;
	CNrpCompany* ptrCompany = _self[ PARENTCOMPANY ].As<CNrpCompany*>();
	INrpProject::Load( fileName );

	IniFile rf( fileName );

	for( int i=0; i < (int)_self[ MODULE_NUMBER ]; ++i )
	{
		NrpText saveFile = rf.Get( SECTION_MODULES, CreateKeyModule(i), NrpText("") );
		
		CNrpProjectModule* tech = new CNrpProjectModule( PROJECT_TYPE( 0 ), *this );
		tech->Load( saveFile );
		_modules.push_back( tech );		
	}

    _self[ GAME_ENGINE ] = CNrpGameMarket::Instance().GetGameEngine( (NrpText)_self[ GAME_ENGINE_NAME ] );
	_self[ SCENARIO ] = new CNrpScenario( (NrpText)_self[ LICENSE_NAME ] );
	_self[ GLICENSE ] = new CNrpLicense( (NrpText)_self[ SCENARIO_NAME ] );
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
			float prc = (*_modules[ i ])[ READYWORKPERCENT ];
			if( prc < 1 )
				ready = false;		
	
			workP += prc;
			workP /= 2.f;
		}
	}

	_self[ PROJECTREADY ] = ready;
	_self[ READYWORKPERCENT ] = workP;
	if( ready )
		_self[ PROJECTSTATUS ] = NrpText( "produce" );

	return ready;
}

CNrpProjectModule* CNrpDevelopGame::GetGenre( size_t index ) const
{
	int position = 0;
	for( u32 i=0; i < _modules.size(); i++ )
	{
		if( (int)(*_modules[ i ])[ TECHGROUP ] == PT_GENRE )
		{
			if( position == index )
				return _modules[ i ];
			else
				position++;
		}
	}

	return NULL;
}

CNrpProjectModule* CNrpDevelopGame::GetModule( u32 index ) const
{
	assert( index < _modules.size() && "CNrpDevelopGame::GetModule index out of range" );
	return index < _modules.size() ? _modules[ index ] : NULL; 
}

CNrpProjectModule* CNrpDevelopGame::GetModule( const NrpText& name ) const
{
	for( u32 i=0; i < _modules.size(); i++ )
	{
		if( _modules[ i ]->Equale( name ) )
			return _modules[ i ];
	}

	assert( false && "CNrpDevelopGame::GetModule can't find module with getting name" );
	return NULL;
}

void CNrpDevelopGame::ModuleTested( IWorkingModule& module )
{
	const USERS& uList = module.GetUsers();

	for( size_t k=0; k < uList.size(); k++ )
	{
		int growExp = (int)module[ CODEVOLUME ] / (int)_self[ BASE_CODEVOLUME ];
		growExp += uList[ k ]->GetSkill( SKILL_TESTING );
		//опыт тестера растет по мере тестирования компонентов
		//а если у разработчкиа не было опыта в тестировани
		uList[ k ]->SetSkill( SKILL_TESTING, growExp );
	}

	_nrpApp.PCall( APP_MODULE_TESTED, this, &module );
}
}//end namespace nrp