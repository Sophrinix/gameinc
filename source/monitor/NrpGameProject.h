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

	CNrpTechnology* GetTech( PROJECT_TYPE type, int index );
	CNrpTechnology* GetTech( PROJECT_TYPE type, const NrpText& Name );
	void AddTech( const CNrpTechnology* tech );
	void RemoveTech( const CNrpTechnology* tech );
	bool IsMyTech( const CNrpTechnology* tech );
	TECHS& GetTechs( PROJECT_TYPE type );

	void AddPlatform( const CNrpPlatform* platform );
	CNrpPlatform* GetPlatform( int index );
	CNrpPlatform* GetPlatform( const NrpText& name );
	CNrpPlatform* GetPlatform( PROJECT_TYPE type, int index );

	void RemovePlatform( CNrpPlatform* platform );

	NrpText Save( const NrpText& folderSave );
	void Load( const NrpText& loadFolder );

	void CalculateCodeVolume();

	const PLATFORMS& GetPlatforms() { return _platforms; }

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

	void _GetAllTech( TECHS& techList );
	void _SaveTech( const OPTION_NAME& name, const NrpText& saveFolder, IniFile* ini );
	float _CalcEngineBaseCode( float& quality );
	float _CalcFinalCodeVolume( float baseCode );
};

typedef CNrpGameProject* PNrpGameProject;

}//namespace nrp