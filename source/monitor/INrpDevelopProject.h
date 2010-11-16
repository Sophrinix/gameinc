#pragma once
#include "INrpProject.h"
#include "nrpArrays.h"

namespace nrp
{
OPTION_NAME MODULE_NUMBER( "moduleNumber" );

class CNrpProjectModule;
class IUser;

class INrpDevelopProject : public INrpProject
{
	INrpDevelopProject();

public:
	INrpDevelopProject( const NrpText& className, const NrpText& systemName );
	 
	~INrpDevelopProject(void);

	void SetDeveloper( IUser* user );
	const STRINGS& GetDevelopers() { return _developers; }
	virtual void ModuleFinished( CNrpProjectModule* module ) = 0;
	virtual bool IsReady() = 0; 
	virtual CNrpProjectModule* GetModule( u32 index ) = 0;
	virtual CNrpProjectModule* GetModule( const NrpText& name ) = 0;

	virtual NrpText Save( const NrpText& amount );
	virtual void Load( const NrpText& pathTo );

	static NrpText ClassName();
protected:
	void InitializeOptions_();

	STRINGS _developers;
private:
};

}//end namespace nrp