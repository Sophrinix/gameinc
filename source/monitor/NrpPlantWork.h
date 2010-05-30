#pragma once
#include "nrpConfig.h"

namespace nrp
{

OPTION_NAME CLASS_NRPPLANTWORK( "CNrpPlantWork" );

OPTION_NAME PRODUCETYPE( "produceType" );
OPTION_NAME NUMBERMACHINE( "numberMachine" );
OPTION_NAME DISKNUMBER( "diskNumber" );
OPTION_NAME DISKPRICE( "diskPrice" );
OPTION_NAME DISKINDAY( "diskInDay" );
OPTION_NAME FINALPRICE( "finalPrice" );
OPTION_NAME DISKMACHINENAME( "diskMachineName" );
OPTION_NAME LEFTPRODUCEDISK( "leftProduceDisk" );
OPTION_NAME FINISHED( "finished" );

class CNrpPlantWork : public INrpConfig
{
	void Load_( char* file_name ) {};
	void CalcParams_();
	void InitializeOptions_();

public:
	CNrpPlantWork( std::string companyName );
	CNrpPlantWork( const CNrpPlantWork& p );
	~CNrpPlantWork(void);

	template<class R > void SetValue( std::string name, R valuel )
	{
		INrpConfig::SetValue<R>( name, valuel );
		CalcParams_();
	}

	void Save( std::string sectionName, std::string fileName );
	void Load( std::string sectionName, std::string fileName );
	void BeginNewDay();
};

}//end of namespace nrp