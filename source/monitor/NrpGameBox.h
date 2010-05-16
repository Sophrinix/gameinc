#pragma once
#include "nrpConfig.h"

namespace nrp
{
class CNrpGame;
class CNrpTechnology;

OPTION_NAME BOXNUMBER( "boxNumber" );
OPTION_NAME BOXGAME( "boxGame" );

class CNrpGameBox : public INrpConfig
{
public:
	CNrpGameBox( CNrpGame* ptrGame );
	bool IsMyBoxAddon( std::string name );
	void RemoveMyBoxAddon( std::string name );
	void AddBoxAddon( CNrpTechnology* tech );

	~CNrpGameBox(void);
private:
	typedef std::vector< CNrpTechnology* > ADDON_LIST;
	typedef ADDON_LIST::iterator ADDON_LIST_ITERATOR;

	ADDON_LIST_ITERATOR FindAddon_( const std::string& name );
	ADDON_LIST addons_;
};

typedef CNrpGameBox* PNrpGameBox;

}//end of namespace nrp