#pragma once
#include "nrpConfig.h"

namespace nrp
{
class CNrpGame;
class CNrpTechnology;

OPTION_NAME GAME( "game" );
OPTION_NAME NUMBERADDON( "numberAddon" );
OPTION_NAME BOXNUMBER( "boxNumber" );

class CNrpGameBox : public INrpConfig
{
public:
	CNrpGameBox( CNrpGame* ptrGame );
	bool IsMyBoxAddon( std::string name );
	void RemoveMyBoxAddon( std::string name );
	void AddBoxAddon( CNrpTechnology* tech );
	CNrpTechnology* GetAddon( size_t index );
	float GetBoxAddonsPrice();

	~CNrpGameBox(void);
	void Save( std::string scetionName, std::string fileName );
	void Load( std::string sectionName, std::string fileName );
private:
	void Load_( char* file_name ) {}
	typedef std::vector< CNrpTechnology* > ADDON_LIST;
	typedef ADDON_LIST::iterator ADDON_LIST_ITERATOR;

	ADDON_LIST_ITERATOR FindAddon_( const std::string& name );
	ADDON_LIST addons_;
};

typedef CNrpGameBox* PNrpGameBox;

}//end of namespace nrp