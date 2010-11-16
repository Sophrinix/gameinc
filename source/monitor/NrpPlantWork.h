#pragma once
#include "nrpConfig.h"

namespace nrp
{
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
	CNrpPlantWork( const NrpText& companyName );
	CNrpPlantWork( const NrpText& fileName, bool load );
	CNrpPlantWork( const CNrpPlantWork& p );
	~CNrpPlantWork(void);

	template< class R > void SetValue( NrpText name, R valuel )
	{
		INrpConfig::SetValue<R>( name, valuel );
		CalcParams_();
	}

	virtual NrpText Save( const NrpText& folder );
	virtual void Load( const NrpText& fileName );
	void BeginNewDay();

	static NrpText ClassName();
};

}//end of namespace nrp