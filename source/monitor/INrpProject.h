#pragma once
#include "nrpConfig.h"
#include "NrpTypes.h"

namespace nrp
{

typedef enum { SCRLVL_UNKNOWN=600, SCRLVL_SIMPLE, SCRLVL_COUNT } SCRIPT_LEVEL;
typedef enum { VDT_UNKNOWN=700, VDT_TEXT_OUTPUT, VDT_COUNT } VIDEO_TECH_TYPE;
typedef enum { SND_UNKNOWN=800, SND_SPEAKER_SOUND, SND_COUNT } SOUND_TECH_TYPE;
typedef enum { MNGM_UNKNOWN=900, MNGM_TEXTQUEST, MNGM_COUNT } MINIGAME_TECH_TYPE;
typedef enum { PHYS_UNKNOWN=1000, PHYS_2DCONCLUSION, PHYS_COUNT } PHYS_TECH_TYPE;
typedef enum { ADV_UNKNOWN=1100, ADV_MEMORY, ADV_JOYSTIK, ADV_HELP, ADV_COUNT } ADV_TECH_TYPE;
typedef enum { VDQ_UNKNOWN=1200, VDQ_SELFRENDER, VDQ_INETART, VDQ_FREELANCE, VDQ_STUDIO, VDQ_UNIQUE, VDQ_COUNT } VIDEO_QUALITY;
typedef enum { SDQ_UNKNOWN=1300, SGQ_SELFCOMPOSE, SDQ_COUNT } SOUND_QUALITY;
typedef enum { SCNQ_UNKNOWN=1400, SCNQ_SELFWRITE, SCNQ_COUNT } SCENARIO_QUALITY;

class INrpProject : public INrpConfig
{
	INrpProject() : INrpConfig( "error", "error" ) {}

public:
	INrpProject( const NrpText& className, const NrpText& systemName);

	virtual NrpText Save( const NrpText& fileName );

	bool Equale( const NrpText& name );
	~INrpProject(void);
};

typedef INrpProject* PNrpProject;

}//namespace nrp