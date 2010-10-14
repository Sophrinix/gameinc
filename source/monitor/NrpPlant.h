#pragma once

#include "nrpConfig.h"

namespace nrp
{

const std::string CLASS_NRPPLANT( "CNrpPlant" );
class CNrpPlantWork;
class CNrpReklameWork;

OPTION_NAME WORKNUMBER( "workNumber" );
OPTION_NAME BASEREKLAMENUMBER( "baseReklameNumber" );
OPTION_NAME REKLAMENUMBER( "reklameNumber" );

class CNrpPlant : public INrpConfig
{
	typedef std::vector< CNrpPlantWork* > WORK_LIST;
	typedef std::vector< CNrpReklameWork* > REKLAME_LIST;
	CNrpPlant(void);
	~CNrpPlant(void);

	void Load_( char* file_name ) {}

	WORK_LIST works_;
	REKLAME_LIST baseReklame_;
	REKLAME_LIST reklameWorks_;
public:
	static CNrpPlant& Instance();

	bool AddBaseReklame( CNrpReklameWork* pReklame );
	CNrpReklameWork* GetBaseReklame( std::string name );
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

	void Load( std::string scetionName, std::string saveFolder );
	void Save( std::string scetionName, std::string saveFolder );
};

}//end namespace nrp