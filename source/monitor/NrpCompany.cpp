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
}

CNrpCompany::~CNrpCompany(void)
{
}

CNrpGameEngine* CNrpCompany::GetGameEngine( int index )
{
	return index < (int)engines_.size() ? engines_[ index ] : NULL;
}

void CNrpCompany::AddGameEngine( CNrpGameEngine* ptrEng )
{
	if( ptrEng != NULL )
		engines_.push_back( ptrEng );

	int k = engines_.size();
	SetValue<int>( ENGINES_NUMBER, k );
}

CNrpTechnology* CNrpCompany::GetTech( int index )
{
	int tSize = (int)technologies_.size();
	if( index >= tSize )
		return NULL;

	TECH_MAP::iterator tech = technologies_.begin();
	for( int k=0; k < index; index++ ) tech++;

	return (*tech).second;
}

CNrpGameProject* CNrpCompany::AddGameProject( CNrpGameProject* ptrProject )
{
	if( projects_.find( ptrProject->GetValue<std::string>( NAME ) ) == projects_.end() )
	{
		projects_[ ptrProject->GetValue<std::string>( NAME ) ] = ptrProject;
		return ptrProject;
	}

	SetValue<int>( PROJECTNUMBER, projects_.size() );
	return NULL;
}

void CNrpCompany::AddUser( IUser* user )
{
	employers_.push_back( user );

	SetValue<int>( USERNUMBER, employers_.size() );
}

IUser* CNrpCompany::GetUser( int index )
{
	return index < (int)employers_.size() ? employers_[ index ] : NULL;
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

	IniFile::Write( PROPERTIES, "ceo", GetValue<PUser>( CEO )->GetValue<std::string>( NAME ), saveFile );

	PROJECT_MAP::iterator pIter = projects_.begin();
	for( int i=0; pIter != projects_.end(); ++pIter, ++i )
	{
		CNrpGameProject* prj = (CNrpGameProject*)pIter->second;
		prj->Save( localFolder + "projects/" );
		IniFile::Write( "projects", "project_" + IntToStr(i), prj->GetValue<std::string>( NAME ), saveFile );
	}

	ENGINE_LIST::iterator eIter = engines_.begin();
	for( int i=0; eIter != engines_.end(); ++eIter, ++i )
	{
		(*eIter)->Save( localFolder + "engines/" );
		IniFile::Write( "engines", "engine_" + IntToStr(i), (*eIter)->GetValue<std::string>( NAME ), saveFile );
	}

	TECH_MAP::iterator tIter = technologies_.begin();
	for( int i=0; tIter != technologies_.end(); ++tIter, ++i )
	{
		CNrpTechnology* tech = (CNrpTechnology*)tIter->second;
		tech->Save( localFolder + "techs/" );
		IniFile::Write( "techs", "tech_" + IntToStr(i), tech->GetValue<std::string>( NAME ), saveFile );
	}

	GAME_MAP::iterator gIter = games_.begin();
	for( int i=0; gIter != games_.end(); ++gIter, ++i )
	{
		CNrpGame* game = (CNrpGame*)gIter->second;
		game->Save( localFolder + "games/" );
		IniFile::Write( "games", "game_" + IntToStr(i), game->GetValue<std::string>( NAME ), saveFile );
	}

	USER_LIST::iterator uIter = employers_.begin();
	for( int i=0; uIter != employers_.end(); ++uIter, ++i )
	{
		(*uIter)->Save( localFolder + "users/" );
		IniFile::Write( "users", "user_" + IntToStr(i), (*uIter)->ClassName() + ":" + (*uIter)->GetValue<std::string>( NAME ), saveFile );
	}
}

void CNrpCompany::Load( std::string loadFolder )
{
	std::string loadFile = loadFolder + "/" + "company.ini";
	INrpConfig::Load( PROPERTIES, loadFile );

	std::string ceoName = IniFile::Read( PROPERTIES, "ceo", std::string(""), loadFile );
	SetValue<PUser>( CEO, CNrpApplication::Instance().GetUser( ceoName ) );

	for( int i=0; i < GetValue<int>( PROJECTNUMBER ); ++i )
	{
		std::string prjName = IniFile::Read( "projects", "project_" + IntToStr(i), std::string(""), loadFile );
		CNrpGameProject* prj = new CNrpGameProject( "" );
		prj->Load( loadFolder + "projects/" + prjName );
		projects_[ prjName ] = prj;
	}

	for( int i=0; i < GetValue<int>( ENGINES_NUMBER ); ++i )
	{
		std::string engineName = IniFile::Read( "engines", "engine_" + IntToStr(i), std::string(""), loadFile );
		CNrpGameEngine* ge = new CNrpGameEngine( "" );
		ge->Load( loadFolder + "engines/" );
		engines_.push_back( ge );
	}

	for( int i=0; i < GetValue<int>( TECHNUMBER ); ++i )
	{
		std::string techName = IniFile::Read( "techs", "tech_" + IntToStr(i), std::string(""), loadFile );
		CNrpTechnology* tech = new CNrpTechnology( PROJECT_TYPE(0) );
		tech->Load( loadFolder + "techs/" + techName + ".ini" );
		technologies_[ techName ] = tech;
	}

	for( int i=0; i < GetValue<int>( GAMENUMBER ); ++i )
	{
		std::string gameName = IniFile::Read( "games", "game_" + IntToStr(i), std::string(""), loadFile );
		CNrpGame* game = new CNrpGame(0);
		game->Load( loadFolder + "games/" );
		games_[ gameName ] = game;
	}

	for( int i=0; i < GetValue<int>( USERNUMBER ); ++i )
	{
		std::string name = IniFile::Read( "users", "user_" + IntToStr(i), std::string(""), loadFile );
		std::string className = name.substr( 0, name.find( ':' ) );
		name = name.substr( name.find(':') + 1, 0xff );
		IUser* user = new IUser( className.c_str(), "" );
		user->Load( loadFolder + "users/" + name + ".ini" );
		employers_.push_back( user );
	}
}

CNrpGame* CNrpCompany::GetGame( std::string gameName )
{
	GAME_MAP::iterator gIter = games_.begin();
	for( int i=0; gIter != games_.end(); ++gIter, ++i )
	{
		if( gIter->second->GetValue<std::string>( NAME ) == gameName )
			return gIter->second;
	}

	return NULL;
}
}//namespace nrp