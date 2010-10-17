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
	INrpDevelopProject( std::string className, std::string systemName )
		: INrpProject( className, systemName )
	{
	}
	~INrpDevelopProject(void);

	void SetDeveloper( IUser* user );
	const DEVELOPERS_LIST& GetDevelopers() { return developers_; }
	virtual void ModuleFinished( CNrpProjectModule* module, IUser* ptrUser ) = 0;
	virtual bool IsReady() = 0; 
	virtual CNrpProjectModule* GetModule( size_t index ) = 0;
	virtual CNrpProjectModule* GetModule( const char* name ) = 0;
	virtual void Save( std::string amount );

	static std::string ClassName() { return CLASS_INRPDEVELOPPROJECT; }
protected:
	void InitializeOptions_();

	DEVELOPERS_LIST developers_;
private:
	void Save( std::string scetionName, std::string fileName ) 
	{
		throw "deprecated function";
	};
};

}//end namespace nrp