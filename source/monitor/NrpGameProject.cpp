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

#include <errno.h>

namespace nrp
{
CLASS_NAME CLASS_GAMEPROJECT( "CNrpGameProject" );

CNrpGameProject::CNrpGameProject( const NrpText& name, CNrpCompany* ptrCompany )
: INrpProject( CLASS_GAMEPROJECT, name )
{
	_InitializeOptions( name );
	SetValue<PNrpCompany>( PARENTCOMPANY, ptrCompany );
}

CNrpGameProject::CNrpGameProject( CNrpGameProject& ptr ) : INrpProject( CLASS_GAMEPROJECT, ptr.GetString( NAME ) )
{
	throw "function not done";
}

CNrpGameProject::~CNrpGameProject(void)
{
}

void CNrpGameProject::SetGameEngine( CNrpGameEngine* gameEng )
{
	SetValue<PNrpGameEngine>( GAME_ENGINE, (PNrpGameEngine)gameEng );
	int modNumber = gameEng ? gameEng->GetValue<int>( GENRE_MODULE_NUMBER ) : 0;
	SetValue<int>( GENRE_MODULE_NUMBER, modNumber );
	SetValue<int>( BASE_CODEVOLUME, gameEng ? gameEng->GetValue<int>( CODEVOLUME ) : 0 );
	SetValue<int>( BASEQUALITY, gameEng ? gameEng->GetValue<int>( QUALITY ) : 0 );

	_genres.clear();
	for( int i=0; i < modNumber; i++ )
		_genres.push_back( NULL );
	CalculateCodeVolume();
}

bool CNrpGameProject::IsGenreIncluded( GENRE_TYPE typen )
{
	for( u32 i=0; i < _genres.size(); i++)
		if( _genres[ i ] && _genres[ i ]->GetValue<GENRE_TYPE>(TECHTYPE) == typen )
			return true;

	return false;
}

void CNrpGameProject::_FindPlaformsAndLanguages()
{
	PropertyArray::Iterator pIter = GetProperties().getIterator();

	int platformNumber = 0;
	int langNumber = 0;
	for( ; !pIter.atEnd(); pIter++ )
	{
		if( (*pIter).getKey().find( "platform_" ) == 0 )
			platformNumber += GetValue<bool>( (*pIter).getKey() ) ? 1 : 0;

		if( (*pIter).getKey().find( "language_" ) == 0 )
			langNumber += GetValue<bool>( (*pIter).getKey() ) ? 1 : 0;
	}

	SetValue<int>( LANGNUMBER, langNumber );
	SetValue<int>( PLATFORMNUMBER, platformNumber );
}

void CNrpGameProject::CalculateCodeVolume()
{
	_FindPlaformsAndLanguages();

	float baseCode = (float)GetValue<int>( BASE_CODEVOLUME );

	float summ = 0;
	float engCoeff = 1;
	int quality = 0;

	TECHS rt;
	_GetAllTech( rt );

	for( u32 i=0; i < rt.size(); i++)
		if( rt[ i ] != NULL )
		{
			engCoeff += rt[ i ]->GetValue<float>( ENGINE_CODE );
			quality += rt[ i ]->GetValue<int>( QUALITY );
			quality /= 2;
		}

	baseCode *= engCoeff;
	SetValue<int>( ENGINE_CODEVOLUME, (int)baseCode );

	for( u32 i=0; i < rt.size(); i++)
		if( rt[ i ] != NULL ) 
		{
			float codeVol = rt[ i ]->GetValue<float>( BASE_CODE ) * baseCode;
			summ += codeVol;
		}

	summ += baseCode;
	float langSupCode = summ * GetValue<int>( LANGNUMBER ) * 0.05f;
	SetValue<int>( LANGUAGESUPPORTCODE, (int)langSupCode );
	summ += langSupCode;

	float platformSupCode = summ *  GetValue<int>( PLATFORMNUMBER ) * 0.1f;
	SetValue<int>( PLATFORMSUPPORTCODE, (int)platformSupCode );
	summ += platformSupCode;
	
	bool projectReady = GetValue<PNrpGameEngine>( GAME_ENGINE ) != NULL;
	projectReady &= ( GetValue<PNrpScenario>( SCENARIO ) != NULL || GetValue<PNrpLicense>( GLICENSE ) != NULL );
	projectReady &= ( GetValue<int>( LANGNUMBER ) > 0 && GetValue<int>( PLATFORMNUMBER ) > 0 );
	projectReady &= ( GetGenre( 0 ) != NULL );
	
	SetValue<int>( CODEVOLUME, (int)summ );
	SetValue<int>( QUALITY, quality ); 
	SetValue<bool>( PROJECTREADY, projectReady );
}

void CNrpGameProject::SetGenre( CNrpTechnology* genre, int number )
{
	if( number >= (int)_genres.size() )
		return;

	if( !genre )
		_genres[ number ] = NULL;
	else
	{	
		GENRE_TYPE techType = genre->GetValue<GENRE_TYPE>( TECHTYPE );

		if( GetValue<PNrpGameEngine>( GAME_ENGINE )->IsGenreAvailble( techType ) &&
			!IsGenreIncluded( techType ) )
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
		if( rt[ i ] && rt[ i ]->GetValue<ADV_TECH_TYPE>(TECHTYPE) == typen )
			return true;

	return false;
}

void CNrpGameProject::_SetTech( CNrpTechnology* ptrTech, int index, TECHS& listd, OPTION_NAME optname )
{
	if( index >= (int)listd.size() )
		listd.push_back( ptrTech );
	else
	{
		if( !ptrTech )
			listd.erase( index );
		else
			listd[ index ] = ptrTech;
	}
	SetValue<int>( optname, listd.size() );
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

	NrpText localFolder = OpFileSystem::CheckEndSlash( folderSave + GetString( NAME ) );

	OpFileSystem::CreateDirectory( localFolder );

	NrpText fileName = localFolder + "project.ini";
	INrpProject::Save( fileName );

	IniFile sv( fileName );

	sv.SetArray< TECHS >( SECTION_TECHS, _technologies,  CreateKeyTech, NAME, false );
	sv.SetArray< TECHS >( SECTION_GENRES, _genres,  CreateKeyTech, NAME, false );
	sv.SetArray< TECHS >( SECTION_VIDEO, _video, CreateKeyTech, NAME, false );
	sv.SetArray< TECHS >( SECTION_SOUND, _sound, CreateKeyTech, NAME, false );

	if( GetValue<PNrpGameEngine>( GAME_ENGINE ) )
	{
		PNrpGameEngine engine = GetValue<PNrpGameEngine>( GAME_ENGINE );
		sv.Set( SECTION_PROPERTIES, GAME_ENGINE, engine->GetString( NAME ) );
	}

	if( GetValue<PNrpGame>( PREV_GAME ) )
		sv.Set( SECTION_PROPERTIES, PREV_GAME, GetValue<PNrpGame>( PREV_GAME )->GetString(NAME) );

	if( GetValue<PNrpScenario>( SCENARIO ) )
	{
		sv.Set( SECTION_PROPERTIES, SCENARIO, GetValue<PNrpScenario>( SCENARIO )->GetString(NAME) );
		GetValue<PNrpScenario>( SCENARIO )->Save( localFolder + SCENARIO + ".ini" );
	}

	if( GetValue<PNrpLicense>( GLICENSE ) )
	{
		sv.Set( SECTION_PROPERTIES, GLICENSE, GetValue<PNrpLicense>( GLICENSE )->GetString(NAME) );
		GetValue<PNrpLicense>( GLICENSE )->Save( localFolder + GLICENSE + ".ini");
	}

	_SaveTech( ENGINEEXTENDED, folderSave, &sv );
	_SaveTech( LOCALIZATION, folderSave, &sv );
	_SaveTech( CROSSPLATFORMCODE, folderSave, &sv );

	return fileName;
}

void CNrpGameProject::_SaveTech( const OPTION_NAME& name, const NrpText& saveFolder, IniFile* ini )
{
	if( PNrpTechnology tech = GetValue<PNrpTechnology>( name ) )
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
	CNrpCompany* ptrCompany = GetValue<PNrpCompany>( PARENTCOMPANY );
	INrpProject::Load( fileName );

	IniFile lv( fileName );
	_LoadTechs( SECTION_TECHS, CreateKeyTech, GetValue<int>( ADVTECHNUMBER ), _technologies, &lv );
	_LoadTechs( SECTION_GENRES, CreateKeyTech, GetValue<int>( GENRE_MODULE_NUMBER ), _genres, &lv );
	_LoadTechs( SECTION_VIDEO, CreateKeyTech, GetValue<int>( VIDEOTECHNUMBER ), _video, &lv );	
	_LoadTechs( SECTION_SOUND, CreateKeyTech, GetValue<int>( SOUNDTECHNUMBER ), _sound, &lv );

	NrpText name = lv.Get( SECTION_PROPERTIES, GAME_ENGINE, NrpText("") );
	SetValue<PNrpGameEngine>( GAME_ENGINE, CNrpApplication::Instance().GetGameEngine( name ) );

	name = lv.Get( SECTION_PROPERTIES, SCENARIO, NrpText("") );
	PNrpScenario scenario = new CNrpScenario( name );
	scenario->Load( loadFolder + SCENARIO + ".ini" ); 
	SetValue<PNrpScenario>( SCENARIO, scenario );

	name = lv.Get( SECTION_PROPERTIES, GLICENSE, NrpText("") );
	PNrpLicense license = new CNrpLicense( name );
	license->Load( loadFolder + GLICENSE + ".ini" );
	SetValue<PNrpLicense>( GLICENSE, license );

	NrpText saveFile = lv.Get( SECTION_PROPERTIES, ENGINEEXTENDED, NrpText("") );
	SetValue<PNrpTechnology>( ENGINEEXTENDED, new CNrpTechnology( saveFile ) );

	saveFile = lv.Get( SECTION_PROPERTIES, LOCALIZATION, NrpText("") );
	SetValue<PNrpTechnology>( LOCALIZATION, new CNrpTechnology( saveFile ) );

	saveFile = lv.Get( SECTION_PROPERTIES, CROSSPLATFORMCODE, NrpText("") );
	SetValue<PNrpTechnology>( CROSSPLATFORMCODE, new CNrpTechnology( saveFile )  );
}

void CNrpGameProject::_InitializeOptions( const NrpText& name )
{
	SetValue<PROJECT_TYPE>( TECHTYPE, PT_GAME );
	SetValue<NrpText>( NAME, name );
	CreateValue<PNrpGameEngine>( GAME_ENGINE, NULL );
	CreateValue<int>( GENRE_MODULE_NUMBER, 0 );
	CreateValue<PNrpGame>( PREV_GAME, NULL );
	CreateValue<int>( CODEVOLUME, 0 );
	CreateValue<int>( BASE_CODEVOLUME, 0 );
	CreateValue<PNrpScenario>( SCENARIO, NULL );
	CreateValue<PNrpLicense>( GLICENSE, NULL ); 
	CreateValue<PNrpTechnology>( SCRIPTENGINE, NULL );
	CreateValue<PNrpTechnology>( MINIGAMEENGINE, NULL );
	CreateValue<PNrpTechnology>( PHYSICSENGINE, NULL );
	CreateValue<PNrpTechnology>( GRAPHICQUALITY, NULL );
	CreateValue<int>( VIDEOTECHNUMBER, 0 );
	CreateValue<PNrpTechnology>( SOUNDQUALITY, NULL );
	CreateValue<int>( SOUNDTECHNUMBER, 0 );
	CreateValue<int>( LANGNUMBER, 0 );
	CreateValue<int>( PLATFORMNUMBER, 0 );
	CreateValue<int>( BASEQUALITY, 0 );
	CreateValue<int>( ADVTECHNUMBER, 0 );
	CreateValue<int>( ENGINE_CODEVOLUME, 0 );
	CreateValue<int>( QUALITY, 0 );
	CreateValue<NrpText>( COMPANYNAME, "" );
	CreateValue<PNrpCompany>( PARENTCOMPANY, NULL );
	CreateValue<int>( PLATFORMSUPPORTCODE, 0 );
	CreateValue<int>( LANGUAGESUPPORTCODE, 0 );
	CreateValue<PNrpTechnology>( ENGINEEXTENDED, NULL );
	CreateValue<PNrpTechnology>( LOCALIZATION, NULL );
	CreateValue<PNrpTechnology>( CROSSPLATFORMCODE, NULL ); 
	CreateValue<int>( MONEYONDEVELOP, 0 );
	CreateValue<bool>( PROJECTREADY, false );
	CreateValue<float>( FAMOUS, 0 );
}

void CNrpGameProject::_GetAllTech( TECHS& techList )
{
	techList = _technologies;
	techList.push_back( GetValue<PNrpTechnology>(MINIGAMEENGINE));
	techList.push_back( GetValue<PNrpTechnology>(SCRIPTENGINE));
	techList.push_back( GetValue<PNrpTechnology>(PHYSICSENGINE));
	techList.push_back( GetValue<PNrpTechnology>(GRAPHICQUALITY));
	techList.push_back( GetValue<PNrpTechnology>(SOUNDQUALITY));
	techList.push_back( GetValue<PNrpTechnology>(ENGINEEXTENDED));
	techList.push_back( GetValue<PNrpTechnology>(LOCALIZATION));
	techList.push_back( GetValue<PNrpTechnology>(CROSSPLATFORMCODE));

	AddArrayTo< TECHS >( techList, _video );
	AddArrayTo< TECHS >( techList, _genres );
	AddArrayTo< TECHS >( techList, _sound );
}

NrpText CNrpGameProject::ClassName()
{
	return CLASS_GAMEPROJECT;
}

}//namespace nrp