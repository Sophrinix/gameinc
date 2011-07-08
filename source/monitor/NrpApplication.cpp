#include "StdAfx.h"
#include "NrpApplication.h"
#include "NrpCompany.h"
#include "IUser.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpTechnology.h"
#include "NrpBank.h"
#include "NrpAiUser.h"
#include "NrpPlayer.h"
#include "OpFileSystem.h"
#include "nrpEngine.h"
#include "NrpGame.h"
#include "NrpDiskMachine.h"
#include "NrpPlant.h"
#include "NrpGameBox.h"
#include "NrpRetailer.h"
#include "NrpExtInfo.h"
#include "NrpDevelopGame.h"
#include "NrpInvention.h"
#include "NrpActionType.h"
#include "NrpPda.h"
#include "nrploadingscreen.h"
#include "NrpHistory.h"
#include "NrpGameTime.h"
#include "NrpTime.h"
#include "IniFile.h"
#include "nrpScript.h"
#include "NrpHtmlEngineConfig.h"
#include "HTMLEngine.h"
#include "NrpPluginEngine.h"
#include "NrpApplication.h"
#include "NrpBank.h"
#include "nrpPlant.h"
#include "OpFileSystem.h"
#include "nrpVideoConfig.h"
#include "nrpHUDConfig.h"
#include "NrpConsoleConfig.h"
#include "NrpPlatform.h"
#include "NrpBridge.h"
#include "NrpLaborMarket.h"
#include "NrpSoundEngine.h"
#include "NrpQuestEngine.h"
#include "NrpLinkHolder.h"
#include "NrpGameMarket.h"

#include <io.h>
#include <errno.h>
#include <OleAuto.h>

static nrp::CNrpApplication* globalApplication = NULL;

