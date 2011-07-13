#include "StdAfx.h"
#include "NrpCompany.h"
#include "IUser.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpTechnology.h"
#include "NrpGame.h"
#include "NrpApplication.h"
#include "NrpDevelopGame.h"
#include "NrpInvention.h"
#include "OpFileSystem.h"
#include "IniFile.h"
#include "NrpTester.h"
#include "NrpBookKeeping.h"
#include "NrpLaborMarket.h"
#include "NrpGameMarket.h"
#include "NrpHistory.h"

#include <errno.h>
#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_NRPCOMPANY( "CNrpCompany" );

const NrpText CNrpCompany::postfix = L"_company";
const NrpText CNrpCompany::saveTemplate = L"item.company";
const NrpText CNrpCompany::historyTemplate = L"company.history";

void CNrpCompany::_InitialyzeOptions()
{
	RegProperty( BALANCE, 100000 );
	RegProperty<NrpText>( NAME, "" );
	RegProperty<PUser>( CEO, NULL );
	RegProperty( ENGINES_NUMBER, 0 );
	RegProperty( TECHNUMBER, 0 );
	RegProperty( USERNUMBER, 0 );
	RegProperty( PROJECTNUMBER, 0 );
	RegProperty( GAMENUMBER, 0 );
	RegProperty<NrpText>( INTERNAL_NAME, "" );
	RegProperty( OBJECTSINPORTFELLE, 0 );
	RegProperty( DEVELOPPROJECTS_NUMBER, 0 );
	RegProperty( FAMOUS, 0.1f );
	RegProperty( INVENTIONSNUMBER, 0 );
	RegProperty( MONEY_ON_PIE, 0.f );
	RegProperty( DIVIDEND, 0.f );
	RegProperty( PROFIT_LASTYEAR, 0 );
	RegProperty( PIE_NUMBER, 0 );
	RegProperty( PIE_COST, 0.f );
	RegProperty( ENDDATE, NrpTime( 0. ) );
	RegProperty( STARTDATE, NrpTime( 0. ) );
	RegProperty( SELF_PIE_NUMBER, 0 );
	RegProperty<NrpText>( TEXTURENORMAL, "" );
	RegProperty<CNrpBookKeeping*>( BOOKKEEPING, NULL );
	RegProperty<int>( INVESTMENT_EXPECTATIONS, 0 );
    RegProperty( HISTORY, new CNrpHistory() );
}

CNrpCompany::CNrpCompany( const NrpText& name, CNrpUser* ceo ) : INrpConfig( CLASS_NRPCOMPANY, name)
{
	_InitialyzeOptions();

	_self[ NAME ] = name;
    NrpText internalName = "";
    for( int index=0; index < name.size(); index++ )
        if( isalpha( name[ index ] ) || core::isdigit( name[ index ] ) )
            internalName += name[ index ];
        
    _self[ INTERNAL_NAME ] = internalName + CNrpCompany::postfix;
	_self[ CEO ] = ceo;
}

CNrpCompany::CNrpCompany( const NrpText& fileName ) : INrpConfig( CLASS_NRPCOMPANY, fileName )
{
	_InitialyzeOptions();

	Load( fileName );
}

CNrpCompany::~CNrpCompany(void)
{
}

CNrpGameEngine* CNrpCompany::GetGameEngine( int index ) const
{
	return index < (int)_engines.size() ? _engines[ index ] : NULL;
}

CNrpGameEngine* CNrpCompany::GetGameEngine( const NrpText& name ) const
{
	return FindByNameAndIntName< ENGINES, CNrpGameEngine>( _engines, name );
}

void CNrpCompany::AddGameEngine( CNrpGameEngine* ptrEng )
{
	assert( ptrEng );
	if( ptrEng != NULL )
	{
		_engines.push_back( ptrEng );
		_self[ ENGINES_NUMBER ] = static_cast< int >( _engines.size() );
		(*ptrEng)[ COMPANYNAME ] = _self[ NAME ];
		(*ptrEng)[ PARENTCOMPANY ] = this;
	}
}

CNrpTechnology* CNrpCompany::GetTechnology( u32 index ) const
{
	return index < _technologies.size() ? _technologies[ index ] : NULL;
}

