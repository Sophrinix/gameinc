#include "StdAfx.h"
#include "NrpCompany.h"
#include "IUser.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpTechnology.h"
#include "NrpGame.h"
#include "NrpApplication.h"
#include "NrpDevelopGame.h"

#include <io.h>
#include <errno.h>
#include <assert.h>

namespace nrp
{

CNrpCompany::CNrpCompany(std::string name, IUser* ceo ) : INrpConfig( CLASS_NRPCOMPANY, name)
{
	CreateValue<int>( BALANCE, 100000 );
	CreateValue<std::string>( NAME, name );
	CreateValue<PUser>( CEO, ceo );
	CreateValue<int>( ENGINES_NUMBER, 0 );
	CreateValue<int>( TECHNUMBER, 0 );
	CreateValue<int>( USERNUMBER, 0 );
	CreateValue<int>( PROJECTNUMBER, 0 );
	CreateValue<int>( GAMENUMBER, 0 );
	CreateValue<int>( OBJECTSINPORTFELLE, 0 );
	CreateValue<int>( DEVELOPPROJECTS_NUMBER, 0 );
	CreateValue<float>( FAMOUS, 0.1f );
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

	SetValue<int>( ENGINES_NUMBER, engines_.size() );
	ptrEng->SetValue<std::string>( COMPANYNAME, this->GetValue<std::string>( NAME ) );
	ptrEng->SetValue<PNrpCompany>( PARENTCOMPANY, this );
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

void CNrpCompany::AddProject( INrpProject* ptrProject )
{
	if( projects_.find( ptrProject->GetValue<std::string>( NAME ) ) == projects_.end() )
		projects_[ ptrProject->GetValue<std::string>( NAME ) ] = ptrProject;

	SetValue<int>( PROJECTNUMBER, projects_.size() );
	ptrProject->SetValue<std::string>( COMPANYNAME, this->GetValue<std::string>( NAME ) );
}

void CNrpCompany::AddUser( IUser* user )
{
	if( !GetUser( user->GetValueA<std::string>( NAME ) ) )
	{
		employers_.push_back( user );
		user->SetValue<int>( SALARY, user->GetValueA<int>( WANTMONEY ) );
		user->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		SetValue<int>( USERNUMBER, employers_.size() );
	}
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

void CNrpCompany::Save( const std::string& saveFolder )
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

	PROJECT_MAP::iterator pIter = projects_.begin();
	for( int i=0; pIter != projects_.end(); pIter++, i++ )
		IniFile::Write( "projects", "project_" + IntToStr(i), pIter->second->GetValue<std::string>( NAME ), saveFile );

	PROJECT_MAP::iterator rIter = devProjects_.begin();
	for( int i=0; rIter != devProjects_.end(); rIter++, i++ )
		IniFile::Write( "devProjects", "project_" + IntToStr(i), rIter->second->GetValue<std::string>( NAME ), saveFile );

	ENGINE_LIST::iterator eIter = engines_.begin();
	for( int i=0; eIter != engines_.end(); eIter++, i++ )
		IniFile::Write( "engines", "engine_" + IntToStr(i), (*eIter)->GetValue<std::string>( NAME ), saveFile );

	TECH_MAP::iterator tIter = technologies_.begin();
	for( int i=0; tIter != technologies_.end(); tIter++, i++ )
		IniFile::Write( "techs", "tech_" + IntToStr(i), tIter->second->GetValue<std::string>( NAME ), saveFile );

	GAME_MAP::iterator gIter = games_.begin();
	for( int i=0; gIter != games_.end(); gIter++, i++ )
		IniFile::Write( "games", "game_" + IntToStr(i), gIter->second->GetValue<std::string>( NAME ), saveFile );

	USER_LIST::iterator uIter = employers_.begin();
	for( int i=0; uIter != employers_.end(); uIter++, i++ )
		IniFile::Write( "users", "user_" + IntToStr(i), (*uIter)->GetValueA<std::string>( NAME ), saveFile );

	OBJECT_LIST::iterator oIter = portfelle_.begin();
	for( int i=0; oIter != portfelle_.end(); oIter++, i++ )
	{
		std::string typeName = (*oIter)->ClassName();
		IniFile::Write( "portfelle", "object_" + IntToStr( i ), typeName + ":" + (*oIter)->GetValue<std::string>( NAME ), saveFile );
	}
}

void CNrpCompany::Load( const std::string& loadFolder )
{
	std::string loadFile = loadFolder + "/company.ini";
	INrpConfig::Load( PROPERTIES, loadFile );

	PUser ceo = GetValue<PUser>( CEO );
	if( ceo )
	    ceo->SetValue<PNrpCompany>( PARENTCOMPANY, this );

	for( int i=0; i < GetValue<int>( ENGINES_NUMBER ); i++ )
	{
		std::string engineName = IniFile::Read( "engines", "engine_" + IntToStr(i), std::string(""), loadFile );
		CNrpGameEngine* ge = CNrpApplication::Instance().GetGameEngine( engineName );
		if( ge )
		{
			engines_.push_back( ge );
			ge->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		}
	}

	for( int i=0; i < GetValue<int>( USERNUMBER ); i++ )
	{
		std::string name = IniFile::Read( "users", "user_" + IntToStr(i), std::string(""), loadFile );
		PUser user = CNrpApplication::Instance().GetUser( name );
		assert( user != NULL );
		if( user )
		{
			employers_.push_back( user );
			user->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		}
	}

	for( int i=0; i < GetValue<int>( TECHNUMBER ); i++ )
	{
		std::string techName = IniFile::Read( "techs", "tech_" + IntToStr(i), std::string(""), loadFile );
		CNrpTechnology* tech = CNrpApplication::Instance().GetTechnology( techName );
		assert( tech != NULL );
		if( tech )
		{
			technologies_[ techName ] = tech;
			tech->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		}
	}

	for( int i=0; i < GetValue<int>( GAMENUMBER ); i++ )
	{
		std::string gameName = IniFile::Read( "games", "game_" + IntToStr(i), std::string(""), loadFile );
		CNrpGame* game = CNrpApplication::Instance().GetGame( gameName );
		if( game )
		{
			games_[ gameName ] = game;
			game->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		}
	}

	for( int i=0; i < GetValue<int>( PROJECTNUMBER ); i++ )
	{
		std::string prjName = IniFile::Read( "projects", "project_" + IntToStr(i), std::string(""), loadFile );
		CNrpGameProject* prj = dynamic_cast< CNrpGameProject* >( CNrpApplication::Instance().GetProject( prjName ) );
		if( prj )
		{
			projects_[ prjName ] = prj;
			prj->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		}
	}

	for( int i=0; i < GetValue<int>( DEVELOPPROJECTS_NUMBER ); i++ )
	{
		std::string prjName = IniFile::Read( "devProjects", "project_" + IntToStr(i), std::string(""), loadFile );
		INrpProject* prj = CNrpApplication::Instance().GetDevelopProject( prjName );
		if( prj )
		{
			devProjects_[ prjName ] = prj;
			prj->SetValue<PNrpCompany>( PARENTCOMPANY, this );
		}
	}

	for( int k=0; k < GetValue<int>( OBJECTSINPORTFELLE ); k++ )
	{
		std::string str = IniFile::Read( "portfelle", "object_"+IntToStr( k ), std::string(""), loadFile );
		std::string typeName = str.substr( 0, str.find(':') );
		std::string prjName = str.substr( str.find( ':' ) + 1, str.find( '=' ) );
		prjName = prjName.substr( prjName.find( '=' )+1, 0xff );
		if( typeName == CLASS_DEVELOPGAME )
			AddToPortfelle( GetDevelopProject( prjName ) );
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
	for( PROJECT_MAP::iterator pIter = devProjects_.begin(); 
		 pIter != devProjects_.end(); 
		 pIter++ )
	{
		if( pIter->second->ClassName() == CLASS_DEVELOPGAME && pIter->second->GetValue<bool>( PROJECTREADY ) )
		{
			INrpProject* project = pIter->second;
			PNrpGame game = CreateGame(	(CNrpDevelopGame*)project );
			RemoveDevelopProject( project->GetValue<std::string>( NAME ) );
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

CNrpGame* CNrpCompany::CreateGame( CNrpDevelopGame* devGame )
{
	CNrpGame* ptrGame = new CNrpGame( devGame, this );
	ptrGame->SetValue<SYSTEMTIME>( STARTDATE, CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME ) );
	CNrpApplication::Instance().AddGame( ptrGame );
	CNrpApplication::Instance().UpdateGameRatings( ptrGame, true );
	RemoveFromPortfelle( devGame );

	games_[ ptrGame->GetValue<std::string>( NAME ) ] = ptrGame;
	SetValue<int>( GAMENUMBER, games_.size() );

	return ptrGame;
}

void CNrpCompany::RemoveProject( std::string name )
{
	PROJECT_MAP::iterator pIter = projects_.find( name );
	if( pIter != projects_.end() ) 
		projects_.erase( pIter );

	SetValue<int>( PROJECTNUMBER, projects_.size() );
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

float CNrpCompany::GetUserModificatorForGame( CNrpGame* game )
{
	return 1;
}

void CNrpCompany::AddDevelopProject( INrpProject* ptrDevProject )
{
	if( devProjects_.find( ptrDevProject->GetValue<std::string>( NAME ) ) == devProjects_.end() )
		devProjects_[ ptrDevProject->GetValue<std::string>( NAME ) ] = ptrDevProject;

	SetValue<int>( DEVELOPPROJECTS_NUMBER, devProjects_.size() );
	ptrDevProject->SetValue<std::string>( COMPANYNAME, GetValue<std::string>( NAME ) );
}

INrpProject* CNrpCompany::GetDevelopProject( const std::string name )
{
	PROJECT_MAP::iterator pIter = devProjects_.find( name );
	if( pIter != devProjects_.end() )
		return pIter->second;

	return NULL;
}

INrpProject* CNrpCompany::GetDevelopProject( size_t index )
{
	if( index < devProjects_.size() )
	{
		PROJECT_MAP::iterator pIter = devProjects_.begin();
		for( size_t k=0; k != index; k++ ) pIter++;
		return pIter->second;
	}

	return NULL;
}

void CNrpCompany::RemoveDevelopProject( std::string name )
{
	PROJECT_MAP::iterator pIter = devProjects_.find( name );
	if( pIter != devProjects_.end() ) 
		devProjects_.erase( pIter );

	SetValue<int>( DEVELOPPROJECTS_NUMBER, devProjects_.size() );
	CNrpApplication::Instance().RemoveDevelopProject( name );
}

void CNrpCompany::RemoveFromPortfelle( INrpConfig* ptrObject )
{
	OBJECT_LIST::iterator iter = portfelle_.begin();
	for( ; iter != portfelle_.end(); iter++ )
	if( *iter == ptrObject )
	{
		portfelle_.erase( iter );
		break;
	}
}

void CNrpCompany::StartInvention( CNrpInvention* inv )
{
	inventions_.push_back( inv );
}
}//namespace nrp