namespace nrp
{
CLASS_NAME CLASS_NRPAPPLICATION( "CNrpApplication" );

const NrpText CNrpApplication::saveTemplate = L"profile.ini";
const NrpText CNrpApplication::oldTemplate = L"Old/";
const NrpText CNrpApplication::bakTemplate = L".bak";

CNrpApplication::CNrpApplication(void) : INrpConfig( CLASS_NRPAPPLICATION, CLASS_NRPAPPLICATION )
{
	RegProperty( TECHNUMBER, (int)0 );
	RegProperty( COMPANIESNUMBER, (int)0 );

	RegProperty<NrpText>( WORKDIR, "" );
	RegProperty<NrpText>( SAVEDIR, "save/" );
	RegProperty<NrpText>( SAVEDIR_INVENTIONS, "" );
	RegProperty<NrpText>( SAVEDIR_COMPANIES, "" );
	RegProperty<NrpText>( SAVEDIR_DEVPR, "" );
	RegProperty<NrpText>( SAVEDIR_GAMES, "" );
	RegProperty<NrpText>( SAVEDIR_PLATFORMS, "" );
	RegProperty<NrpText>( SAVEDIR_PROJECTS, "" );
	RegProperty<NrpText>( SAVEDIR_ENGINES, "" );
	RegProperty<NrpText>( SAVEDIR_USERS, "" );
	RegProperty<NrpText>( SAVEDIR_PLANT, "" );
	RegProperty<NrpText>( SAVEINI_PROFILE, "" );
	RegProperty<NrpText>( SAVEDIR_PROFILE, "" );
	RegProperty<NrpText>( SAVEDIR_TECHS, "" );
	RegProperty<NrpText>( SAVEDIR_BRIDGE, "" );
	RegProperty<NrpText>( SAVEDIR_BANK, "" );
	RegProperty( SYSTEMINI, NrpText( "config/system.ini" ) );

	IniFile rv( (NrpText)Param( SYSTEMINI ) );
	RegProperty( PROFILENAME, rv.Get( SECTION_OPTIONS, "currentProfile", NrpText( "dalerank" ) ) );
	RegProperty( PROFILECOMPANY, rv.Get( SECTION_OPTIONS, "currentCompany", NrpText( "daleteam" ) ) );

	RegProperty( CURRENTTIME, NrpTime( 0. ) );
	RegProperty( BOXADDONNUMBER, (int)0 );
	RegProperty( DEVELOPPROJECTS_NUMBER, 0 );
	RegProperty( PROJECTNUMBER, (int)0 );
	RegProperty<PNrpCompany>( PLAYERCOMPANY, NULL );
	RegProperty( INVENTIONSNUMBER, (int)0 );
	RegProperty( MINIMUM_USER_SALARY, (int)250 );
	RegProperty<CNrpPda*>( PDA, new CNrpPda() );
	RegProperty<CNrpGameTime*>( GAME_TIME, new CNrpGameTime( this ) );
	RegProperty( PAUSEBTWSTEP, 100 );
	RegProperty( INFLATION, 0.02f );
	RegProperty( DEV_FORCE, 1.f );
	RegProperty( PROFIT_TAX, 0.18f );

	srand( GetTickCount() );
}

CNrpApplication::~CNrpApplication(void)
{
}

CNrpCompany* CNrpApplication::GetCompany( const NrpText& companyName )
{
	return FindByNameAndIntName< COMPANIES, CNrpCompany >( _companies, companyName );
}

CNrpCompany* CNrpApplication::GetCompany( u32 index )
{
	return index < _companies.size() ? _companies[ index ] : NULL;
}

COMPANIES& CNrpApplication::GetCompanies()
{
	return _companies;
}

int CNrpApplication::AddCompany( CNrpCompany* company )
{
	assert( company );
	if( company )
	{
		_companies.push_back( company );

		_self[ COMPANIESNUMBER ] = static_cast< int >( _companies.size() );
		PUser ceo = (*company)[ CEO ].As<PUser>();
		
		assert( ceo );
		if( ceo && ceo->ObjectTypeName() == CNrpPlayer::ClassName() )
			_self[ PLAYERCOMPANY ] = company;

		return 1;
	}
	return 0;
}

void CNrpApplication::AddProject( INrpProject* project )
{
	if( FindByName<PROJECTS, INrpProject>( _projects, (*project)[ NAME ] ) == NULL )
		_projects.push_back( project );
}

CNrpTechnology* CNrpApplication::GetTechnology( int index )
{
	return index < (int)_technologies.size() ?  _technologies[ index ] : NULL;
}

CNrpTechnology* CNrpApplication::GetTechnology( const NrpText& name )
{
	return FindByNameAndIntName<TECHS, CNrpTechnology>( _technologies, name );
}

void CNrpApplication::AddTechnology( CNrpTechnology* ptrTech )
{
	assert( ptrTech );
	if( GetTechnology( (NrpText)(*ptrTech)[ INTERNAL_NAME ] ) == NULL )
		_technologies.push_back( ptrTech );

	_self[ TECHNUMBER ] = static_cast<int>( _technologies.size() );
}

void CNrpApplication::_CreateDirectoriesMapForSave()
{
	OpFileSystem::CreateDirectory( _self[ SAVEDIR ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_PROFILE ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_INVENTIONS ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_COMPANIES ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_DEVPR ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_GAMES ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_PLATFORMS ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_PROJECTS ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_ENGINES ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_PLANT ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_USERS ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_BANK ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_BRIDGE ] );
}

void CNrpApplication::Save()
{
	scene::CLoadingScreen ld( _nrpEngine.GetVideoDriver(), _nrpEngine.GetGuiEnvironment()->getFont( "font_14" ) );

    NrpText prevSaveFolder = (NrpText)_self[ SAVEDIR ] + (NrpText)_self[ PROFILENAME ] + CNrpApplication::oldTemplate;
    NrpText profileIni = (NrpText)_self[ SAVEDIR_PROFILE ] + CNrpApplication::saveTemplate;

	ld.render( 10, "Создаем структуру для сохранения" );
	OpFileSystem::Remove( prevSaveFolder );
	OpFileSystem::Copy( _self[ SAVEDIR_PROFILE ], prevSaveFolder );
    OpFileSystem::Rename( prevSaveFolder + CNrpApplication::saveTemplate, prevSaveFolder + CNrpApplication::saveTemplate + CNrpApplication::bakTemplate );
	OpFileSystem::Remove( _self[ SAVEDIR_PROFILE ] );

	_CreateDirectoriesMapForSave();

	assert( !OpFileSystem::IsExist( profileIni ) );

	ld.render( 10, "Сохраняем основные данные" );
	INrpConfig::Save( profileIni );

	IniFile sv( profileIni );

	sv.Set( SECTION_OPTIONS, "currentProfile", (NrpText)_self[ PROFILENAME ] );
	sv.Set( SECTION_OPTIONS, "currentCompany", (NrpText)_self[ PROFILECOMPANY ] );
	
	ld.render( 10, "Сохраняем данные о компаниях" );
	for( u32 i=0; i < _companies.size(); i++ )
	{
		_companies[ i ]->Save( _self[ SAVEDIR_COMPANIES ] );
		sv.Set( SECTION_COMPANIES, CreateKeyCompany( i ), (NrpText)(*_companies[ i ])[ INTERNAL_NAME ]);
	}

	ld.render( 10, "Сохраняем данные о проектах в разработке" );
	for( u32 i=0; i < _devProjects.size(); i++ )
	{
		NrpText saveFile = _devProjects[ i ]->Save( _self[ SAVEDIR_DEVPR ] );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyType(i), _devProjects[ i ]->ObjectTypeName() );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyProject(i), saveFile );
	}

	ld.render( 10, "Сохраняем данные о проектах" );
	for( u32 i=0; i < _projects.size(); i++ )
	{
		NrpText saveFile = _projects[ i ]->Save( _self[ SAVEDIR_PROJECTS ] );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyType(i), _projects[ i ]->ObjectTypeName() );
		sv.Set( SECTION_PROJECTS, CreateKeyProject(i), saveFile );
	}

	ld.render( 10, "Сохраняем данные о технологиях" );
	for( u32 i=0; i < _technologies.size(); i++ )
	{
		NrpText saveFile = _technologies[ i ]->Save( _self[ SAVEDIR_TECHS ] );
		sv.Set( SECTION_TECHS, CreateKeyTech(i), saveFile );
	}

	ld.render( 10, "Сохраняем данные oб изобретениях" );
	for( u32 i=0; i < _inventions.size(); i++ )
	{
		NrpText inventSaveFile = _inventions[ i ]->Save( (NrpText)_self[ SAVEDIR_INVENTIONS ] + 
														 (NrpText)_inventions[ i ]->Param( COMPANYNAME ), true );
		sv.Set( SECTION_INVENTIONS, CreateKeyInvention( i ), inventSaveFile );
	}

	sv.Save();

    ld.render( 10, "Сохраняем данные об игровом рынке" );
    CNrpGameMarket::Instance().Save( _self[ SAVEDIR_PROFILE ] );

	ld.render( 10, "Сохраняем данные банка" );
	CNrpBank::Instance().Save( _self[ SAVEDIR_BANK ] );

	ld.render( 10, "Сохраняем данные о биржи" );
	CNrpBridge::Instance().Save( _self[ SAVEDIR_BRIDGE ] );

	ld.render( 10, "Сохраняем данные о сотрудниках" );
	CNrpLaborMarket::Instance().Save( _self[ SAVEDIR_PROFILE ] );

    ld.render( 10, "Сохраняем результаты квестов" );
    CNrpQuestEngine::Instance().Save( _self[ SAVEDIR_PROFILE ] );

    ld.render( 15, "Сохраняем линки" );
    _nrpLinks.Save( OpFileSystem::CheckEndSlash( _self[ SAVEDIR_PROFILE ] ) + CNrpLinkHolder::saveTemplate );
}

