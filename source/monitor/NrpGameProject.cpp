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
	_self[ GAME_ENGINE ] = (PNrpGameEngine)gameEng;
	int modNumber = gameEng ? (int)(*gameEng)[ GENRE_MODULE_NUMBER ] : 0;
	_self[ GENRE_MODULE_NUMBER ] = modNumber;
	_self[ BASE_CODEVOLUME ] = gameEng ? (int)(*gameEng)[ CODEVOLUME ] : 0;
	_self[ BASEQUALITY ] = gameEng ? (int)(*gameEng)[ QUALITY ] : 0;

	_genres.clear();
	for( int i=0; i < modNumber; i++ )
		_genres.push_back( NULL );

	CalculateCodeVolume();
}

bool CNrpGameProject::IsGenreIncluded( const CNrpTechnology* checkGenre ) const
{
	for( u32 i=0; i < _genres.size(); i++)
		if( _genres[ i ] == checkGenre )
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
	_self[ ENGINE_CODEVOLUME ] = static_cast< int >( baseCode );

	for( u32 i=0; i < rt.size(); i++)
		if( rt[ i ] != NULL ) 
		{
			float codeVol = (*rt[ i ])[ BASE_CODE ] * baseCode;
			summ += codeVol;
		}

	summ += baseCode;
	float langSupCode = (int)_self[ LANGNUMBER ] * summ * 0.05f;
	_self[ LANGUAGESUPPORTCODE ] = static_cast< int >( langSupCode );
	summ += langSupCode;

	float platformSupCode = (int)_self[ PLATFORMNUMBER ] * summ * 0.1f;
	_self[ PLATFORMSUPPORTCODE ] = static_cast< int >( platformSupCode );
	summ += platformSupCode;
	
	bool projectReady = _self[ GAME_ENGINE ].As<PNrpGameEngine>() != NULL;
	projectReady &= ( _self[ SCENARIO ].As<PNrpTechnology>() != NULL || _self[ GLICENSE ].As<PNrpTechnology>() != NULL );
	projectReady &= ( (int)_self[ LANGNUMBER ] >0 && (int)_self[ PLATFORMNUMBER ] > 0 );
	projectReady &= ( GetGenre( 0 ) != NULL );
	
	_self[ CODEVOLUME ] = static_cast< int >( summ );
	_self[ QUALITY ] = quality; 
	_self[ PROJECTREADY ] = projectReady;
}

void CNrpGameProject::SetGenre( CNrpTechnology* genre, int number )
{
	if( number >= (int)_genres.size() )
		return;

	if( !genre )
		_genres[ number ] = NULL;
	else
	{	
		const NrpText& genreName = (*genre)[ INTERNAL_NAME ];

		if( _self[ GAME_ENGINE ].As<PNrpGameEngine>()->IsGenreAvailble( genreName ) &&
			!IsGenreIncluded( genre ) )
			_genres[ number ] = genre;
	}

	CalculateCodeVolume();
}

CNrpTechnology* CNrpGameProject::GetGenre( int index )
{
	return index < (int)_genres.size() ? _genres[ index ] : NULL;
}

bool CNrpGameProject::IsTechInclude( const CNrpTechnology* checkTech ) 
{
	TECHS rt;
	_GetAllTech( rt );

	for( u32 i=0; i < rt.size(); i++)
		if( rt[ i ] == checkTech )
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
	
	_self[ optname ] = static_cast< int >( listd.size() );
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

	NrpText fileName = localFolder + "game.project";
	INrpProject::Save( fileName );

	IniFile sv( fileName );

	sv.Set( SECTION_TECHS, _technologies,  CreateKeyTech, INTERNAL_NAME );
	sv.Set( SECTION_GENRES, _genres,  CreateKeyTech, INTERNAL_NAME );
	sv.Set( SECTION_VIDEO, _video, CreateKeyTech, INTERNAL_NAME );
	sv.Set( SECTION_SOUND, _sound, CreateKeyTech, INTERNAL_NAME );
	sv.Set( SECTION_PLATFORMS, _platforms, CreateKeyTech, INTERNAL_NAME );
	sv.Set( SECTION_LANGUGAGES, _languages, CreateKeyTech, INTERNAL_NAME );

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

	return fileName;
}

void CNrpGameProject::_SaveTech( const OPTION_NAME& name, const NrpText& saveFolder, IniFile* ini )
{
	if( PNrpTechnology tech = _self[ name ].As<PNrpTechnology>() )
	{
		NrpText fileName = tech->Save( saveFolder );
		ini->Set( SECTION_PROPERTIES, name, fileName );
	}
}

