#pragma once
#include "nrpConfig.h"

namespace nrp
{

CLASS_NAME CLASS_NRPHISTORY( "CNrpHistory" );
CLASS_NAME CLASS_HISTORYSTEP( "CNrpHistoryStep" );

OPTION_NAME HISTORY_SIZE( "historySize" );

class CNrpHistoryStep : public INrpConfig
{
	CNrpHistoryStep() : INrpConfig( CLASS_HISTORYSTEP, "" ) {}
public:
	CNrpHistoryStep( const SYSTEMTIME& time );
};

class CNrpHistory :	public INrpConfig
{
public:
	std::string Save( const std::string& fileName );
	void Load( const std::string& fileName );

	CNrpHistoryStep* GetStep( const SYSTEMTIME& time, bool aprocimateData );
	CNrpHistoryStep* GetStep( const SYSTEMTIME& time );

	CNrpHistory();
	~CNrpHistory(void);
private:

	typedef std::map< int, CNrpHistoryStep* > SALE_HISTORY_MAP;
	SALE_HISTORY_MAP _steps;
};

}//end namespace nrp