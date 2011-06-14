#ifndef _INCLUDE_NRPQUEST_
#define _INCLUDE_NRPQUEST_

#include "nrpConfig.h"
#include "nrpArrays.h"
#include "OptionNames.h"
#include "LuaFunctionality.h"

namespace nrp
{
OPTION_NAME POSX( L"posx" );
OPTION_NAME POSY( L"posy" );
OPTION_NAME STARTFUNCREF( L"startfuncref" );
OPTION_NAME OBSOLETE( L"obsolete" );
OPTION_NAME ACTIVE( L"active" );

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