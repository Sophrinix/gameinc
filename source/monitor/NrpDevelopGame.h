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
    static const NrpText saveTemplate;
    static const NrpText extNameTemplate;

	CNrpDevelopGame( const NrpText& name, CNrpCompany* ptrCompany );
	CNrpDevelopGame( CNrpGameProject& nProject, CNrpCompany& ptrCompany );
	CNrpDevelopGame( const NrpText& fileName );

	NrpText Save( const NrpText& folderSave );
	void Load( const NrpText& loadFolder );
	CNrpProjectModule* GetGenre( u32 index ) const;
	CNrpProjectModule* GetModule( u32 index ) const;
	CNrpProjectModule* GetModule( const NrpText& name ) const;

	void CalculateCodeVolume();
	bool IsReady();

	~CNrpDevelopGame(void);

	void ModuleFinished( IWorkingModule& module );
	void ModuleTested( IWorkingModule& module );

	static NrpText ClassName();
private:
	MODULES _modules;

	void _InitializeOptions( const NrpText& name );
	void _FindPlaformsAndLanguages();

	template< typename T, typename TARRAY >
	void _AddModulesFrom( const TARRAY& arrtech, int baseCode );
	
	void _AddModulesFrom( const CNrpTechnology& tech, int baseCode );
};

}//end namespace nrp