#pragma once
#include "INrpProject.h"
#include <vector>

namespace nrp
{

class CNrpCompany;

OPTION_NAME VIDEOTECH( "videoTech" );
OPTION_NAME GENRETECH( "genreTech" );
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
OPTION_NAME LANGNUMBER( "langNumber" );
OPTION_NAME PLATFORMNUMBER( "platformNumber" );
OPTION_NAME BASE_CODEVOLUME( "baseCodeVolume" );
OPTION_NAME PROJECTREADY( "projectReady" );
OPTION_NAME ADVTECHNUMBER( "advTechNumber" );
OPTION_NAME ENGINE_CODEVOLUME( "engineCodeVolume" );
OPTION_NAME PROJECTSTATUS( "projectStatus" );
OPTION_NAME PLATFORMSUPPORTCODE( "platformSupportCode" );
OPTION_NAME LANGUAGESUPPORTCODE( "languageSupportCode" );

class CNrpGameEngine;
class CNrpTechnology;

class CNrpGameProject : public INrpProject
{
public:
	CNrpGameProject( std::string name, CNrpCompany* ptrCompany );
	CNrpGameProject( CNrpGameProject* nProject );
	~CNrpGameProject(void);

	void SetGameEngine( CNrpGameEngine* gameEng );
	CNrpTechnology* GetGenre( int index );
	void SetGenre( CNrpTechnology* genre, int number );
	bool IsGenreIncluded( GENRE_TYPE typen );
	bool IsTechInclude( ADV_TECH_TYPE typen );
	void SetTechnology( CNrpTechnology* ptrTech, int index );
	void SetVideoTech( CNrpTechnology* ptrTech, int index );
	CNrpTechnology* GetVideoTech( int index );
	CNrpTechnology* GetTechnology( int index );
	void CalculateCodeVolume();
	void SetSoundTech( CNrpTechnology* ptrTech, int index );
	CNrpTechnology* GetSoundTech( int index );
	void Save( std::string folderSave );
	void Load( std::string loadFolder );
	void Update();
	void UpdateDevelopmentMoney();

private:

	void InitializeOption_( std::string name );
	typedef std::vector< CNrpTechnology* > TECH_LIST;
	CNrpGameProject( CNrpGameProject& ptr );
	TECH_LIST technologies_;
	TECH_LIST genres_;
	TECH_LIST videoTechnologies_;
	TECH_LIST soundTechnologies_;

	void SetTech_( CNrpTechnology* ptrTech, int index, TECH_LIST& listd, OPTION_NAME optname );
	void FindPlaformsAndLanguages_();
	void GetAllTech_( TECH_LIST& techList );
};

typedef CNrpGameProject* PNrpGameProject;

}//namespace nrp