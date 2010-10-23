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

#include <errno.h>
#include <assert.h>

namespace nrp
{

void CNrpCompany::_InitialyzeOptions()
{
	CreateValue<int>( BALANCE, 100000 );
	CreateValue<std::string>( NAME, "" );
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

CNrpCompany::CNrpCompany( const std::string& name, IUser* ceo ) : INrpConfig( CLASS_NRPCOMPANY, name)
{
	_InitialyzeOptions();

	SetString( NAME, name );
	SetValue<PUser>( CEO, ceo );
}

CNrpCompany::CNrpCompany( const std::string& fileName ) : INrpConfig( CLASS_NRPCOMPANY, fileName )
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

CNrpGameEngine* CNrpCompany::GetGameEngine( const std::string& name ) const
{
	ENGINE_LIST::const_iterator eIter = _engines.begin();

	for( ; eIter != _engines.end(); eIter++ )
		if( (*eIter)->GetString( NAME ) == name )
			return *eIter;

	return NULL;
}

void CNrpCompany::AddGameEngine( CNrpGameEngine* ptrEng )
{
	if( ptrEng != NULL )
		_engines.push_back( ptrEng );

	SetValue<int>( ENGINES_NUMBER, _engines.size() );
	ptrEng->SetString( COMPANYNAME, this->GetString( NAME ) );
	ptrEng->SetValue<PNrpCompany>( PARENTCOMPANY, this );
}

CNrpTechnology* CNrpCompany::GetTechnology( int index ) const
{
	int tSize = (int)_technologies.size();

	assert( index >= tSize );
	if( index >= tSize )
		return NULL;

	TECH_MAP::const_iterator tech = _technologies.begin();
	for( int k=0; k < index; index++ ) tech++;

	return (*tech).second;
}

CNrpTechnology* CNrpCompany::GetTechnology( const std::string& name ) const
{
	TECH_MAP::const_iterator pIter = _technologies.find( name );
	if( pIter != _technologies.end() )
		return pIter->second;

	return NULL;
}

void CNrpCompany::AddProject( INrpProject* ptrProject )
{
	assert( ptrProject != NULL );

	if( ptrProject == NULL )
		return;

	if( _projects.find( ptrProject->GetString( NAME ) ) == _projects.end() )
		_projects[ ptrProject->GetString( NAME ) ] = ptrProject;

	SetValue<int>( PROJECTNUMBER, _projects.size() );
	ptrProject->SetString( COMPANYNAME, this->GetString( NAME ) );
}

void CNrpCompany::AddUser( IUser* user )
{
	if( !GetUser( user->GetValue<std::string>( NAME ) ) )
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

IUser* CNrpCompany::GetUser( const std::string& name ) const
{
	USER_LIST::const_iterator uIter = _employers.begin();
	for( ; uIter != _employers.end(); uIter++ )
		if( (*uIter)->GetString( NAME ) == name )
			return *uIter;

	return NULL;
}

std::string CNrpCompany::Save( const std::string& saveFolder )
{
	//переданная директория должна существовать
	assert( OpFileSystem::IsExist( saveFolder ) );

	std::string localFolder = OpFileSystem::CheckEndSlash( saveFolder + GetString( NAME ) );

	//если нет директории в которую надо сохранять данные
	OpFileSystem::CreateDirectory( localFolder );

	std::string saveFile = localFolder + "company.ini";
	DeleteFile( saveFile.c_str() );
	INrpConfig::Save( saveFile );

	PROJECT_MAP::iterator pIter = _projects.begin();
	for( int i=0; pIter != _projects.end(); pIter++, i++ )
		IniFile::Write( SECTION_PROJECTS, KEY_PROJECT( i ), pIter->second->GetString( NAME ), saveFile );

	DEVPROJECT_MAP::iterator rIter = _devProjects.begin();
	for( int i=0; rIter != _devProjects.end(); rIter++, i++ )
		IniFile::Write( SECTION_DEVPROJECTS, KEY_PROJECT(i), rIter->second->GetString( NAME ), saveFile );

	ENGINE_LIST::iterator eIter = _engines.begin();
	for( int i=0; eIter != _engines.end(); eIter++, i++ )
		IniFile::Write( SECTION_ENGINES, KEY_ENGINE(i), (*eIter)->GetString( NAME ), saveFile );

	TECH_MAP::iterator tIter = _technologies.begin();
	for( int i=0; tIter != _technologies.end(); tIter++, i++ )
		IniFile::Write( SECTION_TECHS, KEY_TECH(i), tIter->second->GetString( NAME ), saveFile );

	GAME_MAP::iterator gIter = _games.begin();
	for( int i=0; gIter != _games.end(); gIter++, i++ )
		IniFile::Write( SECTION_GAMES, KEY_GAME(i), gIter->second->GetString( NAME ), saveFile );

	USER_LIST::iterator uIter = _employers.begin();
	for( int i=0; uIter != _employers.end(); uIter++, i++ )
		IniFile::Write( SECTION_USERS, KEY_USER(i), (*uIter)->GetString( NAME ), saveFile );

	OBJECT_LIST::iterator oIter = _portfelle.begin();
	for( int i=0; oIter != _portfelle.end(); oIter++, i++ )
	{
		IniFile::Write( SECTION_PORTFELLE, KEY_TYPE( i ), (*oIter)->ObjectTypeName(), saveFile ); 
		IniFile::Write( SECTION_PORTFELLE, KEY_MODULE( i ), (*oIter)->GetString( NAME ), saveFile );
	}

	INVENTION_LIST::iterator invIter = _inventions.begin();
	for( int i=0; invIter != _inventions.end(); invIter++, i++ )
		IniFile::Write( SECTION_INVENTIONS, KEY_INVENTION( i ), (*invIter)->GetString( NAME ), saveFile );

	return saveFile;
}

void CNrpCompany::Load( const std::string& loadFolder )
{
	assert( OpFileSystem::IsExist( loadFolder ) );

	std::string loadFile = OpFileSystem::CheckEndSlash( loadFolder ) + "company.ini";
	INrpConfig::Load( loadFile );

	PUser ceo = GetValue<PUser>( CEO );
	if( ceo )
	    ceo->SetValue<PNrpCompany>( PARENTCOMPANY, this );

	for( int i=0; i < GetValue<int>( ENGINES_NUMBER ); i++ )
	{
		std::string engineName = IniFile::Read( SECTION_ENGINES, KEY_ENGINE(i), std::string(""), loadFile );
		CNrpGameEngine* ge = CNrpApplication::Instance().GetGameEngine( engineName );
		if( ge )
		{
			_engines.push_back( ge );
			ge->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		}
	}

	for( int i=0; i < GetValue<int>( USERNUMBER ); i++ )
	{
		std::string name = IniFile::Read( SECTION_USERS, KEY_USER(i), std::string(""), loadFile );
		PUser user = CNrpApplication::Instance().GetUser( name );
		assert( user != NULL );
		if( user )
		{
			_employers.push_back( user );
			user->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		}
	}

	for( int i=0; i < GetValue<int>( TECHNUMBER ); i++ )
	{
		std::string techName = IniFile::Read( SECTION_TECHS, KEY_TECH(i), std::string(""), loadFile );
		CNrpTechnology* tech = CNrpApplication::Instance().GetTechnology( techName );
		assert( tech != NULL );
		if( tech )
		{
			_technologies[ techName ] = tech;
			tech->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		}
	}

	for( int i=0; i < GetValue<int>( GAMENUMBER ); i++ )
	{
		std::string gameName = IniFile::Read( SECTION_GAMES, KEY_GAME(i), std::string(""), loadFile );
		CNrpGame* game = CNrpApplication::Instance().GetGame( gameName );
		if( game )
		{
			_games[ gameName ] = game;
			game->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		}
	}

	for( int i=0; i < GetValue<int>( PROJECTNUMBER ); i++ )
	{
		std::string prjName = IniFile::Read( SECTION_PROJECTS, KEY_PROJECT(i), std::string(""), loadFile );
		CNrpGameProject* prj = dynamic_cast< CNrpGameProject* >( CNrpApplication::Instance().GetProject( prjName ) );
		if( prj )
		{
			_projects[ prjName ] = prj;
			prj->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		}
	}

	for( int i=0; i < GetValue<int>( DEVELOPPROJECTS_NUMBER ); i++ )
	{
		std::string prjName = IniFile::Read( SECTION_DEVPROJECTS, KEY_PROJECT(i), std::string(""), loadFile );
		INrpDevelopProject* prj = CNrpApplication::Instance().GetDevelopProject( prjName );
		if( prj )
		{
			_devProjects[ prjName ] = prj;
			prj->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		}
	}

	for( int k=0; k < GetValue<int>( OBJECTSINPORTFELLE ); k++ )
	{
		std::string typeName = IniFile::Read( SECTION_PORTFELLE, KEY_TYPE( k ), std::string(""), loadFile );
		std::string prjName = IniFile::Read( SECTION_PORTFELLE, KEY_MODULE( k ), std::string( "" ), loadFile );
		if( typeName == CLASS_DEVELOPGAME )
			AddToPortfelle( GetDevelopProject( prjName ) );
	}

	for( int k=0; k < GetValue<int>( INVENTIONSNUMBER ); k++ )
	{
		std::string name = IniFile::Read( SECTION_INVENTIONS, KEY_INVENTION( k ), std::string(""), loadFile );
		CNrpInvention* invention = CNrpApplication::Instance().GetInvention( name, GetString( NAME ) );
		if( invention )
		{
			AddInvention( invention );
			invention->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		}
	}
}

CNrpGame* CNrpCompany::GetGame( const std::string& gameName ) const 
{
	GAME_MAP::const_iterator gIter = _games.find( gameName );
	return gIter != _games.end() ? gIter->second : NULL;
}

CNrpGame* CNrpCompany::GetGame( size_t index ) const
{
	assert( index < _games.size() );
	if( index < _games.size() )
	{
		GAME_MAP::const_iterator gIter = _games.begin();
		for( size_t k=0; k != index; k++ ) gIter++;
		return gIter->second;
	}

	return NULL;
}

INrpProject* CNrpCompany::GetProject( const std::string& name ) const
{
	PROJECT_MAP::const_iterator pIter = _projects.find( name );
	return pIter != _projects.end() ? pIter->second : NULL;
}

INrpProject* CNrpCompany::GetProject( size_t index ) const
{
	assert( index < _projects.size() );
	if( index < _projects.size() )
	{
		PROJECT_MAP::const_iterator pIter = _projects.begin();
		for( size_t k=0; k != index; k++ ) pIter++;
		return pIter->second;
	}

	return NULL;
}

void CNrpCompany::BeginNewHour( const SYSTEMTIME& time  )
{
	for( size_t cnt=0; cnt < _employers.size(); cnt++ )
		_employers[ cnt ]->BeginNewHour( time );
}

void CNrpCompany::BeginNewDay( const SYSTEMTIME& time )
{
	for( DEVPROJECT_MAP::iterator pIter = _devProjects.begin(); 
		 pIter != _devProjects.end(); 
		 pIter++ )
	{
		if( pIter->second->ClassName() == CLASS_DEVELOPGAME && pIter->second->GetValue<bool>( PROJECTREADY ) )
		{
			INrpDevelopProject* project = pIter->second;
			const PNrpGame game = CreateGame(	(CNrpDevelopGame*)project );
			RemoveDevelopProject( project->GetValue<std::string>( NAME ) );
			DoLuaFunctionsByType( COMPANY_READY_PROJECT, game );
			break;
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

	_games[ ptrGame->GetValue<std::string>( NAME ) ] = ptrGame;
	SetValue<int>( GAMENUMBER, _games.size() );

	return ptrGame;
}

void CNrpCompany::RemoveProject( const std::string& name )
{
	PROJECT_MAP::iterator pIter = _projects.find( name );
	if( pIter != _projects.end() ) 
		_projects.erase( pIter );

	SetValue<int>( PROJECTNUMBER, _projects.size() );
}

void CNrpCompany::_UpdateGameProjectState()
{
	PROJECT_MAP::iterator pIter = _projects.begin();
	for( ; pIter != _projects.end(); pIter++ ) 
	{
		if( CNrpGameProject* prj = dynamic_cast< CNrpGameProject* >( pIter->second ) )
			prj->UpdateDevelopmentMoney();
	}
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

	if( _devProjects.find( ptrDevProject->GetString( NAME ) ) == _devProjects.end() )
		_devProjects[ ptrDevProject->GetString( NAME ) ] = ptrDevProject;

	SetValue<int>( DEVELOPPROJECTS_NUMBER, _devProjects.size() );
	ptrDevProject->SetString( COMPANYNAME, GetString( NAME ) );
}

INrpDevelopProject* CNrpCompany::GetDevelopProject( const std::string& name ) const
{
	DEVPROJECT_MAP::const_iterator pIter = _devProjects.find( name );
	if( pIter != _devProjects.end() )
		return pIter->second;

	return NULL;
}

INrpDevelopProject* CNrpCompany::GetDevelopProject( size_t index ) const
{
	assert( index < _devProjects.size() );
	if( index < _devProjects.size() )
	{
		DEVPROJECT_MAP::const_iterator pIter = _devProjects.begin();
		for( size_t k=0; k != index; k++ ) pIter++;
		return pIter->second;
	}

	return NULL;
}

CNrpInvention* CNrpCompany::GetInvention( int index )
{
	assert( index >= 0 && index < (int)_inventions.size() );
	return index < (int)_inventions.size() ? _inventions[ index ] : NULL;	
}

void CNrpCompany::RemoveDevelopProject( const std::string& name )
{
	DEVPROJECT_MAP::iterator pIter = _devProjects.find( name );
	if( pIter != _devProjects.end() ) 
		_devProjects.erase( pIter );

	SetValue<int>( DEVELOPPROJECTS_NUMBER, _devProjects.size() );
	CNrpApplication::Instance().RemoveDevelopProject( name );
}

void CNrpCompany::RemoveFromPortfelle( const INrpConfig* ptrObject )
{
	OBJECT_LIST::iterator iter = _portfelle.begin();
	for( ; iter != _portfelle.end(); iter++ )
	if( *iter == ptrObject )
	{
		_portfelle.erase( iter );
		break;
	}
}

void CNrpCompany::AddInvention( CNrpInvention* const inv )
{
	assert( inv != NULL );

	_inventions.push_back( inv );
	SetValue<int>( INVENTIONSNUMBER, _inventions.size() );
}

void CNrpCompany::InventionReleased( const CNrpInvention* inv )
{
	INVENTION_LIST::iterator pIter = _inventions.begin();
	std::string name = inv->GetValue<std::string>( NAME );
	for( ; pIter != _inventions.end(); pIter++ )
	{
		if( (*pIter)->GetValue<std::string>( NAME ) == name )
		{
			//надо что-то делать с похожей технологией... либо развивать следующий уровень,
			//либо прекращать разработки и переводить людей на другой проект с частичным
			//переносом опыта...
			//в любом случае текущие иследования прекращаются...
			CNrpApplication::Instance().InventionCanceled( *pIter );
			DoLuaFunctionsByType( COMPANY_DUPLICATE_INVENTION_FINISHED, *pIter );
			_inventions.erase( pIter );
			break;
		}			
	}
}

void CNrpCompany::AddTechnology( CNrpTechnology* tech )
{
	_technologies[ tech->GetValue<std::string>( NAME ) ] = tech;
}

void CNrpCompany::RemoveInvention( CNrpInvention* inv )
{
	INVENTION_LIST::iterator pIter = _inventions.begin();
	for( ; pIter != _inventions.end(); pIter++ )
		if( *pIter == inv )
		{
			_inventions.erase( pIter );
			break;
		}

	USER_LIST::iterator uIter = _employers.begin();
	for( ; uIter != _employers.end(); uIter++ )
		(*uIter)->RemoveWork( inv );
}

CNrpInvention* CNrpCompany::GetInvention( const std::string& name )
{
	INVENTION_LIST::iterator pIter = _inventions.begin();
	for( ; pIter != _inventions.end(); pIter++ )
		if( (*pIter)->GetValue<std::string>( NAME ) == name )
			return *pIter;

	return NULL;
}

void CNrpCompany::RemoveUser( const std::string& name )
{
	USER_LIST::iterator uIter = _employers.begin();
	for( ; uIter != _employers.end(); uIter++ )
		if( (*uIter)->GetValue<std::string>( NAME ) == name )
		{
			IUser* user = *uIter;
			_employers.erase( uIter );

			user->SetValue<int>( WANTMONEY, static_cast< int >( user->GetValue<int>( SALARY ) * 1.5 ) );
			user->SetValue<PNrpCompany>( PARENTCOMPANY, NULL );
			SetValue<int>( USERNUMBER, _employers.size() );

			break;
		}
}
}//namespace nrp