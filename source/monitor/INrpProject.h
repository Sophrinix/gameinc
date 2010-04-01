#pragma once
#include "nrpConfig.h"

namespace nrp
{

const std::string PRJTYPE_NAME[] = { "unknown", "gameengine", "game", 
								     "genre", "scenario", "license",
									 "scripts", "video" };

const std::string GENRE_NAME[] = { "unknown", "skill", "scrolling" };

typedef enum { SKL_UNKNOWN=100, SKL_VIDEO_TECH, SKL_CODING, SKL_SOUND, SKL_COUNT } SKILL_TYPE;
typedef enum { GT_UNKNOWN=300, GT_SKILL, GT_SCROLLING, GT_COUNT  } GENRE_TYPE;

typedef enum { SCRLVL_UNKNOWN=600, SCRLVL_SIMPLE, SCRLVL_COUNT } SCRIPT_LEVEL;
typedef enum { VDT_UNKNOWN=700, VDT_TEXT_OUTPUT, VDT_COUNT } VIDEO_TECH_TYPE;
typedef enum { SND_UNKNOWN=800, SND_SPEAKER_SOUND, SND_COUNT } SOUND_TECH_TYPE;

typedef enum { ADV_UNKNOWN=1100, ADV_MEMORY, ADV_COUNT } ADV_TECH_TYPE;
typedef enum { VDQ_UNKNOWN=1200, VDQ_SELFRENDER, VDQ_LOWQPIC, VDQ_INETART, VDQ_FSIZEART, VDQ_HQPIC, VDQ_COUNT } VIDEO_QUALITY;

typedef enum { PT_UNKNOWN=0, 
			   PT_GAME, PT_GAMEENGINE, PT_GENRE, 
			   PT_SCENARIO, PT_LICENSE, PT_SCRIPTS, 
			   PT_VIDEOTECH, PT_SOUNDTECH, PT_MINIGAME,
			   PT_PHYSIC, PT_ADVTECH, PT_VIDEOQUALITY,
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