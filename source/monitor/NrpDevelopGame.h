#pragma once
#include "INrpDevelopProject.h"
#include "nrpArrays.h"

namespace nrp
{
class CNrpProjectModule;
class CNrpGameProject;
class CNrpCompany;
class CNrpScenario;
class CNrpLicense;

class CNrpDevelopGame : public INrpDevelopProject
{
	CNrpDevelopGame();

public:
	CNrpDevelopGame( const NrpText& name, CNrpCompany* ptrCompany );
	CNrpDevelopGame( CNrpGameProject* nProject, CNrpCompany* ptrCompany );
	CNrpDevelopGame( const NrpText& fileName );

	NrpText Save( const NrpText& folderSave );
	void Load( const NrpText& loadFolder );
	CNrpProjectModule* GetGenre( u32 index );
	CNrpProjectModule* GetModule( u32 index );
	CNrpProjectModule* GetModule( const NrpText& name );

	void CalculateCodeVolume();
	bool IsReady();

	~CNrpDevelopGame(void);

	static NrpText ClassName();
private:
	MODULES _modules;

	void _InitializeOptions( const NrpText& name );
	void ModuleFinished( CNrpProjectModule* module );
	void _FindPlaformsAndLanguages();

	template< typename T, typename TARRAY >
	void _AddModulesFrom( const TARRAY& arrtech, int baseCode );
	
	void _AddModulesFrom( const CNrpTechnology* tech, int baseCode );
};

}//end namespace nrp