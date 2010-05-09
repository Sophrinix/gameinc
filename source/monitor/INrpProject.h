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
			   GT_EDUCATION,		//обучающие игры(деские)
			   GT_TEST,				//проверка знаний
			   GT_CONTACT,			//виртуальное общение(возможно сетевая)
			   GT_HERO,				//развитие персонажа
			   GT_TOURE,			//исследование мира
			   GT_PAZZLE,			//головоломка
			   GT_QUEST, 
			   GT_PLATFORMER,		//
			   GT_STEALTH,
			   GT_SHOOTER,			//
			   GT_ACROBATIC,		//
			   GT_ARCADE,			//
			   GT_SPORT,			//
			   GT_SIMULATOR,		//
			   GT_FIGHTING,			//
			   GT_RACING,			//
			   GT_LIFESIM,			//
			   GT_CITYBUILD,		//
			   GT_MANAGER,			//
			   GT_ECONOMIC,			//
			   GT_SLASHER,			//
			   GT_ACTION,			//
			   GT_CONTROL,			//
			   GT_TACTIC,			//
			   GT_LOGIC,			//
			   GT_WARGAME,			//
			   GT_CARDGAME,			//
			   GT_COUNT  } GENRE_TYPE;

typedef enum { SCRLVL_UNKNOWN=600, SCRLVL_SIMPLE, SCRLVL_COUNT } SCRIPT_LEVEL;
typedef enum { VDT_UNKNOWN=700, VDT_TEXT_OUTPUT, VDT_COUNT } VIDEO_TECH_TYPE;
typedef enum { SND_UNKNOWN=800, SND_SPEAKER_SOUND, SND_COUNT } SOUND_TECH_TYPE;

typedef enum { ADV_UNKNOWN=1100, ADV_MEMORY, ADV_COUNT } ADV_TECH_TYPE;
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
			   PT_PLATFORMSUPPORT,
			   PT_COUNT } PROJECT_TYPE;

class INrpProject : public INrpConfig
{
public:
	INrpProject(std::string className, std::string systemName);
	~INrpProject(void);

protected:

	void Load_( char* file_name ) {}
};

typedef INrpProject* PNrpProject;

}//namespace nrp