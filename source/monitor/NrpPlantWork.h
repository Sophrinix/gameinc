#pragma once
#include "nrpConfig.h"

namespace nrp
{

CLASS_NAME CLASS_NRPPLANTWORK( "CNrpPlantWork" );

OPTION_NAME PRODUCETYPE( "produceType" );
OPTION_NAME NUMBERMACHINE( "numberMachine" );
OPTION_NAME DISKNUMBER( "diskNumber" );
OPTION_NAME DISKPRICE( "diskPrice" );
OPTION_NAME DISKINDAY( "diskInDay" );
OPTION_NAME FINALPRICE( "finalPrice" );
OPTION_NAME DISKMACHINENAME( "diskMachineName" );
OPTION_NAME LEFTPRODUCEDISK( "leftProduceDisk" );

class CNrpPlantWork : public INrpConfig
{
	void CalcParams_();
	void InitializeOptions_();

public:
	CNrpPlantWork( const std::string& companyName );
	CNrpPlantWork( const std::string& fileName, bool load );
	CNrpPlantWork( const CNrpPlantWork& p );
	~CNrpPlantWork(void);

	template< class R > void SetValue( std::string name, R valuel )
	{
		INrpConfig::SetValue<R>( name, valuel );
		CalcParams_();
	}

	virtual std::string Save( const std::string& folder );
	virtual void Load( const std::string& fileName );
	void BeginNewDay();

	static std::string ClassName() { return CLASS_NRPPLANTWORK; }
};

}//end of namespace nrp