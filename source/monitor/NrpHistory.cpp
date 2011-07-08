#include "StdAfx.h"
#include "NrpHistory.h"
#include "NrpTime.h"
#include "IniFile.h"
#include "NrpConfigSyncer.h"

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
	RegProperty<int>( HISTORY_SIZE, 0 );
}

CNrpHistory::CNrpHistory( const NrpText& fileName ) : INrpConfig( CLASS_NRPHISTORY, "" )
{
	_InitialyzeOptions();
	Load( fileName );
}

CNrpHistoryStep* CNrpHistory::GetStep( NrpTime time )
{
	HISTORY_MAP::iterator pIter = _steps.find( static_cast< int >( time.GetDate().ToDouble() ) );
	return pIter != _steps.end() ? pIter->second : NULL;
}

NrpText CNrpHistory::Save( const NrpText& fileName )
{
	INrpConfig::Save( fileName );
	IniFile sf( fileName );

	HISTORY_MAP::iterator pIter = _steps.begin();

	for( int index=0; pIter != _steps.end(); pIter++, index++ )
	{
        CNrpConfigSyncer svr( pIter->second );
        svr.Save( sf, CreateSectionStep( index ) );
	}

	sf.Save();
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
        NrpTime time = lf.Get( section, STARTDATE + ":" + CNrpConfigSyncer::t_time, NrpTime( 0. ) );

		CNrpHistoryStep* step = AddStep( time );
		assert( step && "step must be exist on load" );
        if( step )
        {
            CNrpConfigSyncer ldr( step );
            ldr.Load( lf, section );
        }
	}
}

CNrpHistoryStep* CNrpHistory::AddStep( NrpTime time )
{
	s32 ret = floor32( (float)time.GetDate().ToDouble() );
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

int CNrpHistory::GetSummFor( const OPTION_NAME& name, NrpTime time )
{
	int summ = 0;
	if( _steps.size() > 0 )
	{
		HISTORY_MAP::iterator pIter = _steps.begin();
		while( pIter != _steps.end() )
		{	
			NrpTime stepTime = (*pIter->second)[ STARTDATE ];

			if( stepTime.Equale( time ) == -1 )
				summ += (int)(*pIter->second)[ name ];

			pIter++;
		}
	}

	return summ;
}

CNrpHistoryStep::CNrpHistoryStep( const NrpTime& time ) : INrpConfig( CLASS_HISTORYSTEP, "" )
{
	RegProperty<NrpTime>( STARTDATE, time );
	RegProperty<int>( BOXNUMBER, 0 );
	RegProperty<int>( BALANCE, 0 );
}

CNrpHistoryStep::CNrpHistoryStep() : INrpConfig( CLASS_HISTORYSTEP, "" )
{

}

}//end namespace  nrp