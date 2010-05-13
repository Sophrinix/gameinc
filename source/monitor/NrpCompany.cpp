#include "StdAfx.h"
#include "NrpCompany.h"
#include "IUser.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpTechnology.h"
#include "NrpGame.h"
#include "NrpApplication.h"

#include <io.h>
#include <errno.h>
#include <assert.h>

namespace nrp
{

CNrpCompany::CNrpCompany( const char* name ) : INrpConfig( "CNrpCompany", name)
{
	CreateValue<int>( BALANCE, 100000 );
	CreateValue<std::string>( NAME, name );
	CreateValue<PUser>( CEO, NULL );
	CreateValue<int>( ENGINES_NUMBER, 0 );
	CreateValue<int>( TECHNUMBER, 0 );
	CreateValue<int>( USERNUMBER, 0 );
	CreateValue<int>( PROJECTNUMBER, 0 );
	CreateValue<int>( GAMENUMBER, 0 );
	CreateValue<int>( OBJECTSINPORTFELLE, 0 );
}

CNrpCompany::~CNrpCompany(void)
{
}

CNrpGameEngine* CNrpCompany::GetGameEngine( int index )
{
	return index < (int)engines_.size() ? engines_[ index ] : NULL;
}

CNrpGameEngine* CNrpCompany::GetGameEngine( std::string name )
{
	ENGINE_LIST::iterator eIter = engines_.begin();

	for( ; eIter != engines_.end(); eIter++ )
		if( (*eIter)->GetValue<std::string>( NAME ) == name )
			return *eIter;

	return NULL;
}

void CNrpCompany::AddGameEngine( CNrpGameEngine* ptrEng )
{
	if( ptrEng != NULL )
		engines_.push_back( ptrEng );

	int k = engines_.size();
	SetValue<int>( ENGINES_NUMBER, k );
	ptrEng->SetValue<std::string>( COMPANYNAME, this->GetValue<std::string>( NAME ) );
}

CNrpTechnology* CNrpCompany::GetTechnology( int index )
{
	int tSize = (int)technologies_.size();
	if( index >= tSize )
		return NULL;

	TECH_MAP::iterator tech = technologies_.begin();
	for( int k=0; k < index; index++ ) tech++;

	return (*tech).second;
}

CNrpTechnology* CNrpCompany::GetTechnology( std::string name )
{
	TECH_MAP::iterator pIter = technologies_.find( name );
	if( pIter != technologies_.end() )
		return pIter->second;

	return NULL;
}

CNrpGameProject* CNrpCompany::AddGameProject( CNrpGameProject* ptrProject )
{
	if( projects_.find( ptrProject->GetValue<std::string>( NAME ) ) == projects_.end() )
		projects_[ ptrProject->GetValue<std::string>( NAME ) ] = ptrProject;

	SetValue<int>( PROJECTNUMBER, projects_.size() );
	ptrProject->SetValue<std::string>( COMPANYNAME, this->GetValue<std::string>( NAME ) );
	return ptrProject;
}

void CNrpCompany::AddUser( IUser* user )
{
	employers_.push_back( user );
	user->SetValue<int>( SALARY, user->GetValueA<int>( WANTMONEY ) );
	SetValue<int>( USERNUMBER, employers_.size() );
}

IUser* CNrpCompany::GetUser( int index )
{
	return index < (int)employers_.size() ? employers_[ index ] : NULL;
}

IUser* CNrpCompany::GetUser( std::string name )
{
	USER_LIST::iterator uIter = employers_.begin();
	for( ; uIter != employers_.end(); uIter++ )
		if( (*uIter)->GetValueA<std::string>( NAME ) == name )
			return *uIter;

	return NULL;
}
void CNrpCompany::Save( std::string saveFolder )
{
	//если нет родительской директории
	if( _access( saveFolder.c_str(), 0 ) == -1 )			
		CreateDirectory( saveFolder.c_str(), NULL );

	std::string localFolder = saveFolder + GetValue<std::string>( NAME ) + "/";

	//если нет директории в которую надо сохранять данные
	if( _access( localFolder.c_str(), 0 ) == -1 )
		CreateDirectory( localFolder.c_str(), NULL );

	std::string saveFile = localFolder + "company.ini";
	DeleteFile( saveFile.c_str() );
	INrpConfig::Save( PROPERTIES, saveFile );

	IniFile::Write( PROPERTIES, "ceo", GetValue<PUser>( CEO )->GetValueA<std::string>( NAME ), saveFile );

	PROJECT_MAP::iterator pIter = projects_.begin();
	for( int i=0; pIter != projects_.end(); pIter++, i++ )
	{
		CNrpGameProject* prj = (CNrpGameProject*)pIter->second;
		prj->Save( localFolder + "projects/" );
		IniFile::Write( "projects", "project_" + IntToStr(i), prj->GetValue<std::string>( NAME ), saveFile );
	}

	ENGINE_LIST::iterator eIter = engines_.begin();
	for( int i=0; eIter != engines_.end(); eIter++, i++ )
	{
		(*eIter)->Save( localFolder + "engines/" );
		IniFile::Write( "engines", "engine_" + IntToStr(i), (*eIter)->GetValue<std::string>( NAME ), saveFile );
	}

	TECH_MAP::iterator tIter = technologies_.begin();
	for( int i=0; tIter != technologies_.end(); tIter++, i++ )
	{
		CNrpTechnology* tech = (CNrpTechnology*)tIter->second;
		tech->Save( localFolder + "techs/" );
		IniFile::Write( "techs", "tech_" + IntToStr(i), tech->GetValue<std::string>( NAME ), saveFile );
	}

	GAME_MAP::iterator gIter = games_.begin();
	for( int i=0; gIter != games_.end(); gIter++, i++ )
	{
		CNrpGame* game = (CNrpGame*)gIter->second;
		game->Save( localFolder + "games/" );
		IniFile::Write( "games", "game_" + IntToStr(i), game->GetValue<std::string>( NAME ), saveFile );
	}

	USER_LIST::iterator uIter = employers_.begin();
	for( int i=0; uIter != employers_.end(); uIter++, i++ )
	{
		(*uIter)->Save( localFolder + "users/" );
		IniFile::Write( "users", "user_" + IntToStr(i), (*uIter)->ClassName() + ":" + (*uIter)->GetValueA<std::string>( NAME ), saveFile );
	}
}

void CNrpCompany::Load( std::string loadFolder )
{
	std::string loadFile = loadFolder + "/" + "company.ini";
	INrpConfig::Load( PROPERTIES, loadFile );

	std::string ceoName = IniFile::Read( PROPERTIES, "ceo", std::string(""), loadFile );
	PUser usert = CNrpApplication::Instance().GetUser( ceoName );
	assert( usert != NULL );
	SetValue<PUser>( CEO, usert );
	usert->SetValue<std::string>( COMPANYNAME, GetValue<std::string>( NAME ) );

	for( int i=0; i < GetValue<int>( USERNUMBER ); i++ )
	{
		std::string name = IniFile::Read( "users", "user_" + IntToStr(i), std::string(""), loadFile );
		std::string className = name.substr( 0, name.find( ':' ) );
		name = name.substr( name.find(':') + 1, 0xff );
		IUser* user = new IUser( className.c_str(), name.c_str(), this );
		user->Load( loadFolder + "users/" + name + ".ini" );
		employers_.push_back( user );
	}

	for( int i=0; i < GetValue<int>( ENGINES_NUMBER ); i++ )
	{
		std::string engineName = IniFile::Read( "engines", "engine_" + IntToStr(i), std::string(""), loadFile );
		CNrpGameEngine* ge = new CNrpGameEngine( "" );
		ge->Load( loadFolder + "engines/" + engineName + "/" );
		ge->SetValue<std::string>( COMPANYNAME, GetValue<std::string>( NAME ) );
		engines_.push_back( ge );
	}

	for( int i=0; i < GetValue<int>( TECHNUMBER ); i++ )
	{
		std::string techName = IniFile::Read( "techs", "tech_" + IntToStr(i), std::string(""), loadFile );
		CNrpTechnology* tech = new CNrpTechnology( PROJECT_TYPE(0), this );
		tech->Load( loadFolder + "techs/" + techName + ".ini" );
		technologies_[ techName ] = tech;
	}

	for( int i=0; i < GetValue<int>( GAMENUMBER ); i++ )
	{
		std::string gameName = IniFile::Read( "games", "game_" + IntToStr(i), std::string(""), loadFile );
		CNrpGame* game = new CNrpGame( gameName );
		game->Load( loadFolder + "games/" );
		game->SetValue<std::string>( COMPANYNAME, this->GetValue<std::string>( NAME ) );
		games_[ gameName ] = game;
	}

	for( int i=0; i < GetValue<int>( PROJECTNUMBER ); i++ )
	{
		std::string prjName = IniFile::Read( "projects", "project_" + IntToStr(i), std::string(""), loadFile );
		CNrpGameProject* prj = new CNrpGameProject( "", this );
		prj->Load( loadFolder + "projects/" + prjName );
		projects_[ prjName ] = prj;
	}
}

CNrpGame* CNrpCompany::GetGame( std::string gameName )
{
	GAME_MAP::iterator gIter = games_.find( gameName );
	return gIter != games_.end() ? gIter->second : NULL;
}

CNrpGame* CNrpCompany::GetGame( size_t index )
{
	if( index < games_.size() )
	{
		GAME_MAP::iterator gIter = games_.begin();
		for( size_t k=0; k != index; k++ ) gIter++;
		return gIter->second;
	}

	return NULL;
}

INrpProject* CNrpCompany::GetProject( std::string name )
{
	PROJECT_MAP::iterator pIter = projects_.find( name );
	return pIter != projects_.end() ? pIter->second : NULL;
}

INrpProject* CNrpCompany::GetProject( size_t index )
{
	if( index < projects_.size() )
	{
		PROJECT_MAP::iterator pIter = projects_.begin();
		for( size_t k=0; k != index; k++ ) pIter++;
		return pIter->second;
	}

	return NULL;
}

void CNrpCompany::BeginNewHour( const SYSTEMTIME& time  )
{
	for( size_t cnt=0; cnt < employers_.size(); cnt++ )
		employers_[ cnt ]->BeginNewHour( time );
}

void CNrpCompany::BeginNewDay( const SYSTEMTIME& time )
{
	PROJECT_MAP::iterator pIter = projects_.begin();
	for( ; pIter != projects_.end(); pIter++ )
	{
		if( pIter->second->GetType() == "CNrpGameProject" && dynamic_cast<CNrpGameProject*>( pIter->second )->IsReady() )
		{
			CNrpGameProject* project = dynamic_cast<CNrpGameProject*>( pIter->second );
			PNrpGame game = CreateGame( project );
			RemoveGameProject( project );
			delete project;
			DoLuaFunctionsByType( COMPANY_READY_PROJECT, game );
			break;
		}
	}

	for( size_t cnt=0; cnt < employers_.size(); cnt++ )
		employers_[ cnt ]->BeginNewDay( time );
}

void CNrpCompany::PaySalaries_()
{
	int balance = GetValue<int>( BALANCE );
	for( size_t cnt=0; cnt < employers_.size(); cnt++ )
	{
		int salary = employers_[ cnt ]->GetValueA<int>( SALARY );	
		int userBalance = employers_[ cnt ]->GetValueA<int>( BALANCE );
		balance -= salary;

		employers_[ cnt ]->SetValue<int>( BALANCE, userBalance + salary );
	}
	SetValue<int>( BALANCE, balance );
}

CNrpGame* CNrpCompany::CreateGame( CNrpGameProject* ptrProject )
{
	CNrpGame* ptrGame =  new CNrpGame( ptrProject, this );
	ptrGame->SetValue<SYSTEMTIME>( STARTDATE, CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME ) );
	CNrpApplication::Instance().UpdateGameRatings( ptrGame, true );

