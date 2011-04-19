#pragma once
#include "nrpconfig.h"
#include "nrpArrays.h"
#include <map>

namespace nrp
{

OPTION_NAME SHARES_NUMBER("shares_number");

class CNrpHistory;

class CShareholder : public INrpConfig
{
public:
	CShareholder( const NrpText& name, CNrpCompany* cmp );
	CShareholder( const NrpText& fileSave );

	NrpText Save( const NrpText& fileName );
};

class CNrpBridge : public INrpConfig
{
private:
	CNrpBridge();

public:
	static CNrpBridge& Instance();
	static NrpText ClassName();

	CShareholder* GetShares( const NrpText& name, CNrpCompany* cmp );
	int GetAvaibleShares( CNrpCompany* cmp );
	int ChangeShares( INrpConfig* agent, CNrpCompany* cmp, int shareNumber );

	void Load( const NrpText& fileName );
	NrpText Save( const NrpText& fileName );

	CNrpHistory* GetHistory( const NrpText& name );

	void Update();
private:
	typedef std::map< CNrpCompany*, CShareholder* > STOCK_MAP;
	typedef std::map< NrpText, STOCK_MAP > OWNER_MAP;
	typedef std::map< NrpText, CNrpHistory* > STOCK_HISTORY;

	OWNER_MAP _stocks;
	STOCK_HISTORY _history;

	void _CheckCompany( CNrpCompany& company );

	float _GetMiddlePercent( const NrpText& name );
	bool _BuyAvaible( INrpConfig* who, CNrpCompany* cmp, int shareNumber );
	bool _SellAvaible( INrpConfig* who, CShareholder* share, int shareNumber );
	int  _GetSelfCapital( CNrpCompany& cmp );
	int  _GetAdditionCapital( CNrpCompany& cmp );
	float _SplitPie( CNrpCompany& company, float price );
};

}//namespace nrp