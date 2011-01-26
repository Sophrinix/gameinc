#include "StdAfx.h"
#include "NrpHistory.h"
#include "timeHelpers.h"
#include "IniFile.h"

static const NrpText KEY_SALE( L"sale" );
static const NrpText KEY_PROFIT( L"profit" );
static const NrpText KEY_TIME( L"time" );

namespace nrp
{
CLASS_NAME CLASS_NRPHISTORY( "CNrpHistory" );
CLASS_NAME CLASS_HISTORYSTEP( "CNrpHistoryStep" );

CNrpHistory::~CNrpHistory(void)
{
}

CNrpHistory::CNrpHistory() : INrpConfig( CLASS_NRPHISTORY, "" )
{
	_InitialyzeOptions();
}

void CNrpHistory::_InitialyzeOptions()
{
	Add<int>( HISTORY_SIZE, 0 );
}

CNrpHistory::CNrpHistory( const NrpText& fileName ) : INrpConfig( CLASS_NRPHISTORY, "" )
{
	_InitialyzeOptions();
	Load( fileName );
}

CNrpHistoryStep* CNrpHistory::GetStep( const SYSTEMTIME& time )
{
	int ret = TimeHelper::DateToInt( const_cast< SYSTEMTIME& >( time ) );

	HISTORY_MAP::iterator pIter = _steps.find( ret );

	return pIter != _steps.end() ? pIter->second : NULL;
}

NrpText CNrpHistory::Save( const NrpText& fileName )
{
	INrpConfig::Save( fileName );
	IniFile sf( fileName );

	HISTORY_MAP::iterator pIter = _steps.begin();

	for( int index=0; pIter != _steps.end(); pIter++, index++ )
	{
		NrpText section = CreateSectionStep( index );
		sf.Set( section, KEY_TIME, (int)pIter->first );
		sf.Set( section, KEY_SALE, (int)pIter->second->Param( BOXNUMBER ) );
		sf.Set( section, KEY_PROFIT, (int)pIter->second->Param( BALANCE ) );
	}

	return fileName;
}

void CNrpHistory::Load( const NrpText& fileName )
{
	INrpConfig::Load( fileName );
	IniFile lf( fileName );

	int size = _self[ HISTORY_SIZE ];
	for( int index=0; index < size; index++ )
	{
		NrpText section = CreateSectionStep( index );
		SYSTEMTIME time = lf.Get( section, KEY_TIME, SYSTEMTIME() );

		CNrpHistoryStep* step = AddStep( time );
		assert( step );
		(*step)[ BALANCE ] =  lf.Get( section, KEY_PROFIT, 0 );
		(*step)[ BOXNUMBER ] = lf.Get( section, KEY_SALE, 0 );
	}
}

CNrpHistoryStep* CNrpHistory::AddStep( const SYSTEMTIME& time )
{
	int ret = TimeHelper::DateToInt( const_cast< SYSTEMTIME& >( time ) );

	HISTORY_MAP::iterator pIter = _steps.find( ret );
	
	if( pIter == _steps.end() )
	{
		CNrpHistoryStep* step = new CNrpHistoryStep( time );
		_steps[ ret ] = step;
		_self[ HISTORY_SIZE ] = static_cast< int >( _steps.size() );
		return step;
	}
	else
		return pIter->second;
}

CNrpHistoryStep* CNrpHistory::GetLast()
{	
	return _steps.size() > 0 ? _steps.rbegin()->second : NULL;
}

CNrpHistoryStep* CNrpHistory::GetFirst()
{
	return _steps.size() > 0 ? _steps.begin()->second : NULL;
}

int CNrpHistory::GetSummFor( const OPTION_NAME& name, const SYSTEMTIME& time )
{
	int summ = 0;
	if( _steps.size() > 0 )
	{
		HISTORY_MAP::iterator pIter = _steps.begin();
		while( pIter != _steps.end() && 
			TimeHelper::TimeCmp( pIter->second->Param( STARTDATE ), time ) == -1 )
		{	
			 summ += (int)(pIter->second->Param( name ));
			 pIter++;
		}
	}

	return summ;
}

CNrpHistoryStep::CNrpHistoryStep( const SYSTEMTIME& time ) : INrpConfig( CLASS_HISTORYSTEP, "" )
{
	Add<SYSTEMTIME>( STARTDATE, time );
	Add<int>( BOXNUMBER, 0 );
	Add<int>( BALANCE, 0 );
}

CNrpHistoryStep::CNrpHistoryStep() : INrpConfig( CLASS_HISTORYSTEP, "" )
{

}

}//end namespace  nrp