void CNrpApplication::_InitialyzeSaveDirectories( const NrpText& profileName )
{
	NrpText profileDir = OpFileSystem::CheckEndSlash( (NrpText)_self[ SAVEDIR ] + profileName );
	_self[ SAVEDIR_PROFILE ] = profileDir;
	_self[ SAVEDIR_INVENTIONS ] = profileDir + L"inventions/";
    _self[ SAVEDIR_COMPANIES ] = profileDir + L"companies/";
	_self[ SAVEDIR_DEVPR ] = profileDir + L"devProjects/";
	_self[ SAVEDIR_GAMES ] = profileDir + L"games/";
	_self[ SAVEDIR_PLATFORMS ] = profileDir + L"platforms/";
	_self[ SAVEDIR_ENGINES ] = profileDir + L"engines/";
	_self[ SAVEDIR_PROJECTS ] = profileDir + L"projects/";
	_self[ SAVEDIR_PLANT ] = profileDir + L"plant/";
	_self[ SAVEDIR_USERS ] = profileDir + L"users/";
    _self[ SAVEINI_PROFILE ] = profileDir + CNrpApplication::saveTemplate;
	_self[ SAVEDIR_TECHS ] = profileDir + L"techs/";
	_self[ SAVEDIR_BRIDGE ] = profileDir + L"bridge/";
	_self[ SAVEDIR_BANK ] = profileDir + L"bank/";
}

