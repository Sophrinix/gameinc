#pragma once
#include "nrpConfig.h"
#include "timeHelpers.h"

namespace nrp
{
OPTION_NAME HISTORY_SIZE( "historySize" );

class CNrpHistoryStep : public INrpConfig
{
	CNrpHistoryStep();
public:
	CNrpHistoryStep( const NrpTime& time );

	template< class T >
	void AddValue( const NrpText& name, T amount )
	{
		if( !IsExist( name ) )
			Add<T>( name, amount );
		else
			_self[ name ].As<T>() += amount;
	}
};

typedef std::map< int, CNrpHistoryStep* > HISTORY_MAP;

class CNrpHistory :	public INrpConfig
{
public:
	NrpText Save( const NrpText& fileName );
	void Load( const NrpText& fileName );

	CNrpHistoryStep* GetStep( NrpTime time );
	CNrpHistoryStep* GetLast();
	CNrpHistoryStep* GetFirst();

	CNrpHistoryStep* AddStep( NrpTime time );
	int GetSummFor( const OPTION_NAME& name, NrpTime time );

	CNrpHistory( const NrpText& fileName );
	CNrpHistory();
	~CNrpHistory(void);
private:
	void _InitialyzeOptions();

	HISTORY_MAP _steps;
};

}//end namespace nrp