CNrpTechnology* CNrpCompany::GetTechnology( const NrpText& name ) const
{
	return FindByNameAndIntName< TECHS, CNrpTechnology>( _technologies, name );
}

void CNrpCompany::AddProject( INrpProject* ptrProject )
{
	assert( ptrProject != NULL );

	if( ptrProject == NULL )
		return;

	if( FindByNameAndIntName< PROJECTS, INrpProject >( _projects, NAME ) == NULL )
	{
		_projects.push_back( ptrProject );
		_self[ PROJECTNUMBER ] = static_cast< int >( _projects.size() );
		(*ptrProject)[ COMPANYNAME ] = _self[ NAME ];
	}
}

void CNrpCompany::AddUser( CNrpUser& user )
{
	if( !GetUser( (NrpText)user[ NAME ] ) )
	{
		_employers.push_back( &user );
		user[ SALARY ] = user[ WANTMONEY ];
		user[ PARENTCOMPANY ] = this;
		_self[ USERNUMBER ] = static_cast< int >( _employers.size() );
	}
}

CNrpUser* CNrpCompany::GetUser( int index ) const
{
	return index < (int)_employers.size() ? _employers[ index ] : NULL;
}

CNrpUser* CNrpCompany::GetUser( const NrpText& name ) const
{
	return FindByName< USERS, CNrpUser >( _employers, name );
}

NrpText CNrpCompany::Save( const NrpText& saveFolder )
{
	//переданная директория должна существовать
	assert( OpFileSystem::IsExist( saveFolder ) );

	NrpText localFolder = OpFileSystem::CheckEndSlash( saveFolder + Text( INTERNAL_NAME ) );

	//если нет директории в которую надо сохранять данные
	OpFileSystem::CreateDirectory( localFolder );

    NrpText saveFile = localFolder + CNrpCompany::saveTemplate;

	assert( !OpFileSystem::IsExist( saveFile ) );
	INrpConfig::Save( saveFile );

	IniFile rv( saveFile );

	rv.Set( SECTION_PROJECTS, _projects, CreateKeyItem, NAME );
	rv.Set( SECTION_DEVPROJECTS, _devProjects,	CreateKeyItem, NAME );
	rv.Set( SECTION_ENGINES, _engines,		CreateKeyItem, INTERNAL_NAME );
	rv.Set( SECTION_TECHS,			_technologies,	CreateKeyItem, INTERNAL_NAME );
	rv.Set( SECTION_GAMES,			_games,			CreateKeyItem, INTERNAL_NAME );
	rv.Set( SECTION_USERS,			_employers,		CreateKeyItem, NAME );
	rv.Set( SECTION_PORTFELLE,		_portfelle,		CreateKeyItem, NAME );
	rv.Set( SECTION_INVENTIONS,	_inventions,	CreateKeyItem, INTERNAL_NAME );

	rv.Save();

    assert( _self[ HISTORY ].As<CNrpHistory*>() && "history must be exist" );
    if( _self[ HISTORY ].As<CNrpHistory*>() )
        _self[ HISTORY ].As<CNrpHistory*>()->Save( localFolder + CNrpCompany::historyTemplate );

	return saveFile;
}

void CNrpCompany::_LoadArray( const NrpText& section, const NrpText& fileName, const NrpText& condition )
{
	IniFile rv( fileName );

	int maxCond = (int)_self[ condition ];
	for( int i=0; i < maxCond; i++ )
	{
		NrpText type = rv.Get( section, CreateKeyType(i), NrpText() );
		NrpText rName = rv.Get( section, CreateKeyItem(i), NrpText() );

		INrpConfig* conf = NULL;
		if( type == CNrpGameEngine::ClassName() )
		{
            AddGameEngine( CNrpGameMarket::Instance().GetGameEngine( rName ) );
		}
		else if( type == CNrpUser::ClassName() || type == NrpCoder::ClassName() || type == NrpDesigner::ClassName()
				 || type == NrpComposer::ClassName() || type == NrpTester::ClassName())
		{
            CNrpUser* user = _nrpLaborMarkt.GetUser( rName );
            assert( user );
            if( user )
			    AddUser( *user );
		}
		else if( type == CNrpTechnology::ClassName() )
		{
			AddTechnology( _nrpApp.GetTechnology( rName ) );
		}
		else if( type == CNrpGame::ClassName() )
		{
            AddGame( CNrpGameMarket::Instance().GetGame( rName ) );
		}
		else if( type == CNrpInvention::ClassName() )
		{
			AddInvention( _nrpApp.GetInvention( rName, Text( NAME ) ) );
		}
		else if( type == CNrpDevelopGame::ClassName() )
		{
			AddDevelopProject( _nrpApp.GetDevelopProject( rName ) );
		}
		/*else if( type == INrpProject::ClassName() )
		{
			if( conf = app.GetGame( rName ) )
				_games.push_back( conf );
		}*/
		/*else if( type == ::ClassName() )
		{
			if( conf = app.GetGame( rName ) )
				_games.push_back( conf );
		}*/
	}
	
	//SetValue( condition, arrayT.size() );
}

