#pragma once
#include "nrpConfig.h"

namespace nrp
{
class CNrpGame;
class CNrpGameBoxAddon;

OPTION_NAME BOXNUMBER( "boxNumber" );
OPTION_NAME BOXGAME( "boxGame" );

class CNrpGameBox : public INrpConfig
{
public:
	CNrpGameBox( CNrpGame* ptrGame );
	~CNrpGameBox(void);
private:
	typedef std::vector< CNrpGameBoxAddon* > ADDON_LIST;
	
	ADDON_LIST addons_;
};

}//end of namespace nrp