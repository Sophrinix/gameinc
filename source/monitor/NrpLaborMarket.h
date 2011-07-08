#ifndef _INCLUDE_LABORMARKET_
#define _INCLUDE_LABORMARKET_

#include "NrpText.h"
#include "nrpArrays.h"
#include "nrpConfig.h"
#include "LuaFunctionality.h"

namespace nrp
{

class CNrpUser;

class CNrpLaborMarket : public INrpConfig, public ILuaFunctionality
{
public:
	static CNrpLaborMarket& Instance();
    static const NrpText saveTemplate;
	CNrpLaborMarket(void);
	~CNrpLaborMarket(void);

	CNrpUser* CreateRandomUser( NrpText userType );
	void CreateNewFreeUsers();

	int AddUser( CNrpUser* user );
	int RemoveUser( CNrpUser* user );

	CNrpUser* GetUser( u32 index );
	CNrpUser* GetUser( const NrpText& name );

	NrpText Save( const NrpText& folderName );
	void Load( const NrpText& folderName );

	void Reset();

private:
	int _GetTechsByGroup( int type, TECHS& arrayt );
	void _LoadUsers( const NrpText& fileName );
	USERS _users;
};

#define _nrpLaborMarkt CNrpLaborMarket::Instance()

}//end namespace nrp

#endif