void CNrpApplication::Load( const NrpText& profileName, const NrpText& companyName )
{
	_InitialyzeSaveDirectories( profileName );

	CNrpScript::Instance().TemporaryScript( AFTER_LOAD_SCRIPT, CNrpScript::SA_CREATE );

	NrpText profileIni = _self[ SAVEINI_PROFILE ];
	INrpConfig::Load( profileIni );

	CNrpLaborMarket::Instance().Load( _self[ SAVEDIR_PROFILE ] );
    _nrpLinks.Load( OpFileSystem::CheckEndSlash( _self[ SAVEDIR_PROFILE ] ) + CNrpLinkHolder::saveTemplate );

	IniFile rv( profileIni );

	for( int i=0; i < (int)_self[ TECHNUMBER ]; i++ )
	{
		NrpText fileName = rv.Get( SECTION_TECHS, CreateKeyTech(i), NrpText("") );
		CNrpTechnology* tech = new CNrpTechnology( fileName ); //loading
		_technologies.push_back( tech );
	}

	for( int i=0; i < (int)_self[ DEVELOPPROJECTS_NUMBER ]; i++ )
	{
		NrpText type = rv.Get( SECTION_DEVPROJECTS, CreateKeyType( i ), NrpText("") );
		NrpText saveFile = rv.Get( SECTION_DEVPROJECTS, CreateKeyProject( i ), NrpText("") );
		if( type == CNrpDevelopGame::ClassName() )
		{
			CNrpDevelopGame* game = new CNrpDevelopGame( saveFile );
			_devProjects.push_back( game );
		}
		else if( type == NrpText("devengine") )
		{
			//
		}
	}

	for( int i=0; i < (int)_self[ PROJECTNUMBER ]; i++ )
	{
		NrpText fileName = rv.Get( SECTION_PROJECTS, CreateKeyProject( i ), NrpText("") );
		NrpText type = rv.Get( SECTION_PROJECTS, CreateKeyType( i ), NrpText("") );
		if( type == CNrpGameProject::ClassName() )
		{
			CNrpDevelopGame* game = new CNrpDevelopGame( (NrpText)_self[ SAVEDIR_PROJECTS ] + fileName );
			_projects.push_back( game );
		}
		else if( type == NrpText("devengine") )
		{
			//
		}
	}

	for( int k=0; k < (int)_self[ INVENTIONSNUMBER ]; k++ )
	{
		NrpText saveFile = rv.Get( SECTION_INVENTIONS, CreateKeyInvention( k ), NrpText("") );
		CNrpInvention* invention = new CNrpInvention( saveFile );
		_inventions.push_back( invention );
	}

	
    CNrpGameMarket::Instance().Load( _self[ SAVEDIR_PROFILE ] );
    CNrpBridge::Instance().Load( _self[ SAVEDIR_BRIDGE ] );
	CNrpBank::Instance().Load( _self[ SAVEDIR_BANK ] );
    CNrpQuestEngine::Instance().Load( (NrpText)_self[ SAVEDIR_PROFILE ] );	

    _LoadCompanies( rv );
    
    CNrpScript::Instance().TemporaryScript( AFTER_LOAD_SCRIPT, CNrpScript::SA_EXEC );
}

void CNrpApplication::_LoadCompanies( IniFile& ini )
{
    int maxComp = (int)_self[ COMPANIESNUMBER ];
    for( int k=0; k < maxComp; k++ )
    {
        NrpText fileName = ini.Get( SECTION_COMPANIES, CreateKeyCompany( k ), NrpText("") );
        if( !GetCompany( fileName ) )
        {
            CNrpCompany* cmp = new CNrpCompany( (NrpText)_self[ SAVEDIR_COMPANIES ] + fileName );
            AddCompany( cmp );
        }
    }
}

