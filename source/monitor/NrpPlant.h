#pragma once

#include "nrpConfig.h"

namespace nrp
{

CLASS_NAME CLASS_NRPPLANT( "CNrpPlant" );
class CNrpPlantWork;
class CNrpReklameWork;
class CNrpDiskMachine;

OPTION_NAME BASEREKLAMENUMBER( "baseReklameNumber" );
OPTION_NAME REKLAMENUMBER( "reklameNumber" );
OPTION_NAME DISKMACHINENUMBER( "diskMachineNumber" );

class CNrpPlant : public INrpConfig
{
	typedef std::vector< CNrpPlantWork* > WORK_LIST;
	typedef std::vector< CNrpReklameWork* > REKLAME_LIST;
	typedef std::vector< CNrpDiskMachine* > DISKMACHINES_LIST;
	CNrpPlant(void);
	~CNrpPlant(void);

	WORK_LIST _works;
	REKLAME_LIST baseReklame_;
	DISKMACHINES_LIST diskMachines_;
	REKLAME_LIST _reklameWorks;
public:
	static CNrpPlant& Instance();

	bool AddBaseReklame( CNrpReklameWork* pReklame );
	CNrpReklameWork* GetBaseReklame( const std::string& name );
	CNrpReklameWork* GetBaseReklame( int index );

	void AddReklame( CNrpReklameWork* reklame );
	CNrpReklameWork* GetReklame( const std::string& type, 
								 const std::string& gameName );
	void RemoveReklame( std::string type, std::string gameName );
	CNrpReklameWork* CreateReklame( const std::string& type, 
									const std::string& gameName, 
									const std::string& company );

	void AddWork( CNrpPlantWork* work );
	CNrpPlantWork* GetWork( int index );
	CNrpPlantWork* GetWork( std::string name );
	void RemoveWork( CNrpPlantWork* work );

	CNrpDiskMachine* GetDiskMachine( const std::string& name );
	CNrpDiskMachine* GetDiskMachine( size_t index );
	void AddDiskMachine( CNrpDiskMachine* pDm );

	void BeginNewDay( );

	void Load( const std::string& saveFolder );
	std::string Save( const std::string& saveFolder );

	static std::string ClassName() { return CLASS_NRPPLANT; }
};

}//end namespace nrp