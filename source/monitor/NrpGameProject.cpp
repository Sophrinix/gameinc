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

NrpText _GetNameFromType( PROJECT_TYPE type )
{
	switch( type )
	{
	case PT_ADVTECH: return ADVTECHNUMBER;
	case PT_GENRE: return GENRE_MODULE_NUMBER;
	case PT_VIDEOTECH: return VIDEOTECHNUMBER;
	case PT_SOUNDTECH: return SOUNDTECHNUMBER;
	case PT_LANGUAGE: return LANGNUMBER;
	default: assert( false ); return "error";
	}
}

CNrpGameProject::CNrpGameProject( const NrpText& name, CNrpCompany* ptrCompany )
: INrpProject( CLASS_GAMEPROJECT, name )
{
	_InitializeOptions( name );
	_self[ PARENTCOMPANY ] = ptrCompany;
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

	CalculateCodeVolume();
}

float CNrpGameProject::_CalcEngineBaseCode( float& quality )
{
	float baseCode = static_cast<float>( (int)Param( BASE_CODEVOLUME ) );
	float engCoeff = 1;

	TECHS rt;
	_GetAllTech( rt );

	//Подсчитываем влияние модулей на размер проекта
	for( u32 i=0; i < rt.size(); i++)
		if( rt[ i ] != NULL )
		{
			engCoeff += (float)(*rt[ i ])[ ENGINE_CODE ];
			quality += (int)(*rt[ i ])[ QUALITY ];
			quality /= 2;
		}

	//Подсчитаем вличение платформ на размер проекта
	for( u32 i=0; i < _platforms.size(); i++ )
		if( _platforms[ i ] )
			engCoeff += (float)(*_platforms[ i ])[ ENGINE_CODE ];

	return baseCode;
}

float CNrpGameProject::_CalcFinalCodeVolume( float baseCode )
{
	float summ = 0;

	TECHS rt;
	_GetAllTech( rt );
	//Подсчитаем окончательный размер проекта
	for( u32 i=0; i < rt.size(); i++)
		if( rt[ i ] != NULL ) 
			summ +=  (*rt[ i ])[ BASE_CODE ] * baseCode;

	for( u32 i=0; i < _platforms.size(); i++ )
		if( _platforms[ i ] )
			summ += (float)(*_platforms[ i ])[ BASE_CODE ];

	summ += baseCode;
	float langSupCode = (int)_self[ LANGNUMBER ] * summ * 0.05f;
	_self[ LANGUAGESUPPORTCODE ] = static_cast< int >( langSupCode );
	summ += langSupCode;

	return summ;
}

void CNrpGameProject::CalculateCodeVolume()
{
	float quality = 0;

	float baseCode = _CalcEngineBaseCode( quality );
	float summ = _CalcFinalCodeVolume( baseCode );

	_self[ ENGINE_CODEVOLUME ] = static_cast< int >( baseCode );
	
	bool projectReady = _self[ GAME_ENGINE ].As<PNrpGameEngine>() != NULL;
	projectReady &= ( _self[ SCENARIO ].As<PNrpScenario>() != NULL || _self[ GLICENSE ].As<PNrpLicense>() != NULL );
	projectReady &= ( (int)_self[ LANGNUMBER ] >0 && (int)_self[ PLATFORMNUMBER ] > 0 );
	projectReady &= ( GetTech( PT_GENRE, 0 ) != NULL );
	
	_self[ CODEVOLUME ] = static_cast< int >( summ );
	_self[ QUALITY ] = static_cast< int >( quality ); 
	_self[ PROJECTREADY ] = projectReady;
}