void CNrpCompany::Load( const NrpText& pathTo )
{
	NrpText loadFile = pathTo;

	if( OpFileSystem::IsFolder( pathTo ) )
        loadFile = OpFileSystem::CheckEndSlash( pathTo ) + CNrpCompany::saveTemplate;

	assert( OpFileSystem::IsExist( loadFile ) );

	if( !OpFileSystem::IsExist( loadFile ) )
		return;

	INrpConfig::Load( loadFile );

	PUser ceo = _self[ CEO ].As<PUser>();
	if( ceo )
	    (*ceo)[ PARENTCOMPANY ] = this;

	_LoadArray( SECTION_ENGINES, loadFile, ENGINES_NUMBER );
	_LoadArray( SECTION_USERS, loadFile, USERNUMBER );
	_LoadArray( SECTION_TECHS, loadFile, TECHNUMBER );
	_LoadArray( SECTION_GAMES, loadFile, GAMENUMBER );
	_LoadArray( SECTION_INVENTIONS, loadFile, INVENTIONSNUMBER );
	//_LoadArray( SECTION_PROJECTS, loadFile, PROJECTNUMBER );
	_LoadArray( SECTION_DEVPROJECTS, loadFile, DEVELOPPROJECTS_NUMBER );

	IniFile rv( loadFile );

	int maxObj = (int)_self[ OBJECTSINPORTFELLE ];
	for( int k=0; k < maxObj; k++ )
	{
		NrpText typeName = rv.Get( SECTION_PORTFELLE, CreateKeyType( k ), NrpText() );
		NrpText prjName = rv.Get( SECTION_PORTFELLE, CreateKeyModule( k ), NrpText() );
		if( typeName == CNrpDevelopGame::ClassName() )
			AddToPortfelle( GetDevelopProject( prjName ) );
	}

    NrpText historyIni = OpFileSystem::CheckEndSlash( pathTo ) + CNrpCompany::historyTemplate;
    if( OpFileSystem::IsExist( historyIni ) )
        _self[ HISTORY ].As<CNrpHistory*>()->Load( historyIni );
    else
        _CreateHistory();
}

void CNrpCompany::_CreateHistory()
{
   CNrpHistory* hs  = _self[ HISTORY ].As<CNrpHistory*>();

   CNrpHistoryStep* historyStep = hs->AddStep( _nrpApp[ CURRENTTIME ].As<NrpTime>() );
   //вычисляем коеффициент продаж в этом месяце
   (*historyStep)[ BOXNUMBER ] = static_cast< int >( 0 );
   (*historyStep)[ BALANCE ] = static_cast< int >( 0 );
}

CNrpGame* CNrpCompany::GetGame( const NrpText& gameName ) const 
{
	return FindByNameAndIntName< GAMES, CNrpGame >( _games, gameName );
}

CNrpGame* CNrpCompany::GetGame( u32 index ) const
{
	return index < _games.size() ? _games[ index ] : NULL;
}

INrpProject* CNrpCompany::GetProject( const NrpText& name ) const
{
	return FindByNameAndIntName< PROJECTS, INrpProject >( _projects, name );
}

INrpProject* CNrpCompany::GetProject( size_t index ) const
{
	return index < _projects.size() ? _projects[ index ] : NULL;
}

