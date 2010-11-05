#include "StdAfx.h"
#include "NrpHistory.h"
#include "timeHelpers.h"

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

CNrpHistoryStep* CNrpHistory::AddStep( SYSTEMTIME& time )
{
	int ret = TimeHelper::DateToInt( time );

	SALE_HISTORY_MAP::const_iterator pIter = _steps.find( ret );
	
	if( pIter == _steps.end() )
	{
		CNrpHistoryStep* step = new CNrpHistoryStep( time );
		_steps[ ret ] = step;
		return step;
	}
	else
		return pIter->second;
}

CNrpHistoryStep::CNrpHistoryStep( const SYSTEMTIME& time ) : INrpConfig( CLASS_HISTORYSTEP, "" )
{
	CreateValue<int>( BOXNUMBER, 0 );
}

}//end namespace  nrp