void CNrpGameProject::AddTech( const CNrpTechnology* tech )
{
	assert( tech );
	if( !tech )
		return;

	PROJECT_TYPE type = static_cast< PROJECT_TYPE >( (int)(*tech)[ TECHGROUP ] ); 
	TECHS& arr = GetTechs( type );

	if( PT_GENRE == type )
	{	
		PNrpGameEngine ge = _self[ GAME_ENGINE ].As<PNrpGameEngine>();
		if( (int)arr.size() >= (int)(*ge)[ GENRE_MODULE_NUMBER ] ) 
			return;
	}

	if( !IsMyTech( tech ) ) 
		arr.push_back( const_cast< CNrpTechnology* >( tech ) );

	_self[ _GetNameFromType( type ) ] = static_cast< int >( arr.size() );
	CalculateCodeVolume();
}

void CNrpGameProject::RemoveTech( const CNrpTechnology* tech )
{
	assert( tech );
	if( !tech )
		return;
	PROJECT_TYPE type = static_cast< PROJECT_TYPE >( (int)(*tech)[ TECHGROUP ] );
	TECHS& arr = GetTechs( type );

	for( u32 i=0; i < arr.size(); i++ )
		if( tech == arr[ i ] )
		{
			arr.erase( i );
			break;
		}

	_self[ _GetNameFromType( type ) ] = static_cast< int >( arr.size() );
	CalculateCodeVolume();
}


bool CNrpGameProject::IsMyTech( const CNrpTechnology* checkTech )
{
	TECHS rt;
	_GetAllTech( rt );

	for( u32 i=0; i < rt.size(); i++)
		if( checkTech == rt[ i ] )
			return true;

	return false;
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

	sv.Save();
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
	CNrpCompany* ptrCompany = _self[ PARENTCOMPANY ].As<PNrpCompany>();
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
	_self[ ENGINEEXTENDED ] = new CNrpTechnology( saveFile );
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
	Add<PNrpScenario>( SCENARIO, NULL );
	Add<PNrpLicense>( GLICENSE, NULL ); 
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

	AddArrayTo< TECHS >( techList, _video );
	AddArrayTo< TECHS >( techList, _genres );
	AddArrayTo< TECHS >( techList, _sound );
	AddArrayTo< TECHS >( techList, _languages );
}

NrpText CNrpGameProject::ClassName()
{
	return CLASS_GAMEPROJECT;
}

CNrpPlatform* CNrpGameProject::GetPlatform( int index )
{
	assert( index < (int)_platforms.size() );
	return index < (int)_platforms.size() ? _platforms[ index ] : NULL; 
}

CNrpPlatform* CNrpGameProject::GetPlatform( const NrpText& name )
{
	return FindByNameAndIntName<PLATFORMS, CNrpPlatform>( _platforms, name );
}

CNrpPlatform* CNrpGameProject::GetPlatform( PROJECT_TYPE type, int index )
{
	return GetPlatform( index );
}

void CNrpGameProject::AddPlatform( const CNrpPlatform* platform )
{
	if( NULL == FindByNameAndIntName<PLATFORMS, CNrpPlatform>( _platforms, (*platform)[ INTERNAL_NAME ] ) )
	{
		_platforms.push_back( const_cast< CNrpPlatform* >( platform ) );
		_self[ PLATFORMNUMBER ] = static_cast< int >( _platforms.size() );
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

TECHS nullTech;
TECHS& CNrpGameProject::GetTechs( PROJECT_TYPE type )
{
	switch( type ) 
	{
	case PT_ADVTECH: return _technologies;
	case PT_GENRE: return _genres;
	case PT_VIDEOTECH: return _video;
	case PT_SOUNDTECH: return _sound;
	case PT_LANGUAGE: return _languages;
	default: assert( "unknown type" == 0 ); return nullTech;
	}
}

CNrpTechnology* CNrpGameProject::GetTech( PROJECT_TYPE type, int index )
{
	const TECHS& arr = GetTechs( type );
	return index < (int)arr.size() ? arr[ index ] : NULL; 
}

CNrpTechnology* CNrpGameProject::GetTech( PROJECT_TYPE type, const NrpText& name )
{
	const TECHS& arr = GetTechs( type );
	
	return FindByNameAndIntName< TECHS, CNrpTechnology >( arr, name );
}

}//end namespace nrp