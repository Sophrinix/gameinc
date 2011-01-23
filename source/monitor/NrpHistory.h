#pragma once
#include "nrpConfig.h"

namespace nrp
{
OPTION_NAME HISTORY_SIZE( "historySize" );

class CNrpHistoryStep : public INrpConfig
{
	CNrpHistoryStep();
public:
	CNrpHistoryStep( const SYSTEMTIME& time );

	template< class T >
	void AddValue( const NrpText& name, T& amount )
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

	CNrpHistoryStep* GetStep( const SYSTEMTIME& time );
	CNrpHistoryStep* GetLast();
	CNrpHistoryStep* GetFirst();

	CNrpHistoryStep* AddStep( const SYSTEMTIME& time );

	int GetSummFor( const OPTION_NAME& name, const SYSTEMTIME& time );

	CNrpHistory( const NrpText& fileName );
	CNrpHistory();
	~CNrpHistory(void);
private:
	void _InitialyzeOptions();

	HISTORY_MAP _steps;
};

}//end namespace nrp