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

	Add<int>( USERNUMBER, 0 );
	Add<NrpText>( PROJECTSTATUS, "unknown" );
	Add<CNrpCompany*>( PARENTCOMPANY, NULL );
	Add<PNrpGameEngine>( GAME_ENGINE, NULL );
	Add<NrpText>( PREV_GAME, "" );
	Add<int>( BASE_CODEVOLUME, 0 );
	Add<int>( BASEQUALITY, 0 );
	Add<int>( QUALITY, 0 );
	Add<int>( CODEVOLUME, 0 );
	Add<CNrpScenario*>( SCENARIO, NULL );
	Add<CNrpLicense*>( GLICENSE, NULL );
	Add<int>( MONEYONDEVELOP, 0 );
	Add<int>( PLATFORMNUMBER, 0 );
	Add<int>( GENRE_MODULE_NUMBER, 0 );
	Add<int>( LOCALIZATION, 0 );
	Add<float>( FAMOUS, 0 );

	Param( NAME ) = name;
}

void CNrpDevelopGame::_AddModulesFrom( const CNrpTechnology* tech, int baseCode )
{
	if( tech )
	{
		CNrpProjectModule* module = new CNrpProjectModule( const_cast< CNrpTechnology* >( tech ), this );
		(*module)[ CODEVOLUME ] = static_cast< int >( baseCode * (*module)[ BASE_CODE ].As<float>() );
		_modules.push_back( module );
	}
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
			CNrpProjectModule* nTech = new CNrpProjectModule( ptr, this );
			(*nTech)[ CODEVOLUME ] = static_cast< int >( baseCode * (float)(*nTech)[ BASE_CODE ] );
			_modules.push_back( nTech );
		}
	}
}

CNrpDevelopGame::CNrpDevelopGame( CNrpGameProject* nProject, CNrpCompany* ptrCompany ) 
: INrpDevelopProject( CLASS_DEVELOPGAME, nProject->Text( NAME ) )
{
	CNrpGameProject& refPr = *nProject;
	_InitializeOptions( nProject->Text( NAME ) );
    //��������, ������� ������ ��� ������
	Param( PARENTCOMPANY ) = ptrCompany;
	assert( nProject->Param( PLATFORMNUMBER ) != (int)0 );
	Param( PLATFORMNUMBER ) = refPr[ PLATFORMNUMBER ];
	Param( GENRE_MODULE_NUMBER ) = refPr[ GENRE_MODULE_NUMBER ];
	int bcv = refPr[ ENGINE_CODEVOLUME ];

	PNrpGameEngine ge = refPr[ GAME_ENGINE ].As<PNrpGameEngine>();
	Param( GAME_ENGINE ) = ge;

	CNrpProjectModule* extEngine = new CNrpProjectModule( PT_ENGINDEEXTEND, this );
	(*extEngine)[ NAME ] = Text( NAME ) + " ���������� ������";
	(*extEngine).SetEmployerSkillRequire( SKL_CODING, (*ge)[ SKILL_CODING ] );
	(*extEngine)[ CODEVOLUME ] = refPr[ ENGINE_CODEVOLUME ];
	(*extEngine)[ BASE_CODE ] = 1.f;
	_modules.push_back( extEngine );
	
	Param( PREV_GAME ) = refPr[ PREV_GAME ];
	Param( BASE_CODEVOLUME ) = refPr[ BASE_CODEVOLUME ];
	Param( CODEVOLUME ) = refPr[ CODEVOLUME ];
	Param( TECHTYPE ) = refPr[ TECHTYPE ];
	Param( SCENARIO ) = refPr[ SCENARIO ];
	Param( GLICENSE ) = refPr[ GLICENSE ];
	Param( BASEQUALITY ) = refPr[ BASEQUALITY ];
	Param( PROJECTREADY ) = false;
	Param( QUALITY ) = int(0);

	_AddModulesFrom( refPr[ SCRIPTENGINE ].As<CNrpTechnology*>(), bcv );
	_AddModulesFrom( refPr[ MINIGAMEENGINE ].As<CNrpTechnology*>(), bcv );
	_AddModulesFrom( refPr[ PHYSICSENGINE ].As<CNrpTechnology*>(), bcv );
	_AddModulesFrom( refPr[ GRAPHICQUALITY ].As<CNrpTechnology*>(), bcv);
	_AddModulesFrom( refPr[ SOUNDQUALITY ].As<CNrpTechnology*>(), bcv );
	_AddModulesFrom< CNrpTechnology >( refPr.GetTechList(), bcv );
	_AddModulesFrom< CNrpTechnology >( refPr.GetGenreList(), bcv );
	_AddModulesFrom< CNrpTechnology >( refPr.GetVideoTechList(), bcv );
	_AddModulesFrom< CNrpTechnology >( refPr.GetSoundTechList(), bcv );
	_AddModulesFrom< CNrpTechnology >( refPr.GetLanguageTechList(), bcv );
	_AddModulesFrom< CNrpPlatform >( refPr.GetPlatformsList(), bcv );

	Param( PROJECTSTATUS ) = NrpText( "develop" );
	Param( MODULE_NUMBER ) = static_cast< int >( _modules.size() );
}