void CNrpGameProject::Load( const NrpText& loadFolder )
{
	NrpText fileName = OpFileSystem::CheckEndSlash( loadFolder ) + "project.ini";
	CNrpCompany* ptrCompany = Param( PARENTCOMPANY ).As<PNrpCompany>();
	INrpProject::Load( fileName );

	IniFile lv( fileName );
	lv.Get( SECTION_TECHS, CreateKeyTech, _self[ ADVTECHNUMBER ], _technologies, &CNrpApplication::GetTechnology, &_nrpApp );
	lv.Get( SECTION_GENRES, CreateKeyTech, _self[ GENRE_MODULE_NUMBER ], _genres, &CNrpApplication::GetTechnology,&_nrpApp );
	lv.Get( SECTION_VIDEO, CreateKeyTech, _self[ VIDEOTECHNUMBER ], _video, &CNrpApplication::GetTechnology, &_nrpApp );	
	lv.Get( SECTION_SOUND, CreateKeyTech, _self[ SOUNDTECHNUMBER ], _sound, &CNrpApplication::GetTechnology, &_nrpApp );
	lv.Get( SECTION_PLATFORMS, CreateKeyTech, _self[ PLATFORMNUMBER ], _platforms, &CNrpApplication::GetPlatform, &_nrpApp );
	lv.Get( SECTION_LANGUGAGES, CreateKeyTech, _self[ LANGNUMBER ], _languages, &CNrpApplication::GetTechnology, &_nrpApp );

	NrpText name = lv.Get( SECTION_PROPERTIES, GAME_ENGINE, NrpText("") );
	_self[ GAME_ENGINE ] = _nrpApp.GetGameEngine( name );

	name = lv.Get( SECTION_PROPERTIES, SCENARIO, NrpText("") );
	PNrpScenario scenario = new CNrpScenario( name );
	scenario->Load( loadFolder + SCENARIO + ".ini" ); 
	_self[ SCENARIO ] = scenario;

	name = lv.Get( SECTION_PROPERTIES, GLICENSE, NrpText("") );
	PNrpLicense license = new CNrpLicense( name );
	license->Load( loadFolder + GLICENSE + ".ini" );
	_self[ GLICENSE ] = license;

	NrpText saveFile = lv.Get( SECTION_PROPERTIES, ENGINEEXTENDED, NrpText("") );
	Param( ENGINEEXTENDED ) = new CNrpTechnology( saveFile );
}

void CNrpGameProject::_InitializeOptions( const NrpText& name )
{
	_self[ TECHGROUP ] = (int)PT_GAME;
	_self[ NAME ] = name;
	Add( GAME_ENGINE, (PNrpGameEngine)NULL );
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
	Add( PLATFORMSUPPORTCODE, 0 );
	Add( LANGUAGESUPPORTCODE, 0 );
	Add<PNrpTechnology>( ENGINEEXTENDED, NULL );
	Add<PNrpTechnology>( LOCALIZATION, NULL );
	Add<PNrpTechnology>( CROSSPLATFORMCODE, NULL ); 
	Add( MONEYONDEVELOP, (int)0 );
	Add( PROJECTREADY, false );
	Add( FAMOUS, 0.f );
	Add( CPU, 0.f );
	Add( RAM, 0.f );
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
		_self[ PLATFORMNUMBER ] = static_cast< int >( _platforms.size() );
	}
}

void CNrpGameProject::SetLanguage( CNrpTechnology* language, int index/*=-1 */ )
{
	if( FindByNameAndIntName<TECHS, CNrpTechnology>( _languages, (*language)[ INTERNAL_NAME ] ) == NULL )
	{
		_languages.push_back( language );
		_self[ LANGNUMBER ] = static_cast< int >( _languages.size() );
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

void CNrpGameProject::RemoveLanguage( CNrpTechnology* ptrLang )
{
	int pos = -1;
	assert( ptrLang );
	if( !ptrLang ) 
		return;
	if( FindByNameAndIntName<TECHS, CNrpTechnology>( _languages, (*ptrLang)[ INTERNAL_NAME ], &pos ) != NULL )
	{
		_languages.erase( pos );
		_self[ LANGNUMBER ] = static_cast< int >( _languages.size() );
	}
}

void CNrpGameProject::RemovePlatform( CNrpPlatform* platform )
{
	int pos = -1;
	assert( platform );
	if( !platform ) 
		return;
	if( FindByNameAndIntName<PLATFORMS, CNrpPlatform>( _platforms, (*platform)[ INTERNAL_NAME ], &pos ) != NULL )
	{
		_platforms.erase( pos );
		_self[ PLATFORMNUMBER ] = static_cast< int >( _platforms.size() );
	}
}

}//end namespace nrp