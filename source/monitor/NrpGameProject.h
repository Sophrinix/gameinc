#pragma once
#include "INrpProject.h"
#include "nrpArrays.h"

namespace nrp
{
OPTION_NAME VIDEOTECH( "videoTech" );
OPTION_NAME ADVTECH( "advtech" );
OPTION_NAME SOUNDTECH( "soundTech" );
OPTION_NAME GAME_ENGINE( "gameEngine" );
OPTION_NAME ENGINEEXTENDED( "endgineExtended" );
OPTION_NAME LOCALIZATION( "localozation" );
OPTION_NAME CROSSPLATFORMCODE( "crossPlatformCode" );
OPTION_NAME PREV_GAME( "prevGame" );
OPTION_NAME SCENARIO( "scenario" );
OPTION_NAME GLICENSE( "license" );
OPTION_NAME SCRIPTENGINE( "scriptEngine" );
OPTION_NAME MINIGAMEENGINE( "miniGame" );
OPTION_NAME BASEQUALITY( "baseQuality" );
OPTION_NAME PHYSICSENGINE( "phisicsEngine" ); 
OPTION_NAME GRAPHICQUALITY( "graphicQuality" );
OPTION_NAME VIDEOTECHNUMBER( "videoTechNumber" );
OPTION_NAME SOUNDQUALITY( "soundQuality" );
OPTION_NAME SOUNDTECHNUMBER( "soundTechNumber" );
OPTION_NAME BASE_CODEVOLUME( "baseCodeVolume" );
OPTION_NAME ENGINE_CODEVOLUME( "engineCodeVolume" );
OPTION_NAME PROJECTSTATUS( "projectStatus" );
OPTION_NAME PLATFORMSUPPORTCODE( "platformSupportCode" );
OPTION_NAME LANGUAGESUPPORTCODE( "languageSupportCode" );

class CNrpGameEngine;
class CNrpTechnology;
class CNrpProjectModule;
class IUser;
class CNrpCompany;
class IniFile;

class CNrpGameProject : public INrpProject
{
public:
	CNrpGameProject( const NrpText& name, CNrpCompany* ptrCompany );
	~CNrpGameProject(void);

	void SetGameEngine( CNrpGameEngine* gameEng );
	CNrpTechnology* GetGenre( int index );
	void SetGenre( CNrpTechnology* genre, int number );
	bool IsGenreIncluded( GENRE_TYPE typen );
	bool IsTechInclude( ADV_TECH_TYPE typen );
	void SetTechnology( CNrpTechnology* ptrTech, int index );
	void SetVideoTech( CNrpTechnology* ptrTech, int index );
	void SetPlatform( CNrpPlatform* platform, int index=-1 );
	CNrpPlatform* GetPlatform( int index );
	CNrpPlatform* GetPlatform( const NrpText& name );
	CNrpTechnology* GetVideoTech( int index );
	CNrpTechnology* GetTechnology( int index );
	void SetLanguage( CNrpTechnology* ptrLang, int index=-1 );
	CNrpTechnology* GetLanguage( int index );
	CNrpTechnology* GetLanguage( const NrpText& name );
	void SetSoundTech( CNrpTechnology* ptrTech, int index );
	CNrpTechnology* GetSoundTech( int index );
	NrpText Save( const NrpText& folderSave );
	void Load( const NrpText& loadFolder );

	void CalculateCodeVolume();

	const TECHS& GetTechList() { return _technologies; }
	const TECHS& GetGenreList() { return _genres; }
	const TECHS& GetVideoTechList() { return _video; }
	const TECHS& GetSoundTechList() { return _sound; }
	const TECHS& GetLanguageTechList() { return _languages; }
	const PLATFORMS& GetPlatformsList() { return _platforms; }

	static NrpText ClassName();
private:
	void _InitializeOptions( const NrpText& name );
	CNrpGameProject( const CNrpGameProject& ptr );
	TECHS _technologies;
	TECHS _genres;
	TECHS _video;
	PLATFORMS _platforms;
	TECHS _languages;
	TECHS _sound;

	void _SetTech( CNrpTechnology* ptrTech, int index, TECHS& listd, OPTION_NAME optname );
	void _GetAllTech( TECHS& techList );
	void _SaveTech( const OPTION_NAME& name, const NrpText& saveFolder, IniFile* ini );
	void _LoadTechs( const SECTION_NAME& section, NrpText (*func)(int index), int maxVal, TECHS& arr, IniFile* ini );
};

typedef CNrpGameProject* PNrpGameProject;

}//namespace nrp