	games_[ ptrProject->GetValue<std::string>( NAME ) ] = ptrGame;
	SetValue<int>( GAMENUMBER, games_.size() );

	return ptrGame;
}

void CNrpCompany::RemoveGameProject( CNrpGameProject* ptrProject )
{
	PROJECT_MAP::iterator pIter = projects_.find( ptrProject->GetValue<std::string>( NAME ) );
	if( pIter != projects_.end() ) 
		projects_.erase( pIter );
}

void CNrpCompany::UpdateGameProjectState_()
{
	PROJECT_MAP::iterator pIter = projects_.begin();
	for( ; pIter != projects_.end(); pIter++ ) 
	{
		if( pIter->second->GetType() == "CNrpGameProject" )
			dynamic_cast< CNrpGameProject* >( pIter->second )->UpdateDevelopmentMoney();
	}
}

void CNrpCompany::BeginNewMonth( const SYSTEMTIME& time )
{
	PaySalaries_();
}

void CNrpCompany::AddToPortfelle( INrpConfig* ptrObject )
{
	portfelle_.push_back( ptrObject );
	SetValue<int>( OBJECTSINPORTFELLE, portfelle_.size() );
}

INrpConfig* CNrpCompany::GetFromPortfelle( int index )
{
	return index < (int)portfelle_.size() ? portfelle_[ index ] : NULL;	
}
}//namespace nrp