#ifndef _INCLUDE_NRPQUEST_
#define _INCLUDE_NRPQUEST_

#include "nrpConfig.h"
#include "nrpArrays.h"
#include "OptionNames.h"
#include "LuaFunctionality.h"

namespace nrp
{
OPTION_NAME POSX( "posx" );
OPTION_NAME POSY( "posy" );
OPTION_NAME STARTFUNCREF( "startfuncref" );
OPTION_NAME OBSOLETE( "obsolete" );

class CNrpQuest : public INrpConfig, ILuaFunctionality
{
public:
	CNrpQuest(void);
	~CNrpQuest(void);

	void Start();

private:
	STRINGS _results;
};

}
#endif