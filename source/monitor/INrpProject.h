#pragma once
#include "nrpConfig.h"

namespace nrp
{

const std::string PRJTYPE_NAME[] = { "unknown", "gameengine", "game", 
								     "genre", "scenario", "license",
									 "scripts", "video" };

const std::string GENRE_NAME[] = { "unknown", "skill", "scrolling" };

typedef enum { SKL_UNKNOWN=2000, SKL_VIDEO_TECH, 
				SKL_CODING, 
				SKL_SOUND, SKL_TESTING, SKL_DRAWING,
				SKL_SOUND_TECH, SKL_PHYS_TECH,
				SKILL_MIDDLE = 2099, SKL_COUNT } SKILL_TYPE;

typedef enum { GT_UNKNOWN=300, 
			   GT_SKILL,			//развитие навыков игрока
			   GT_SCROLLING,		//скролл-игры
			   GT_EDUCATION,		//обучающие игры(детские)
			   GT_TEST,				//4 проверка знаний
			   GT_CONTACT,			//5 виртуальное общение(возможно сетевая)
			   GT_HERO,				//6 развитие персонажа
			   GT_TOURE,			//7 исследование мира
			   GT_PAZZLE,			//8 головоломка
			   GT_QUEST,			//9
			   GT_PLATFORMER,		//10 
			   GT_STEALTH,			//11
			   GT_SHOOTER,			//12
			   GT_ACROBATIC,		//13
			   GT_ARCADE,			//14
			   GT_SPORT,			//15
			   GT_SIMULATOR,		//16
			   GT_FIGHTING,			//17
			   GT_RACING,			//18
			   GT_LIFESIM,			//19
			   GT_CITYBUILD,		//20
			   GT_MANAGER,			//21
			   GT_ECONOMIC,			//22
			   GT_SLASHER,			//23
			   GT_ACTION,			//24
			   GT_CONTROL,			//25
			   GT_TACTIC,			//26
			   GT_LOGIC,			//27
			   GT_WARGAME,			//28
			   GT_CARDGAME,			//29
			   GT_COUNT  } GENRE_TYPE;

typedef enum { SCRLVL_UNKNOWN=600, SCRLVL_SIMPLE, SCRLVL_COUNT } SCRIPT_LEVEL;
typedef enum { VDT_UNKNOWN=700, VDT_TEXT_OUTPUT, VDT_COUNT } VIDEO_TECH_TYPE;
typedef enum { SND_UNKNOWN=800, SND_SPEAKER_SOUND, SND_COUNT } SOUND_TECH_TYPE;
typedef enum { MNGM_UNKNOWN=900, MNGM_TEXTQUEST, MNGM_COUNT } MINIGAME_TECH_TYPE;
typedef enum { PHYS_UNKNOWN=1000, PHYS_2DCONCLUSION, PHYS_COUNT } PHYS_TECH_TYPE;
typedef enum { ADV_UNKNOWN=1100, ADV_MEMORY, ADV_JOYSTIK, ADV_HELP, ADV_COUNT } ADV_TECH_TYPE;
typedef enum { VDQ_UNKNOWN=1200, VDQ_SELFRENDER, VDQ_INETART, VDQ_FREELANCE, VDQ_STUDIO, VDQ_UNIQUE, VDQ_COUNT } VIDEO_QUALITY;
typedef enum { SDQ_UNKNOWN=1300, SGQ_SELFCOMPOSE, SDQ_COUNT } SOUND_QUALITY;
typedef enum { SCNQ_UNKNOWN=1400, SCNQ_SELFWRITE, SCNQ_COUNT } SCENARIO_QUALITY;

typedef enum { PT_UNKNOWN=0, 
			   PT_GAME, PT_GAMEENGINE, PT_GENRE, 
			   PT_SCENARIO, PT_LICENSE, PT_SCRIPTS, 
			   PT_VIDEOTECH, PT_SOUNDTECH, PT_MINIGAME,
			   PT_PHYSIC, PT_ADVTECH, PT_VIDEOQUALITY,
			   PT_SOUNDQUALITY, PT_SCENARIOQUALITY,
			   PT_ENGINDEEXTEND, PT_LANGSUPPORT, 
			   PT_PLATFORMSUPPORT, PT_GAMEBOXADDON,
			   PT_COUNT } PROJECT_TYPE;

class INrpProject : public INrpConfig
{
	INrpProject() : INrpConfig( "error", "error" ) {}

public:
	INrpProject( const std::string& className, const std::string& systemName);

	virtual std::string Save( const std::string& fileName );
	~INrpProject(void);
};

typedef INrpProject* PNrpProject;

}//namespace nrp