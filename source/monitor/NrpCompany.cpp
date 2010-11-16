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

#include <errno.h>
#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_NRPCOMPANY( "CNrpCompany" );

void CNrpCompany::_InitialyzeOptions()
{
	CreateValue<int>( BALANCE, 100000 );
	CreateValue<NrpText>( NAME, "" );
	CreateValue<PUser>( CEO, NULL );
	CreateValue<int>( ENGINES_NUMBER, 0 );
	CreateValue<int>( TECHNUMBER, 0 );
	CreateValue<int>( USERNUMBER, 0 );
	CreateValue<int>( PROJECTNUMBER, 0 );
	CreateValue<int>( GAMENUMBER, 0 );
	CreateValue<int>( OBJECTSINPORTFELLE, 0 );
	CreateValue<int>( DEVELOPPROJECTS_NUMBER, 0 );
	CreateValue<float>( FAMOUS, 0.1f );
	CreateValue<int>( INVENTIONSNUMBER, 0 );
}

CNrpCompany::CNrpCompany( const NrpText& name, IUser* ceo ) : INrpConfig( CLASS_NRPCOMPANY, name)
{
	_InitialyzeOptions();

	SetString( NAME, name );
	SetValue<PUser>( CEO, ceo );
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
	if( ptrEng != NULL )
		_engines.push_back( ptrEng );

	SetValue<int>( ENGINES_NUMBER, _engines.size() );
	ptrEng->SetString( COMPANYNAME, this->GetString( NAME ) );
	ptrEng->SetValue<PNrpCompany>( PARENTCOMPANY, this );
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
		SetValue<int>( PROJECTNUMBER, _projects.size() );
		ptrProject->SetString( COMPANYNAME, this->GetString( NAME ) );
	}
}

void CNrpCompany::AddUser( IUser* user )
{
	if( !GetUser( user->GetValue<NrpText>( NAME ) ) )
	{
		_employers.push_back( user );
		user->SetValue<int>( SALARY, user->GetValue<int>( WANTMONEY ) );
		user->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		SetValue<int>( USERNUMBER, _employers.size() );
	}
}

IUser* CNrpCompany::GetUser( int index ) const
{
	assert( index >=0 && index < (int)_employers.size() );
	return index < (int)_employers.size() ? _employers[ index ] : NULL;
}

IUser* CNrpCompany::GetUser( const NrpText& name ) const
{
	return FindByName< USERS, IUser >( _employers, name );
}

NrpText CNrpCompany::Save( const NrpText& saveFolder )
{
	//переданная директория должна существовать
	assert( OpFileSystem::IsExist( saveFolder ) );

	NrpText localFolder = OpFileSystem::CheckEndSlash( saveFolder + GetString( NAME ) );

	//если нет директории в которую надо сохранять данные
	OpFileSystem::CreateDirectory( localFolder );

	NrpText saveFile = localFolder + "company.ini";

	assert( !OpFileSystem::IsExist( saveFile ) );
	INrpConfig::Save( saveFile );

	IniFile rv( saveFile );

	rv.SetArray( SECTION_PROJECTS,		_projects,		CreateKeyItem, NAME, true );
	rv.SetArray( SECTION_DEVPROJECTS,	_devProjects,	CreateKeyItem, NAME, true );
	rv.SetArray( SECTION_ENGINES,		_engines,		CreateKeyItem, INTERNAL_NAME, true );
	rv.SetArray( SECTION_TECHS,			_technologies,	CreateKeyItem, INTERNAL_NAME, true );
	rv.SetArray( SECTION_GAMES,			_games,			CreateKeyItem, INTERNAL_NAME );
	rv.SetArray( SECTION_USERS,			_employers,		CreateKeyItem, NAME, true );
	rv.SetArray( SECTION_PORTFELLE,		_portfelle,		CreateKeyItem, NAME, true );
	rv.SetArray( SECTION_INVENTIONS,	_inventions,	CreateKeyItem, NAME, true );

	return saveFile;
}

