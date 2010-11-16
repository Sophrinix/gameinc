#pragma once
#include "nrpConfig.h"

namespace nrp
{
using irr::core::array;

class CNrpGame;
class CNrpTechnology;

typedef CNrpTechnology CNrpBoxAddon;

OPTION_NAME GAME( "game" );
OPTION_NAME NUMBERADDON( "numberAddon" );

class CNrpGameBox : public INrpConfig
{
public:
	CNrpGameBox( CNrpGame* ptrGame );
	bool IsMyAddon( const NrpText& name );
	void RemoveAddon( const NrpText& name );
	bool AddAddon( CNrpBoxAddon* tech );
	CNrpBoxAddon* GetAddon( size_t index );
	float GetBoxAddonsPrice();

	~CNrpGameBox(void);
	NrpText Save( const NrpText& fileName );
	void Load( const NrpText& fileName );

	static NrpText ClassName();
private:
	typedef array< CNrpBoxAddon* > ADDON_LIST;

	int _FindAddon( const NrpText& name );
	int _GetAddonSumLevel();
	ADDON_LIST _addons;
};

typedef CNrpGameBox* PNrpGameBox;

}//end of namespace nrp