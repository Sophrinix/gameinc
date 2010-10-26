#pragma once

#include "nrpConfig.h"

namespace nrp
{

CLASS_NAME CLASS_NRPPLANT( "CNrpPlant" );
class CNrpPlantWork;
class CNrpReklameWork;

OPTION_NAME BASEREKLAMENUMBER( "baseReklameNumber" );
OPTION_NAME REKLAMENUMBER( "reklameNumber" );

class CNrpPlant : public INrpConfig
{
	typedef std::vector< CNrpPlantWork* > WORK_LIST;
	typedef std::vector< CNrpReklameWork* > REKLAME_LIST;
	CNrpPlant(void);
	~CNrpPlant(void);

	WORK_LIST works_;
	REKLAME_LIST baseReklame_;
	REKLAME_LIST reklameWorks_;
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

	void BeginNewDay( );

	void Load( const std::string& saveFolder );
	std::string Save( const std::string& saveFolder );

	static std::string ClassName() { return CLASS_NRPPLANT; }
};

}//end namespace nrp