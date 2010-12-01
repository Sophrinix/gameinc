#include "StdAfx.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpScenario.h"
#include "NrpLicense.h"
#include "NrpTechnology.h"
#include "NrpGame.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "OpFileSystem.h"
#include "IniFile.h"
#include "NrpPlatform.h"

#include <errno.h>

namespace nrp
{
CLASS_NAME CLASS_GAMEPROJECT( "CNrpGameProject" );

CNrpGameProject::CNrpGameProject( const NrpText& name, CNrpCompany* ptrCompany )
: INrpProject( CLASS_GAMEPROJECT, name )
{
	_InitializeOptions( name );
	Param( PARENTCOMPANY ) = ptrCompany;
}

CNrpGameProject::CNrpGameProject( const CNrpGameProject& ptr ) : INrpProject( CLASS_GAMEPROJECT, ptr[ NAME ] )
{
	throw "function not done";
}

CNrpGameProject::~CNrpGameProject(void)
{
}

void CNrpGameProject::SetGameEngine( CNrpGameEngine* gameEng )
{
	Param( GAME_ENGINE ) = (PNrpGameEngine)gameEng;
	int modNumber = gameEng ? (int)(*gameEng)[ GENRE_MODULE_NUMBER ] : 0;
	Param( GENRE_MODULE_NUMBER ) = modNumber;
	Param( BASE_CODEVOLUME ) = gameEng ? (int)(*gameEng)[ CODEVOLUME ] : 0;
	Param( BASEQUALITY ) = gameEng ? (int)(*gameEng)[ QUALITY ] : 0;

	_genres.clear();
	for( int i=0; i < modNumber; i++ )
		_genres.push_back( NULL );
	CalculateCodeVolume();
}

bool CNrpGameProject::IsGenreIncluded( GENRE_TYPE typen )
{
	for( u32 i=0; i < _genres.size(); i++)
		if( _genres[ i ] && (*_genres[ i ])[TECHTYPE] == (int)typen )
			return true;

	return false;
}

void CNrpGameProject::CalculateCodeVolume()
{
	float baseCode = static_cast<float>( (int)Param( BASE_CODEVOLUME ) );

	float summ = 0;
	float engCoeff = 1;
	int quality = 0;

	TECHS rt;
	_GetAllTech( rt );

	for( u32 i=0; i < rt.size(); i++)
		if( rt[ i ] != NULL )
		{
			engCoeff += (float)(*rt[ i ])[ ENGINE_CODE ];
			quality += (int)(*rt[ i ])[ QUALITY ];
			quality /= 2;
		}

	baseCode *= engCoeff;
	Param( ENGINE_CODEVOLUME ) = static_cast< int >( baseCode );

	for( u32 i=0; i < rt.size(); i++)
		if( rt[ i ] != NULL ) 
		{
			float codeVol = (*rt[ i ])[ BASE_CODE ] * baseCode;
			summ += codeVol;
		}

	summ += baseCode;
	float langSupCode = (int)Param( LANGNUMBER ) * summ * 0.05f;
	Param( LANGUAGESUPPORTCODE ) = static_cast< int >( langSupCode );
	summ += langSupCode;

	float platformSupCode = (int)Param( PLATFORMNUMBER ) * summ * 0.1f;
	Param( PLATFORMSUPPORTCODE ) = static_cast< int >( platformSupCode );
	summ += platformSupCode;
	
	bool projectReady = Param( GAME_ENGINE ).As<PNrpGameEngine>() != NULL;
	projectReady &= ( Param( SCENARIO ).As<PNrpTechnology>() != NULL || Param( GLICENSE ).As<PNrpTechnology>() != NULL );
	projectReady &= ( (int)Param( LANGNUMBER ) >0 && (int)Param( PLATFORMNUMBER ) > 0 );
	projectReady &= ( GetGenre( 0 ) != NULL );
	
	Param( CODEVOLUME ) = static_cast< int >( summ );
	Param( QUALITY ) = quality; 
	Param( PROJECTREADY ) = projectReady;
}

void CNrpGameProject::SetGenre( CNrpTechnology* genre, int number )
{
	if( number >= (int)_genres.size() )
		return;

	if( !genre )
		_genres[ number ] = NULL;
	else
	{	
		int techType = (*genre)[ TECHTYPE ];

		if( Param( GAME_ENGINE ).As<PNrpGameEngine>()->IsGenreAvailble( GENRE_TYPE( techType ) ) &&
			!IsGenreIncluded( GENRE_TYPE( techType ) ) )
			_genres[ number ] = genre;
	}

	CalculateCodeVolume();
}

CNrpTechnology* CNrpGameProject::GetGenre( int index )
{
	return index < (int)_genres.size() ? _genres[ index ] : NULL;
}

bool CNrpGameProject::IsTechInclude( ADV_TECH_TYPE typen )
{
	TECHS rt;
	_GetAllTech( rt );

	for( u32 i=0; i < rt.size(); i++)
		if( rt[ i ] && (*rt[ i ])[ TECHTYPE ] == (int)typen )
			return true;

	return false;
}

void CNrpGameProject::_SetTech( CNrpTechnology* ptrTech, int index, TECHS& listd, OPTION_NAME optname )
{
	if( index >= (int)listd.size() )
	{
		if( ptrTech )
			listd.push_back( ptrTech );
	}
	else
	{
		if( !ptrTech )
			listd.erase( index );
		else
			listd[ index ] = ptrTech;
	}
	
	Param( optname ) = static_cast< int >( listd.size() );
	CalculateCodeVolume();
}

void CNrpGameProject::SetTechnology( CNrpTechnology* ptrTech, int index )
{
	_SetTech( ptrTech, index, _technologies, ADVTECHNUMBER );
}

CNrpTechnology* CNrpGameProject::GetTechnology( int index )
{
	return ( index < (int)_technologies.size() ) ? _technologies[ index ] : NULL;
}

void CNrpGameProject::SetVideoTech( CNrpTechnology* ptrTech, int index )
{
	_SetTech( ptrTech, index, _video, VIDEOTECHNUMBER );
}

CNrpTechnology* CNrpGameProject::GetVideoTech( int index )
{
	return index < (int)_video.size() ? _video[ index ] : NULL;
}

CNrpTechnology* CNrpGameProject::GetSoundTech( int index )
{
	return index < (int)_sound.size() ? _sound[ index ] : NULL;
}

void CNrpGameProject::SetSoundTech( CNrpTechnology* ptrTech, int index )
{
	_SetTech( ptrTech, index, _sound, SOUNDTECHNUMBER );
}

NrpText CNrpGameProject::Save( const NrpText& folderSave )
{
	assert( OpFileSystem::IsExist(folderSave) );

	NrpText localFolder = OpFileSystem::CheckEndSlash( folderSave + Text( NAME ) );

	OpFileSystem::CreateDirectory( localFolder );

	NrpText fileName = localFolder + "project.ini";
	INrpProject::Save( fileName );

	IniFile sv( fileName );

	sv.SetArray< TECHS >( SECTION_TECHS, _technologies,  CreateKeyTech, NAME, false );
	sv.SetArray< TECHS >( SECTION_GENRES, _genres,  CreateKeyTech, NAME, false );
	sv.SetArray< TECHS >( SECTION_VIDEO, _video, CreateKeyTech, NAME, false );
	sv.SetArray< TECHS >( SECTION_SOUND, _sound, CreateKeyTech, NAME, false );

	if( PNrpGameEngine engine = Param( GAME_ENGINE ).As<PNrpGameEngine>() )
		sv.Set( SECTION_PROPERTIES, GAME_ENGINE, engine->Text( NAME ) );

	if( CNrpGame* game = Param( PREV_GAME ).As<PNrpGame>() )
		sv.Set( SECTION_PROPERTIES, PREV_GAME, game->Text( NAME ) );

	if( PNrpScenario scen = Param( SCENARIO ).As<PNrpScenario>() )
	{
		sv.Set( SECTION_PROPERTIES, SCENARIO, scen->Text(NAME) );
		scen->Save( localFolder + SCENARIO + ".ini" );
	}

	if( PNrpLicense lic = Param( GLICENSE ).As<PNrpLicense>() )
	{
		sv.Set( SECTION_PROPERTIES, GLICENSE, lic->Text(NAME) );
		lic->Save( localFolder + GLICENSE + ".ini");
	}

	_SaveTech( ENGINEEXTENDED, folderSave, &sv );
	_SaveTech( LOCALIZATION, folderSave, &sv );
	_SaveTech( CROSSPLATFORMCODE, folderSave, &sv );

	return fileName;
}

void CNrpGameProject::_SaveTech( const OPTION_NAME& name, const NrpText& saveFolder, IniFile* ini )
{
	if( PNrpTechnology tech = Param( name ).As<PNrpTechnology>() )
	{
		NrpText fileName = tech->Save( saveFolder );
		ini->Set( SECTION_PROPERTIES, ENGINEEXTENDED, fileName );
	}
}

void CNrpGameProject::_LoadTechs( const SECTION_NAME& section, NrpText (*func)(int index), int maxVal, TECHS& arr, IniFile* ini )
{
	for( int i=0; i < maxVal; ++i )
	{
		NrpText name = ini->Get( section, func(i), NrpText("") );
		PNrpTechnology tech = CNrpApplication::Instance().GetTechnology( name );
		assert( tech );
		if( tech )
			arr.push_back( tech );
	}
}

void CNrpGameProject::Load( const NrpText& loadFolder )
{
	NrpText fileName = OpFileSystem::CheckEndSlash( loadFolder ) + "project.ini";
	CNrpCompany* ptrCompany = Param( PARENTCOMPANY ).As<PNrpCompany>();
	INrpProject::Load( fileName );

	IniFile lv( fileName );
	_LoadTechs( SECTION_TECHS, CreateKeyTech, Param( ADVTECHNUMBER ), _technologies, &lv );
	_LoadTechs( SECTION_GENRES, CreateKeyTech, Param( GENRE_MODULE_NUMBER ), _genres, &lv );
	_LoadTechs( SECTION_VIDEO, CreateKeyTech, Param( VIDEOTECHNUMBER ), _video, &lv );	
	_LoadTechs( SECTION_SOUND, CreateKeyTech, Param( SOUNDTECHNUMBER ), _sound, &lv );

	NrpText name = lv.Get( SECTION_PROPERTIES, GAME_ENGINE, NrpText("") );
	Param( GAME_ENGINE ) = CNrpApplication::Instance().GetGameEngine( name );

	name = lv.Get( SECTION_PROPERTIES, SCENARIO, NrpText("") );
	PNrpScenario scenario = new CNrpScenario( name );
	scenario->Load( loadFolder + SCENARIO + ".ini" ); 
	Param( SCENARIO ) = scenario;

	name = lv.Get( SECTION_PROPERTIES, GLICENSE, NrpText("") );
	PNrpLicense license = new CNrpLicense( name );
	license->Load( loadFolder + GLICENSE + ".ini" );
	Param( GLICENSE ) = license;

	NrpText saveFile = lv.Get( SECTION_PROPERTIES, ENGINEEXTENDED, NrpText("") );
	Param( ENGINEEXTENDED ) = new CNrpTechnology( saveFile );

	saveFile = lv.Get( SECTION_PROPERTIES, LOCALIZATION, NrpText("") );
	Param( LOCALIZATION ) = new CNrpTechnology( saveFile );

	saveFile = lv.Get( SECTION_PROPERTIES, CROSSPLATFORMCODE, NrpText("") );
	Param( CROSSPLATFORMCODE ) = new CNrpTechnology( saveFile );
}

void CNrpGameProject::_InitializeOptions( const NrpText& name )
{
	Param( TECHTYPE ) = (int)PT_GAME;
	Param( NAME ) = name;
	Add( GAME_ENGINE, static_cast<PNrpGameEngine>( NULL ) );
	Add( GENRE_MODULE_NUMBER, (int)0 );
	Add( PREV_GAME, (PNrpGame)NULL );
	Add( CODEVOLUME, (int)0 );
	Add( BASE_CODEVOLUME, (int)0 );
	Add<PNrpTechnology>( SCENARIO, NULL );
	Add<PNrpTechnology>( GLICENSE, NULL ); 
	Add<PNrpTechnology>( SCRIPTENGINE, NULL );
	Add<PNrpTechnology>( MINIGAMEENGINE, NULL );
	Add<PNrpTechnology>( PHYSICSENGINE, NULL );
	Add<PNrpTechnology>( GRAPHICQUALITY, NULL );
	Add( VIDEOTECHNUMBER, 0 );
	Add<PNrpTechnology>( SOUNDQUALITY, NULL );
	Add( SOUNDTECHNUMBER, 0 );
	Add( LANGNUMBER, 0 );
	Add( PLATFORMNUMBER, 0 );
	Add( BASEQUALITY, 0 );
	Add( ADVTECHNUMBER, 0 );
	Add( ENGINE_CODEVOLUME, 0 );
	Add( QUALITY, 0 );
	Add<NrpText>( COMPANYNAME, "" );
	Add<PNrpCompany>( PARENTCOMPANY, NULL );
	Add<int>( PLATFORMSUPPORTCODE, 0 );
	Add<int>( LANGUAGESUPPORTCODE, 0 );
	Add<PNrpTechnology>( ENGINEEXTENDED, NULL );
	Add<PNrpTechnology>( LOCALIZATION, NULL );
	Add<PNrpTechnology>( CROSSPLATFORMCODE, NULL ); 
	Add( MONEYONDEVELOP, (int)0 );
	Add( PROJECTREADY, false );
	Add( FAMOUS, 0.f );
}

void CNrpGameProject::_GetAllTech( TECHS& techList )
{
	techList = _technologies;
	techList.push_back( Param(MINIGAMEENGINE) );
	techList.push_back( Param(SCRIPTENGINE) );
	techList.push_back( Param(PHYSICSENGINE) );
	techList.push_back( Param(GRAPHICQUALITY) );
	techList.push_back( Param(SOUNDQUALITY) );
	techList.push_back( Param(ENGINEEXTENDED ));
	techList.push_back( Param(LOCALIZATION) );
	techList.push_back( Param(CROSSPLATFORMCODE) );

	AddArrayTo< TECHS >( techList, _video );
	AddArrayTo< TECHS >( techList, _genres );
	AddArrayTo< TECHS >( techList, _sound );
}

NrpText CNrpGameProject::ClassName()
{
	return CLASS_GAMEPROJECT;
}

CNrpPlatform* CNrpGameProject::GetPlatform( int index )
{
	assert( index < _platforms.size() );
	return index < _platforms.size() ? _platforms[ index ] : NULL; 
}

CNrpPlatform* CNrpGameProject::GetPlatform( const NrpText& name )
{
	return FindByNameAndIntName<PLATFORMS, CNrpPlatform>( _platforms, name );
}

void CNrpGameProject::SetPlatform( CNrpPlatform* platform, int index/*=-1 */ )
{
	if( FindByNameAndIntName<PLATFORMS, CNrpPlatform>( _platforms, (*platform)[ INTERNAL_NAME ] ) == NULL )
	{
		_platforms.push_back( platform );
		Param( PLATFORMNUMBER ) = static_cast< int >( _platforms.size() );
	}
}

void CNrpGameProject::SetLanguage( CNrpTechnology* language, int index/*=-1 */ )
{
	if( FindByNameAndIntName<TECHS, CNrpTechnology>( _languages, (*language)[ INTERNAL_NAME ] ) == NULL )
	{
		_languages.push_back( language );
		Param( LANGNUMBER ) = static_cast< int >( _languages.size() );
	}
}

CNrpTechnology* CNrpGameProject::GetLanguage( int index )
{
	assert( index < _languages.size() );
	return index < _languages.size() ? _languages[ index ] : NULL;
}

CNrpTechnology* CNrpGameProject::GetLanguage( const NrpText& name )
{
	return FindByNameAndIntName<TECHS, CNrpTechnology>( _languages, name );
}

}//end namespace nrp