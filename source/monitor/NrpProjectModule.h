#pragma once
#include "IWorkingModule.h"

namespace nrp
{
class INrpDevelopProject;

class CNrpProjectModule : public IWorkingModule
{
public:
	CNrpProjectModule( CNrpTechnology* pTech, INrpProject* pProject  );
	CNrpProjectModule( CNrpPlatform* platform, INrpProject* project );
	CNrpProjectModule( PROJECT_TYPE type, INrpDevelopProject* pProject );
	virtual ~CNrpProjectModule(void);

	int AddUser( IUser* ptrUser );
	int RemoveUser( const NrpText& userName );
	const USERS& GetUsers() const { return _users; }

	void Update( IUser* ptrUser );

	NrpText Save( const NrpText& saveFolder );
	void Load( const NrpText& fileName );

	static NrpText ClassName();

private:
	void InitializeOptions_();
	CNrpProjectModule();
	float _GetWorkKoeffForUser( IUser* ptrUser );

	USERS _users;
};

typedef CNrpProjectModule* PNrpProjectModule;

}//end namespace nrp