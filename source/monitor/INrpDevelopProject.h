#pragma once
#include "INrpProject.h"
#include "nrpArrays.h"

namespace nrp
{

class CNrpProjectModule;
class CNrpUser;

class INrpDevelopProject : public INrpProject
{
	INrpDevelopProject();

public:
	INrpDevelopProject( const NrpText& className, const NrpText& systemName );
	 
	~INrpDevelopProject(void);

	void SetDeveloper( CNrpUser& user );
	const STRINGS& GetDevelopers() { return _developers; }
	virtual void ModuleFinished( IWorkingModule& module ) = 0;
	virtual void ModuleTested( IWorkingModule& module ) = 0;
	virtual bool IsReady() = 0; 
	virtual CNrpProjectModule* GetModule( u32 index ) const = 0;
	virtual CNrpProjectModule* GetModule( const NrpText& name ) const= 0;

	virtual NrpText Save( const NrpText& amount );
	virtual void Load( const NrpText& pathTo );

	static NrpText ClassName();
protected:
	void InitializeOptions_();

	STRINGS _developers;
};

}//end namespace nrp