CNrpDevelopGame::CNrpDevelopGame( const NrpText& name, CNrpCompany* ptrCompany )
: INrpDevelopProject( CLASS_DEVELOPGAME, name )
{
	_InitializeOptions( name );
	Param( PARENTCOMPANY ) = ptrCompany;
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
		 float growExp = (int)(*module)[ CODEVOLUME ] / (float)Param( BASE_CODEVOLUME );
		 GENRE_TYPE techType = (*GetGenre( 0 ))[ TECHTYPE ].As<GENRE_TYPE>();
		 //���� ������������ ������ �� ���� ���������� �����������
		 //� ���� � ������������ �� ���� ����� � ���� �����, �� �� ����������
		 uList[ k ]->IncreaseExperience( techType, static_cast< int >( growExp ) );
	}

	CNrpApplication::Instance().DoLuaFunctionsByType( APP_MODULE_FINISHED, module );
}

NrpText CNrpDevelopGame::Save( const NrpText& folderSave )
{
	assert( OpFileSystem::IsExist( folderSave ) );

	NrpText localFolder = OpFileSystem::CheckEndSlash( folderSave + Text( NAME ) );

	OpFileSystem::CreateDirectory( localFolder );

	NrpText fileName = localFolder + "item.devgame";
	INrpDevelopProject::Save( fileName );
	IniFile sf( fileName );

	for( u32 i=0; i < _modules.size(); i++ )
	{
		_modules[ i ]->Save( localFolder );
		sf.Set( SECTION_MODULES, CreateKeyModule(i), _modules[ i ]->Text( NAME ) );
	}

	if( PNrpGameEngine ge = Param( GAME_ENGINE ).As<PNrpGameEngine>() )
		sf.Set( SECTION_PROPERTIES, GAME_ENGINE, ge->Text( NAME ) );

	if( PNrpScenario sxn = Param( SCENARIO ).As<PNrpScenario>() )
	{
		sf.Set( SECTION_PROPERTIES, SCENARIO, sxn->Text(NAME) );
		sxn->Save( localFolder + SCENARIO + ".ini" );
	}

	if( PNrpLicense sln = Param( GLICENSE ).As<PNrpLicense>() )
	{
		sf.Set( SECTION_PROPERTIES, GLICENSE, sln->Text(NAME) );
		sln->Save( localFolder + GLICENSE + ".ini");
	}

	return folderSave;
}

void CNrpDevelopGame::Load( const NrpText& loadFolder )
{
	assert( OpFileSystem::IsExist(loadFolder) );
	NrpText myFolder = OpFileSystem::CheckEndSlash(loadFolder);

	NrpText fileName = myFolder + "item.devgame";
	CNrpCompany* ptrCompany = Param( PARENTCOMPANY ).As<CNrpCompany*>();
	INrpProject::Load( fileName );
	IniFile rf( fileName );

	for( int i=0; i < (int)Param( MODULE_NUMBER ); ++i )
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
	Param( GAME_ENGINE ) = CNrpApplication::Instance().GetGameEngine( name );

	name = rf.Get( SECTION_PROPERTIES, SCENARIO, NrpText("") );
	PNrpScenario scenario = new CNrpScenario( name );
	scenario->Load( myFolder + SCENARIO + ".ini" ); 
	Param( SCENARIO ) = scenario;

	name = rf.Get( SECTION_PROPERTIES, GLICENSE, NrpText("") );
	PNrpLicense license = new CNrpLicense( name );
	license->Load( myFolder + GLICENSE + ".ini" );
	Param( GLICENSE ) = license;
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

	Param( PROJECTREADY ) = ready;
	Param( READYWORKPERCENT ) = workP;
	if( ready )
		Param( PROJECTSTATUS ) = NrpText( "produce" );

	return ready;
}

CNrpProjectModule* CNrpDevelopGame::GetGenre( size_t index )
{
	int position = 0;
	for( u32 i=0; i < _modules.size(); i++ )
	{
		 if( _modules[ i ]->Param( TECHGROUP ).As<GENRE_TYPE>() == PT_GENRE && position == index )
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