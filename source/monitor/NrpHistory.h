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
			Param( name ).As<T>() += amount;
	}
};

class CNrpHistory :	public INrpConfig
{
public:
	NrpText Save( const NrpText& fileName );
	void Load( const NrpText& fileName );

	CNrpHistoryStep* GetStep( const SYSTEMTIME& time, bool aprocimateData );
	CNrpHistoryStep* GetStep( const SYSTEMTIME& time );

	CNrpHistoryStep* AddStep( SYSTEMTIME& time );

	CNrpHistory();
	~CNrpHistory(void);
private:

	typedef core::map< int, CNrpHistoryStep* > SALE_HISTORY_MAP;
	SALE_HISTORY_MAP _steps;
};

}//end namespace nrp