#pragma once
#include "IWorkingModule.h"

namespace nrp
{

const CLASS_NAME CLASS_PROJECTMODULE( "CNrpProjectModule" );
class INrpProject;

class CNrpProjectModule : public IWorkingModule
{
public:
	CNrpProjectModule( CNrpTechnology* pTech, INrpProject* pProject  );
	CNrpProjectModule( PROJECT_TYPE type, INrpProject* pProject );
	virtual ~CNrpProjectModule(void);

	int AddUser( IUser* ptrUser );
	int RemoveUser( const std::string& userName );
	void Update( IUser* ptrUser );

	void Save( std::string saveFolder );
	void Load( std::string fileName );

private:
	void InitializeOptions_();
	CNrpProjectModule() : IWorkingModule( PROJECT_TYPE(0), CLASS_PROJECTMODULE ) {};

	typedef std::vector< IUser* > USER_LIST;
	USER_LIST users_;
};

typedef CNrpProjectModule* PNrpProjectModule;

}//end namespace nrp