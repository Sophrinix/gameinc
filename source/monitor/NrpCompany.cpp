#include "StdAfx.h"
#include "NrpCompany.h"
#include "IUser.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpTechnology.h"
#include "NrpGame.h"

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
	std::string localFolder = saveFolder + GetValue<std::string>( NAME ) + "/";
	std::string saveFile = localFolder + "company.ini";
	INrpConfig::Save( "properties", saveFile );

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
		IniFile::Write( "users", "user_" + IntToStr(i), (*uIter)->GetValue<std::string>( NAME ), saveFile );
	}
}

void CNrpCompany::Load( std::string loadFolder )
{

}
}//namespace nrp