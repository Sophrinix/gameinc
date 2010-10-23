#include "StdAfx.h"
#include "NrpHistory.h"

namespace nrp
{

CNrpHistory::~CNrpHistory(void)
{
}

CNrpHistory::CNrpHistory() : INrpConfig( CLASS_NRPHISTORY, "" )
{
	CreateValue<int>( HISTORY_SIZE, 0 );
}

CNrpHistoryStep* CNrpHistory::GetStep( const SYSTEMTIME& time, bool aprocimateData )
{
	return NULL;
}

CNrpHistoryStep* CNrpHistory::GetStep( const SYSTEMTIME& time )
{
	return NULL;
}

std::string CNrpHistory::Save( const std::string& fileName )
{
	return "";
}

void CNrpHistory::Load( const std::string& fileName )
{

}


CNrpHistoryStep::CNrpHistoryStep( const SYSTEMTIME& time ) : INrpConfig( CLASS_HISTORYSTEP, "" )
{
	CreateValue<int>( BOXNUMBER, 0 );
}

}//end namespace  nrp