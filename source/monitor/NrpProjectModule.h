#pragma once
#include "IWorkingModule.h"

namespace nrp
{

CLASS_NAME CLASS_PROJECTMODULE( "CNrpProjectModule" );
class INrpDevelopProject;

class CNrpProjectModule : public IWorkingModule
{
public:
	typedef std::vector< IUser* > USER_LIST;

	CNrpProjectModule( CNrpTechnology* pTech, INrpProject* pProject  );
	CNrpProjectModule( PROJECT_TYPE type, INrpDevelopProject* pProject );
	virtual ~CNrpProjectModule(void);

	int AddUser( IUser* ptrUser );
	int RemoveUser( const std::string& userName );
	const USER_LIST& GetUsers() const { return _users; }

	void Update( IUser* ptrUser );

	std::string Save( const std::string& saveFolder );
	void Load( const std::string& fileName );

	static std::string ClassName() { return CLASS_PROJECTMODULE; }

private:
	void InitializeOptions_();
	CNrpProjectModule() : IWorkingModule( PROJECT_TYPE(0), CLASS_PROJECTMODULE ) {};
	float _GetWorkKoeffForUser( IUser* ptrUser );

	USER_LIST _users;
};

typedef CNrpProjectModule* PNrpProjectModule;

}//end namespace nrp