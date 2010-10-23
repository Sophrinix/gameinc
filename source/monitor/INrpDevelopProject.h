#pragma once
#include "INrpProject.h"

namespace nrp
{
CLASS_NAME CLASS_INRPDEVELOPPROJECT( "INrpDevelopProject" );

OPTION_NAME MODULE_NUMBER( "moduleNumber" );

class CNrpProjectModule;
class IUser;

typedef std::vector< CNrpProjectModule* > MODULE_LIST;
typedef std::vector< std::string > DEVELOPERS_LIST;

class INrpDevelopProject : public INrpProject
{
	INrpDevelopProject();

public:
	INrpDevelopProject( const std::string& className, const std::string& systemName );
	 
	~INrpDevelopProject(void);

	void SetDeveloper( IUser* user );
	const DEVELOPERS_LIST& GetDevelopers() { return developers_; }
	virtual void ModuleFinished( CNrpProjectModule* module ) = 0;
	virtual bool IsReady() = 0; 
	virtual CNrpProjectModule* GetModule( size_t index ) = 0;
	virtual CNrpProjectModule* GetModule( const char* name ) = 0;

	virtual std::string Save( const std::string& amount );
	virtual void Load( const std::string& pathTo );

	static std::string ClassName() { return CLASS_INRPDEVELOPPROJECT; }
protected:
	void InitializeOptions_();

	DEVELOPERS_LIST developers_;
private:
};

}//end namespace nrp