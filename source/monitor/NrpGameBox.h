#pragma once
#include "nrpConfig.h"

namespace nrp
{
class CNrpGame;
class CNrpTechnology;

typedef CNrpTechnology CNrpBoxAddon;

CLASS_NAME CLASS_GAMEBOX( "CNrpGameBox" );

OPTION_NAME GAME( "game" );
OPTION_NAME NUMBERADDON( "numberAddon" );

class CNrpGameBox : public INrpConfig
{
public:
	CNrpGameBox( CNrpGame* ptrGame );
	bool IsMyAddon( const std::string& name );
	void RemoveAddon( const std::string& name );
	bool AddAddon( CNrpBoxAddon* tech );
	CNrpBoxAddon* GetAddon( size_t index );
	float GetBoxAddonsPrice();

	~CNrpGameBox(void);
	std::string Save( const std::string& fileName );
	void Load( const std::string& fileName );

	static std::string ClassName() { return CLASS_GAMEBOX; }
private:
	typedef std::vector< CNrpBoxAddon* > ADDON_LIST;
	typedef ADDON_LIST::iterator ADDON_LIST_ITERATOR;

	ADDON_LIST_ITERATOR _FindAddon( const std::string& name );
	int _GetAddonSumLevel();
	ADDON_LIST _addons;
};

typedef CNrpGameBox* PNrpGameBox;

}//end of namespace nrp