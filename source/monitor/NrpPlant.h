#pragma once

#include "nrpConfig.h"
#include "nrpArrays.h"

namespace nrp
{
class CNrpPlantWork;
class CNrpReklameWork;
class CNrpDiskMachine;

OPTION_NAME BASEREKLAMENUMBER( L"baseReklameNumber" );
OPTION_NAME REKLAMENUMBER( L"reklameNumber" );
OPTION_NAME DISKMACHINENUMBER( L"diskMachineNumber" );

class CNrpPlant : public INrpConfig
{
	typedef array< CNrpPlantWork* > PLANTWORKS;

	CNrpPlant(void);
	~CNrpPlant(void);

	PLANTWORKS _works;
	REKLAMES _baseReklame;
	DISKMACHINES _diskMachines;
	REKLAMES _reklameWorks;
public:
	static CNrpPlant& Instance();

	bool AddBaseReklame( CNrpReklameWork* pReklame );
	CNrpReklameWork* GetBaseReklame( const NrpText& name );
	CNrpReklameWork* GetBaseReklame( int index );

	void AddReklame( CNrpReklameWork& reklame );
	CNrpReklameWork* GetReklame( const NrpText& type, 
								 const NrpText& gameName );
	CNrpReklameWork* GetReklame( u32 index );

	void RemoveReklame( NrpText type, const NrpText& gameName );
	CNrpReklameWork* CreateReklame( const NrpText& type, 
									const NrpText& gameName, 
									const NrpText& company );

	void AddWork( CNrpPlantWork* work );
	CNrpPlantWork* GetWork( u32 index );
	void RemoveWork( CNrpPlantWork* work );

	CNrpDiskMachine* GetDiskMachine( const NrpText& name );
	CNrpDiskMachine* GetDiskMachine( size_t index );
	void AddDiskMachine( CNrpDiskMachine* pDm );

	void BeginNewDay( );

	void Load( const NrpText& saveFolder );
	NrpText Save( const NrpText& saveFolder );

	static NrpText ClassName();
};

}//end namespace nrp