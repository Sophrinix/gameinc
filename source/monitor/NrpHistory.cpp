#include "StdAfx.h"
#include "NrpHistory.h"
#include "timeHelpers.h"

namespace nrp
{
CLASS_NAME CLASS_NRPHISTORY( "CNrpHistory" );
CLASS_NAME CLASS_HISTORYSTEP( "CNrpHistoryStep" );

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

NrpText CNrpHistory::Save( const NrpText& fileName )
{
	return "";
}

void CNrpHistory::Load( const NrpText& fileName )
{

}

CNrpHistoryStep* CNrpHistory::AddStep( SYSTEMTIME& time )
{
	int ret = TimeHelper::DateToInt( time );

	SALE_HISTORY_MAP::Iterator pIter = _steps.find( ret );
	
	if( pIter.atEnd() )
	{
		CNrpHistoryStep* step = new CNrpHistoryStep( time );
		_steps[ ret ] = step;
		return step;
	}
	else
		return pIter->getValue();
}

CNrpHistoryStep::CNrpHistoryStep( const SYSTEMTIME& time ) : INrpConfig( CLASS_HISTORYSTEP, "" )
{
	CreateValue<int>( BOXNUMBER, 0 );
}

CNrpHistoryStep::CNrpHistoryStep() : INrpConfig( CLASS_HISTORYSTEP, "" )
{

}

}//end namespace  nrp