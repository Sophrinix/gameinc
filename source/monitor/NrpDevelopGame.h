#pragma once
#include "INrpDevelopProject.h"

const std::string CLASS_DEVELOPGAME("CNrpDevelopGame");

namespace nrp
{

class CNrpProjectModule;
class CNrpGameProject;
class CNrpCompany;
class CNrpScenario;
class CNrpLicense;

class CNrpDevelopGame : public INrpDevelopProject
{
	typedef std::vector< CNrpProjectModule* > MODULE_LIST;
	CNrpDevelopGame() : INrpDevelopProject( CLASS_DEVELOPGAME, "" ) {}; 
public:
	CNrpDevelopGame( std::string name, CNrpCompany* ptrCompany );
	CNrpDevelopGame( CNrpGameProject* nProject, CNrpCompany* ptrCompany );

	void Save( std::string folderSave );
	void Load( std::string loadFolder );
	CNrpProjectModule* GetGenre( size_t index );
	CNrpProjectModule* GetModule( size_t index );

	void CalculateCodeVolume();
	bool IsReady();

	~CNrpDevelopGame(void);
private:
	MODULE_LIST gameModules_;

	void InitializeOptions_( const std::string& name );
	void ModuleFinished( CNrpProjectModule* module, IUser* ptrUser );
	void FindPlaformsAndLanguages_();
};

}//end namespace nrp