void CNrpApplication::CreateProfile( const NrpText& profileName, const NrpText& companyName )
{
	_self[ PROFILENAME ] = profileName;
	_self[ PROFILECOMPANY ] = companyName;

	_InitialyzeSaveDirectories( profileName );
}

void CNrpApplication::ResetData()
{
	ClearArray( _companies );
	CNrpLaborMarket::Instance().Reset();
	ClearArray( _technologies );
}

void CNrpApplication::_BeginNewDay()
{
	for( u32 i=0; i < _companies.size(); i++)
		 _companies[ i ]->BeginNewDay( _self[ CURRENTTIME ] );

	CNrpPlant::Instance().BeginNewDay();
    CNrpGameMarket::Instance().BeginNewDay();

    _UpdateInvention();
}

void CNrpApplication::_UpdateInvention()
{
	for( u32 k=0; k < _inventions.size(); k++ )
	{
		CNrpInvention& pInv = *(_inventions[ k ]);

		pInv.CheckParams( _self[ CURRENTTIME ].As<NrpTime>() );

		if( pInv[ READYWORKPERCENT ] >= 1.f )
		{
			pInv[ ENDDATE ] = _self[ CURRENTTIME ];
			InventionFinished( pInv );
			k--;
		}
	}
}

void CNrpApplication::Init()
{
#ifdef _DEBUG
	_self[ WORKDIR ] = OpFileSystem::UpDir( OpFileSystem::UpDir( __argv[ 0 ] ) );
#else
	_self[ WORKDIR ] = OpFileSystem::UpDir( __argv[ 0 ] );
#endif

	CNrpBank::Instance();
	CNrpBridge::Instance();
	plugin::CNrpPluginEngine::Instance();
	//инициализация систем
	CNrpHUDConfig::Instance();
	_nrpSound.Instance();
	CNrpVideoConfig::Instance();
	CNrpConsoleConfig::Instance();
	CNrpPlant::Instance();
	CNrpScript::Instance();
	CNrpHtmlEngineConfig& heConfig = CNrpHtmlEngineConfig::Instance();
	heConfig[ BASEDIR ] = OpFileSystem::CheckEndSlash( _self[ WORKDIR ] ) + (NrpText)heConfig[ BASEDIR ];

	//ожидаем подгрузки видео
	_nrpEngine.InitVideo();
	_nrpEngine.InitConsole();

	HTMLEngine::Instance();
}

CNrpApplication& nrp::CNrpApplication::Instance()
{
	if( !globalApplication )
		globalApplication = new CNrpApplication();

	return *globalApplication;
}

void CNrpApplication::_BeginNewMonth()
{
	for( u32 i=0; i < _inventions.size(); i++ )
		_inventions[ i ]->BeginNewMonth( _self[ CURRENTTIME ] );

	//начало месяца в компании
	for( u32 i=0; i < _companies.size(); i++)
		_companies[ i ]->BeginNewMonth( _self[ CURRENTTIME ] );

	//обновляем рейтинги игр
    CNrpGameMarket::Instance().BeginNewMonth();

	CNrpBridge::Instance().Update();
}

void CNrpApplication::_BeginNewHour()
{
	for( u32 i=0; i < _companies.size(); i++)
		 _companies[ i ]->BeginNewHour( _self[ CURRENTTIME ] );
}

bool CNrpApplication::AddBoxAddon( CNrpTechnology* tech )
{
	if( GetBoxAddon( (*tech)[ NAME ].As<NrpText>() ) == NULL  )
	{
		_boxAddons.push_back( tech );
		_self[ BOXADDONNUMBER ] = static_cast< int >( _boxAddons.size() );
		return true;
	}

	return false;
}

CNrpTechnology* CNrpApplication::GetBoxAddon( const NrpText& name )
{
	return FindByNameAndIntName< TECHS, CNrpTechnology >( _boxAddons, name );
}

void CNrpApplication::RemoveTechnology( CNrpTechnology* ptrTech )
{
	for( u32 i=0; i < _technologies.size(); i++ )
		if( _technologies[ i ] == ptrTech )
		{
			_technologies.erase( i );
			return;
		}

	Log(HW) << "unknown technology" << term;
}

INrpProject* CNrpApplication::GetProject( const NrpText& name )
{
	return FindByName< PROJECTS, INrpProject >( _projects, name );
}