void CNrpCompany::_LoadArray( const NrpText& section, const NrpText& fileName, const NrpText& condition )
{
	IniFile rv( fileName );
	CNrpApplication& app = CNrpApplication::Instance();

	for( int i=0; i < GetValue<int>( condition ); i++ )
	{
		NrpText type = rv.Get( section, CreateKeyType(i), NrpText() );
		NrpText rName = rv.Get( section, CreateKeyItem(i), NrpText() );

		INrpConfig* conf = NULL;
		if( type == CNrpGameEngine::ClassName() )
		{
			if( CNrpGameEngine* rr = app.GetGameEngine( rName ) )
				_engines.push_back( rr );
		}
		else if( type == IUser::ClassName() )
		{
			if( IUser* rr = app.GetUser( rName ) )
				_employers.push_back( rr );
		}
		else if( type == CNrpTechnology::ClassName() )
		{
			if( CNrpTechnology* rr = app.GetTechnology( rName ) )
				_technologies.push_back( rr );
		}
		else if( type == CNrpGame::ClassName() )
		{
			if( CNrpGame* rr = app.GetGame( rName ) )
				_games.push_back( rr );
		}
		else if( type == CNrpInvention::ClassName() )
		{
			if( CNrpInvention* rr = app.GetInvention( rName, GetString( NAME ) ) )
				AddInvention( rr );
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

		assert( conf );
		if( conf ) conf->SetValue<PNrpCompany>( PARENTCOMPANY, this );

	}
	
	//SetValue( condition, arrayT.size() );
}

void CNrpCompany::Load( const NrpText& loadFolder )
{
	assert( OpFileSystem::IsExist( loadFolder ) );

	NrpText loadFile = OpFileSystem::CheckEndSlash( loadFolder ) + "company.ini";
	INrpConfig::Load( loadFile );

	PUser ceo = GetValue<PUser>( CEO );
	if( ceo )
	    ceo->SetValue<PNrpCompany>( PARENTCOMPANY, this );

	_LoadArray( SECTION_ENGINES, loadFile, ENGINES_NUMBER );
	_LoadArray( SECTION_USERS, loadFile, USERNUMBER );
	_LoadArray( SECTION_TECHS, loadFile, TECHNUMBER );
	_LoadArray( SECTION_GAMES, loadFile, GAMENUMBER );
	_LoadArray( SECTION_INVENTIONS, loadFile, INVENTIONSNUMBER );
	//_LoadArray( SECTION_PROJECTS, loadFile, PROJECTNUMBER );
	//_LoadArray( SECTION_DEVPROJECTS, loadFile, DEVELOPPROJECTS_NUMBER );

	IniFile rv( loadFile );

	for( int k=0; k < GetValue<int>( OBJECTSINPORTFELLE ); k++ )
	{
		NrpText typeName = rv.Get( SECTION_PORTFELLE, CreateKeyType( k ), NrpText() );
		NrpText prjName = rv.Get( SECTION_PORTFELLE, CreateKeyModule( k ), NrpText() );
		if( typeName == CNrpDevelopGame::ClassName() )
			AddToPortfelle( GetDevelopProject( prjName ) );
	}
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

void CNrpCompany::BeginNewHour( const SYSTEMTIME& time  )
{
	for( size_t cnt=0; cnt < _employers.size(); cnt++ )
		_employers[ cnt ]->BeginNewHour( time );
}

void CNrpCompany::BeginNewDay( const SYSTEMTIME& time )
{
	//уведомление о завершении проекта происходит на следующий день после окончания разработки
	//последнего модуля
	for( u32 i = 0; i < _devProjects.size(); i++ ) 
	{
		if( CNrpDevelopGame* prj = dynamic_cast< CNrpDevelopGame* >( _devProjects[ i ] ) )
		{
			if( prj->IsReady() )
			{
				INrpDevelopProject* project = _devProjects[ i ];
				const PNrpGame game = CreateGame(	(CNrpDevelopGame*)project );
				RemoveDevelopProject( project->GetValue<NrpText>( NAME ) );
				CNrpApplication::Instance().DoLuaFunctionsByType( APP_PROJECT_FINISHED, game );
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
		int salary = _employers[ cnt ]->GetValue<int>( SALARY );	
		workersSalary += salary;

		_employers[ cnt ]->AddValue<int>( BALANCE, salary );
	}
	AddValue<int>( BALANCE, workersSalary );
}

CNrpGame* CNrpCompany::CreateGame( CNrpDevelopGame* devGame )
{
	CNrpGame* ptrGame = new CNrpGame( devGame, this );
	ptrGame->SetValue<SYSTEMTIME>( STARTDATE, CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME ) );
	CNrpApplication::Instance().AddGame( ptrGame );
	CNrpApplication::Instance().UpdateGameRatings( ptrGame, true );
	RemoveFromPortfelle( devGame );

	_games.push_back( ptrGame );
	SetValue<int>( GAMENUMBER, _games.size() );

	return ptrGame;
}

void CNrpCompany::RemoveProject( const NrpText& name )
{
	for( u32 i=0; i < _projects.size(); i++ ) 
		if( _projects[ i ]->Equale( name ) )
			_projects.erase( i );

	SetValue<int>( PROJECTNUMBER, _projects.size() );
}

void CNrpCompany::BeginNewMonth( const SYSTEMTIME& time )
{
	_PaySalaries();
}

void CNrpCompany::AddToPortfelle( INrpConfig* const ptrObject )
{
	_portfelle.push_back( ptrObject );
	SetValue<int>( OBJECTSINPORTFELLE, _portfelle.size() );
}

INrpConfig* CNrpCompany::GetFromPortfelle( size_t index ) const
{
	assert( index < _portfelle.size() );
	return index < _portfelle.size() ? _portfelle[ index ] : NULL;	
}

float CNrpCompany::GetUserModificatorForGame( CNrpGame* game )
{
	return 1;
}

void CNrpCompany::AddDevelopProject( INrpDevelopProject* ptrDevProject )
{
	assert( ptrDevProject != NULL );
	if( ptrDevProject == NULL )
		return;

	if( FindByNameAndIntName< DEVPROJECTS, INrpDevelopProject >( _devProjects, ptrDevProject->GetString( NAME ) ) )
		_devProjects.push_back( ptrDevProject );

	SetValue<int>( DEVELOPPROJECTS_NUMBER, _devProjects.size() );
	ptrDevProject->SetString( COMPANYNAME, GetString( NAME ) );
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

	SetValue<int>( DEVELOPPROJECTS_NUMBER, _devProjects.size() );
	CNrpApplication::Instance().RemoveDevelopProject( name );
}

void CNrpCompany::RemoveFromPortfelle( const INrpConfig* ptrObject )
{
	for( u32 i=0; i < _portfelle.size(); i++ ) 
		if( _portfelle[ i ] == ptrObject )
			_portfelle.erase( i );

	SetValue<int>( OBJECTSINPORTFELLE, _portfelle.size() );
}

void CNrpCompany::AddInvention( CNrpInvention* const inv )
{
	assert( inv != NULL );

	if( inv && FindByNameAndIntName<INVENTIONS, CNrpInvention>( _inventions, inv->GetString( NAME ) ) == NULL )
	{
		_inventions.push_back( inv );
		SetValue<int>( INVENTIONSNUMBER, _inventions.size() );
	}
}

void CNrpCompany::InventionReleased( const CNrpInvention* inv )
{
	NrpText name = static_cast< CNrpInvention >( *inv ).GetString( NAME );
	for( u32 p=0; p < _inventions.size(); p++ )
	{
		if( _inventions[ p ]->Equale( name, GetString( name ) ) )
		{
			//надо что-то делать с похожей технологией... либо развивать следующий уровень,
			//либо прекращать разработки и переводить людей на другой проект с частичным
			//переносом опыта...
			//в любом случае текущие иследования прекращаются...
			CNrpApplication::Instance().InventionCanceled(  _inventions[ p ] );
			CNrpApplication::Instance().DoLuaFunctionsByType( COMPANY_DUPLICATE_INVENTION_FINISHED,  _inventions[ p ] );
			_inventions.erase( p );
			break;
		}			
	}
}

void CNrpCompany::AddTechnology( CNrpTechnology* tech )
{
	if( FindByNameAndIntName< TECHS, CNrpTechnology>( _technologies, tech->GetString( NAME ) )== NULL )
	{
		_technologies.push_back( tech );
		SetValue<int>( TECHNUMBER, _technologies.size() );
	}
}

void CNrpCompany::RemoveInvention( CNrpInvention* inv )
{
	for( u32 i=0; i < _inventions.size(); i++ )
		if( _inventions[ i ] == inv )
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
			IUser* user = _employers[ i ];
			_employers.erase( i );

			user->SetValue<int>( WANTMONEY, static_cast< int >( user->GetValue<int>( SALARY ) * 1.5 ) );
			user->SetValue<PNrpCompany>( PARENTCOMPANY, NULL );
			SetValue<int>( USERNUMBER, _employers.size() );

			break;
		}
}

NrpText CNrpCompany::ClassName()
{
	return CLASS_NRPCOMPANY;
}

}//namespace nrp