void CNrpCompany::BeginNewHour( const NrpTime& time  )
{
	for( size_t cnt=0; cnt < _employers.size(); cnt++ )
		_employers[ cnt ]->BeginNewHour( time );
}

void CNrpCompany::BeginNewDay( const NrpTime& time )
{
	//уведомление о завершении проекта происходит на следующий день после окончания разработки
	//последнего модуля
	for( u32 i = 0; i < _devProjects.size(); i++ ) 
	{
		if( CNrpDevelopGame* prj = dynamic_cast< CNrpDevelopGame* >( _devProjects[ i ] ) )
		{
			if( prj->IsReady() )
			{
				const PNrpGame game = CreateGame( *prj );
				RemoveDevelopProject( (*prj)[ NAME ] );
				CNrpApplication::Instance().PCall( APP_PROJECT_FINISHED, this, game );
				break;
			}
		}
	}

	for( size_t cnt=0; cnt < _employers.size(); cnt++ )
		 _employers[ cnt ]->BeginNewDay( time );
}

void CNrpCompany::_PaySalaries()
{
	int workersSalary = 0;
	for( size_t cnt=0; cnt < _employers.size(); cnt++ )
	{
		int salary = (*_employers[ cnt ])[ SALARY ];	
		workersSalary += salary;

		(*_employers[ cnt ])[ BALANCE ] += salary;
	}
	_self[ BALANCE ] -= workersSalary;
}

void CNrpCompany::AddGame( CNrpGame* game )
{
	if( game && FindByNameAndIntName< GAMES, CNrpGame >( _games, (*game)[ NAME ] ) == NULL )
	{
		_games.push_back( game );
		(*game)[ PARENTCOMPANY ] = this;
		(*game)[ COMPANYNAME ] = _self[ NAME ];
		_self[ GAMENUMBER ] = static_cast< int >( _games.size() );
	}
}

CNrpGame* CNrpCompany::CreateGame( const CNrpDevelopGame& devGame )
{
	CNrpGame* ptrGame = new CNrpGame( devGame, this );
    assert( ptrGame && "new game must be exist" );
	(*ptrGame)[ STARTDATE ] = _nrpApp[ CURRENTTIME ];
    CNrpGameMarket::Instance().AddGame( *ptrGame );
	//!!!!!!!!!!!!!!!!RemoveFromPortfelle( devGame );

	_games.push_back( ptrGame );
	_self[ GAMENUMBER ] = static_cast< int >( _games.size() );

	return ptrGame;
}

void CNrpCompany::RemoveProject( const NrpText& name )
{
	for( u32 i=0; i < _projects.size(); i++ ) 
		if( _projects[ i ]->Equale( name ) )
			_projects.erase( i );

	_self[ PROJECTNUMBER ] = static_cast< int >( _projects.size() );
}

void CNrpCompany::BeginNewMonth( const NrpTime& time )
{
	_PaySalaries();
}

void CNrpCompany::AddToPortfelle( INrpConfig* ptrObject )
{
	_portfelle.push_back( ptrObject );
	_self[ OBJECTSINPORTFELLE ] = static_cast< int >( _portfelle.size() );
}

INrpConfig* CNrpCompany::GetFromPortfelle( size_t index ) const
{
	assert( index < _portfelle.size() );
	return index < _portfelle.size() ? _portfelle[ index ] : NULL;	
}

float CNrpCompany::GetUserModificatorForGame( CNrpGame& game )
{
	return 1;
}

void CNrpCompany::AddDevelopProject( INrpDevelopProject* ptrDevProject )
{
	assert( ptrDevProject != NULL );
	if( ptrDevProject == NULL )
		return;

	if( FindByNameAndIntName< DEVPROJECTS, INrpDevelopProject >( _devProjects, ptrDevProject->Text( NAME ) ) == NULL )
		_devProjects.push_back( ptrDevProject );

	_self[ DEVELOPPROJECTS_NUMBER ] = static_cast< int >( _devProjects.size() );
	(*ptrDevProject)[ COMPANYNAME ] = _self[ NAME ];
}

INrpDevelopProject* CNrpCompany::GetDevelopProject( const NrpText& name ) const
{
	return FindByNameAndIntName<DEVPROJECTS, INrpDevelopProject>( _devProjects, name );
}

