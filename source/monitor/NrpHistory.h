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

	template< class T >
	void AddValue( std::string name, T& amount )
	{
		if( !IsValueExist( name ) )
			CreateValue<T>( name, amount );
		else
			INrpConfig::AddValue<T>( name, amount );
	}
};

class CNrpHistory :	public INrpConfig
{
public:
	std::string Save( const std::string& fileName );
	void Load( const std::string& fileName );

	CNrpHistoryStep* GetStep( const SYSTEMTIME& time, bool aprocimateData );
	CNrpHistoryStep* GetStep( const SYSTEMTIME& time );

	CNrpHistoryStep* AddStep( SYSTEMTIME& time );

	CNrpHistory();
	~CNrpHistory(void);
private:

	typedef std::map< int, CNrpHistoryStep* > SALE_HISTORY_MAP;
	SALE_HISTORY_MAP _steps;
};

}//end namespace nrp