void CNrpApplication::AddDevelopProject( nrp::INrpDevelopProject* project )
{
	if( FindByName< DEVPROJECTS, INrpDevelopProject >( _devProjects, (*project)[ NAME ] ) == NULL )
	{
		_devProjects.push_back( project );
		Param( DEVELOPPROJECTS_NUMBER ) = static_cast< int >( _devProjects.size() );
	}
}

INrpDevelopProject* CNrpApplication::GetDevelopProject( const NrpText& name )
{
	return FindByName<DEVPROJECTS, INrpDevelopProject>( _devProjects, name );
}

void CNrpApplication::RemoveDevelopProject( const NrpText& name )
{
	for( u32 i=0; i < _devProjects.size(); i++ )
	{
		if( _devProjects[ i ]->Equale( name ) )
		{
			delete _devProjects[ i ];
			_devProjects.erase( i );
			break;
		}
	}

	_self[ DEVELOPPROJECTS_NUMBER ] = static_cast< int >( _devProjects.size() );
}

void CNrpApplication::AddInvention( const NrpText& fileName, CNrpCompany& parentCompany )
{
	std::auto_ptr< CNrpTechnology > startTech( new CNrpTechnology( fileName ) );

	CNrpInvention* tmp = GetInvention( (*startTech)[ INTERNAL_NAME ], parentCompany[ NAME ] );

	assert( !tmp );

	if( !tmp )
	{
		CNrpInvention* inv = new CNrpInvention( *startTech, parentCompany, _self[ CURRENTTIME ].As<NrpTime>() );
		parentCompany.AddInvention( inv );	
		_inventions.push_back( inv );  
		_self[ INVENTIONSNUMBER ] = static_cast< int >( _inventions.size() );
	}
}

void CNrpApplication::InventionFinished( CNrpInvention& ptrInvention )
{
	//создать соответствующую изобретению технологию 
	//разместить её в списке доступных
	CNrpTechnology* tech = new CNrpTechnology( ptrInvention );
	AddTechnology( tech );

	int delPos = -1;
	for( u32 i=0; i < _inventions.size(); i++ )
	{
		CNrpInvention& inv = *(_inventions[ i ]);
		if( (NrpText)inv[ INTERNAL_NAME ] == (NrpText)ptrInvention[ INTERNAL_NAME ] )
		{
			CNrpCompany* pCmp = inv[ PARENTCOMPANY ].As<PNrpCompany>();
			if( pCmp == ptrInvention[ PARENTCOMPANY ].As<PNrpCompany>() )
			{
				delPos = i;//найти это изобретение в своем списке и удалить его оттуда...
				pCmp->AddTechnology( tech );
				pCmp->RemoveInvention( ptrInvention );
				continue;
			}
		
			pCmp->InventionReleased( ptrInvention );//уведомить все компании об изобретении технологии
		}
	}

	PCall( APP_INVENTION_FINISHED, tech );
	
	delete _inventions[ delPos ];
	_inventions.erase( delPos );
}

void CNrpApplication::InventionCanceled( CNrpInvention* ptrInvention )
{
	for( u32 i=0; i < _inventions.size(); i++ )
	{
		CNrpInvention* inv = _inventions[ i ];
		if( inv == ptrInvention )
		{
#ifdef _DEBUG
			NrpText text = NrpText( "Удалена технология " ) + (NrpText)(*inv)[ INTERNAL_NAME ];
			PNrpCompany cmp = (*inv)[ PARENTCOMPANY ].As<PNrpCompany>();
			assert( cmp != NULL );
			text.append( cmp ? (NrpText)(*cmp)[ NAME ] : NrpText("unknown company") );
			Log( HW ) << text << term;
#endif
			delete inv;
			_inventions.erase( i );
			break;
		}
	}
}

CNrpInvention* CNrpApplication::GetInvention( const NrpText& name, const NrpText& companyName )
{
	for( u32 i=0; i < _inventions.size(); i++ )
		if( _inventions[ i ]->Equale( name, companyName ) )
			return _inventions[ i ];

	return NULL;
}

NrpText CNrpApplication::ClassName()
{
	return CLASS_NRPAPPLICATION;
}

}//namespace nrp