INrpDevelopProject* CNrpCompany::GetDevelopProject( u32 index ) const
{
	assert( index < _devProjects.size() );
	return index < _devProjects.size() ? _devProjects[ index ] : NULL;	
}

CNrpInvention* CNrpCompany::GetInvention( int index )
{
	assert( index >= 0 && index < (int)_inventions.size() );
	return index < (int)_inventions.size() ? _inventions[ index ] : NULL;	
}

void CNrpCompany::RemoveDevelopProject( const NrpText& name )
{
	for( u32 i=0; i < _devProjects.size(); i++ ) 
		if( _devProjects[ i ]->Equale( name ) )
			_devProjects.erase( i );

	_self[ DEVELOPPROJECTS_NUMBER ] = static_cast< int >( _devProjects.size() );
	_nrpApp.RemoveDevelopProject( name );
}

void CNrpCompany::RemoveFromPortfelle( const INrpConfig* ptrObject )
{
	for( u32 i=0; i < _portfelle.size(); i++ ) 
		if( _portfelle[ i ] == ptrObject )
			_portfelle.erase( i );

	_self[ OBJECTSINPORTFELLE ] = static_cast< int >( _portfelle.size() );
}

void CNrpCompany::AddInvention( CNrpInvention* const inv )
{
	assert( inv != NULL );

	if( inv && FindByNameAndIntName<INVENTIONS, CNrpInvention>( _inventions, inv->Text( NAME ) ) == NULL )
	{
		_inventions.push_back( inv );
		(*inv)[ PARENTCOMPANY ] = this;
		_self[ INVENTIONSNUMBER ] = static_cast< int >( _inventions.size() );
	}
}

void CNrpCompany::InventionReleased( const CNrpInvention& inv )
{
	NrpText name = inv[ NAME ];
	for( u32 p=0; p < _inventions.size(); p++ )
	{
		if( _inventions[ p ]->Equale( name, _self[ name ] ) )
		{
			//надо что-то делать с похожей технологией... либо развивать следующий уровень,
			//либо прекращать разработки и переводить людей на другой проект с частичным
			//переносом опыта...
			//в любом случае текущие иследования прекращаются...
			_nrpApp.InventionCanceled(  _inventions[ p ] );
			_nrpApp.PCall( COMPANY_DUPLICATE_INVENTION_FINISHED, this, _inventions[ p ] );
			_inventions.erase( p );
			break;
		}			
	}
}

void CNrpCompany::AddTechnology( CNrpTechnology* tech )
{
	assert( tech );
	if( tech && FindByNameAndIntName< TECHS, CNrpTechnology>( _technologies, tech->Text( NAME ) )== NULL )
	{
		_technologies.push_back( tech );
		_self[ TECHNUMBER ] = static_cast< int >( _technologies.size() );
	}
}

void CNrpCompany::RemoveInvention( CNrpInvention& inv )
{
	for( u32 i=0; i < _inventions.size(); i++ )
		if( _inventions[ i ] == &inv )
		{
			_inventions.erase( i );
			break;
		}

	for( u32 i=0; i < _employers.size(); i++ )
		_employers[ i ]->RemoveWork( inv );
}

CNrpInvention* CNrpCompany::GetInvention( const NrpText& name )
{
	return FindByNameAndIntName< INVENTIONS, CNrpInvention>( _inventions, name );
}

void CNrpCompany::RemoveUser( const NrpText& name )
{
	for( u32 i=0; i < _employers.size(); i++ )
		if( _employers[ i ]->Equale( name ) )
		{
			CNrpUser& user = *_employers[ i ];
			_employers.erase( i );

			user[ WANTMONEY ] = static_cast< int >( (int)user[ SALARY ] * 1.5f );
			user[ PARENTCOMPANY ] = (CNrpCompany*)NULL;
			_self[ USERNUMBER ] = static_cast< int >( _employers.size() );

			break;
		}
}

NrpText CNrpCompany::ClassName()
{
	return CLASS_NRPCOMPANY;
}

void CNrpCompany::BeginNewYear( const NrpTime& time )
{
	_self[ PROFIT_LASTYEAR ] = _self[ BALANCE ];
}

}//namespace nrp