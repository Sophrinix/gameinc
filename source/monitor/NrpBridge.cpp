#include "StdAfx.h"
#include "NrpBridge.h"
#include "NrpCompany.h"
#include "NrpBank.h"
#include "NrpApplication.h"
#include "OpFileSystem.h"
#include "NrpHistory.h"
#include "IniFile.h"

using namespace irr;
static nrp::CNrpBridge * globalBridgeInstance = 0;

static const NrpText MY_SAVE( L"bridge.ini" );

namespace nrp 
{
CLASS_NAME CLASS_NRPBRIDGE( L"CNrpBridge" );

CNrpBridge::CNrpBridge(void) : INrpConfig( CLASS_NRPBRIDGE, CLASS_NRPBRIDGE )
{
	Add<int>( COMPANIESNUMBER, 0 );
	Add<int>( SHARES_NUMBER, 0 );
	Add<NrpTime>( LASTTIMEUPDATE, _nrpApp[ CURRENTTIME ] );
}

CNrpBridge& CNrpBridge::Instance()
{
	if( !globalBridgeInstance)
		globalBridgeInstance = new CNrpBridge();

	return *globalBridgeInstance;
}

NrpText CNrpBridge::ClassName()
{
	return CLASS_NRPBRIDGE;
}

int CNrpBridge::_GetSelfCapital( CNrpCompany& cmp )
{
	int sum = (int)cmp[ BALANCE ];	

	return sum;
}

int CNrpBridge::_GetAdditionCapital( CNrpCompany& cmp )
{
	float sum = 0;
	OWNER_MAP::iterator owner = _stocks.find( cmp[ NAME ] );
	if( owner != _stocks.end() )
	{
		STOCK_MAP::iterator share = owner->second.begin();
		for( ; share != owner->second.end(); share++ )
		{
			CShareholder* sh = share->second;
			if( CNrpCompany* ret_cmp = (*sh)[ PARENTCOMPANY ].As<CNrpCompany*>() )
			{
				//не берем в расчет собственные активы при подсчете дополнительных активов
				if( ret_cmp != &cmp )
					sum += (int)(*sh)[ PIE_NUMBER ] * (float)(*ret_cmp)[ PIE_COST ];
			}
		}
	}

	return static_cast< int >( sum );
}

CNrpHistory* CNrpBridge::GetHistory( const NrpText& name )
{
	STOCK_HISTORY::iterator pIter = _history.find( name );
	if( pIter == _history.end() )
	{
		_history[ name ] = new CNrpHistory();
		return _history[ name ];
	}

	return pIter->second;
}

float CNrpBridge::_SplitPie( CNrpCompany& company, float price )
{
	float nPrice = price;

	int koeff = 1;
	while( nPrice > 150 )
	{
		nPrice /= 2;
		koeff *= 2;
	}

	if( koeff > 1 )
	{
		//если акции разделились
		OWNER_MAP::iterator owner = _stocks.begin();
		for( ; owner != _stocks.end(); owner++ )
		{
			//пройдемс€ по всем акционерам и увеличим у них количество акций
			STOCK_MAP::iterator share = owner->second.find( &company );
			if( share != owner->second.end() )
			{
				CShareholder* sh = share->second;
				(*sh)[ PIE_NUMBER ] = (int)(*sh)[ PIE_NUMBER ] * koeff;
			}
		}

		company[ PIE_NUMBER ] = (int)company[ PIE_NUMBER ] * koeff;
		company[ SELF_PIE_NUMBER ] = (int)company[ SELF_PIE_NUMBER ] * koeff;
	}

	return nPrice;
}

void CNrpBridge::_CheckCompany( CNrpCompany& company )
{
	//наличка + активы
	int selfCapital = _GetSelfCapital( company );

	//стоимость акций других компаний и инвестиционных ожиданий
	int otherCapital = _GetAdditionCapital( company ) + (int)company[ INVESTMENT_EXPECTATIONS ];
	int investMoney = selfCapital + otherCapital;

	//долги
	int loanMoney = CNrpBank::Instance().GetLoanMoney( company[ NAME ] );
	
	//предполагаемый доход на свой капитал
	float selfDividend = 0;

	//может быть что стоимость акций еще не расчитыавлась тогда будем считить что
	//доход тоже нулевой
	if( (float)company[ PIE_COST ] > 0 )
	   selfDividend = (float)company[ MONEY_ON_PIE ] / (float)company[ PIE_COST ];

	float koeff1 = ( selfCapital - loanMoney ) / investMoney * selfDividend;

	//предполагаемый доход на добавочный капитал
	float middleDividend = _GetMiddlePercent( (NrpText)company[ NAME ] );
	float koeff2 = otherCapital / investMoney * middleDividend;

	//конечна€ стоимость акции расчитываетс€ исход€ из ожиданий прибыли
	float piePrice = (1 + koeff1 + koeff2) * investMoney / (int)company[ PIE_NUMBER ];

	if( piePrice > 150 )
		piePrice = _SplitPie( company, piePrice );

    //снизим точность вычислений до п€ти знаков
	company[ PIE_COST ] = floor(piePrice * 10000) / 10000.f;

	CNrpHistory* history = GetHistory( company[ NAME ] );
	CNrpHistoryStep* step = history->AddStep( _nrpApp[ CURRENTTIME ].As<NrpTime>() );
	step->AddValue( PIE_COST, (float)company[ PIE_COST ] );
}

void CNrpBridge::_UpdateExpexctations( CNrpCompany& cmp )
{
	if( _self[ LASTTIMEUPDATE ].As<NrpTime>().RMonth() == _nrpApp[ CURRENTTIME ].As<NrpTime>().RMonth() )
		return;

	_self[ LASTTIMEUPDATE ] = _nrpApp[ CURRENTTIME ];
	int expt = cmp[ INVESTMENT_EXPECTATIONS ];
	cmp[ INVESTMENT_EXPECTATIONS ] = static_cast< int >( expt * 0.9f ); 
}

void CNrpBridge::Update()
{
	for( int i=0; i < (int)_nrpApp[ COMPANIESNUMBER ]; i++ )
		if( CNrpCompany* cmp = _nrpApp.GetCompany( i ) )
		{
			if( (int)(*cmp)[ PIE_NUMBER ] > 0 )
			{
				_CheckCompany( *cmp );

				//если началс€ следующий мес€ц
				//уменьшим инвестиционные ожидани€ дл€ следующего мес€ца
				_UpdateExpexctations( *cmp );

				//все компании с акци€ми должны находитьс€ в списке	
				CShareholder* share = GetShares( (*cmp)[ NAME], *cmp );
				if( !share )
				{
					share = new CShareholder( (*cmp)[ NAME ], cmp );
					(*share)[ PIE_NUMBER ] = (*cmp)[ SELF_PIE_NUMBER ];
					_stocks[ (*cmp)[ NAME ] ][ cmp ] = share;
				}
			}
		}
}

float CNrpBridge::_GetMiddlePercent( const NrpText& name )
{
	return 0;
}

CShareholder* CNrpBridge::GetShares( const NrpText& name, CNrpCompany& cmp )
{
	OWNER_MAP::iterator owner = _stocks.find( name );
	if( owner != _stocks.end() )
	{
		STOCK_MAP::iterator share = owner->second.find( &cmp );
		if( share != owner->second.end() )
			return share->second;
	}

	return NULL;
}

void CNrpBridge::Load( const NrpText& saveFolder )
{
	NrpText fileName = OpFileSystem::CheckEndSlash( saveFolder ) + MY_SAVE;
	assert( OpFileSystem::IsExist( fileName) );
	if( OpFileSystem::IsExist( fileName) )
	{
		INrpConfig::Load( fileName );
		IniFile rv( fileName );

		int sharesNum = _self[ SHARES_NUMBER ];

		for( int j=0; j < sharesNum; j++ )
		{
			NrpText shareSaveFile = rv.Get( SECTION_SHARES, CreateKeyItem( j ), NrpText("") );
			CShareholder* share = new CShareholder( shareSaveFile );
			//попробуем найти компанию по имени
			CNrpCompany* cmp = _nrpApp.GetCompany( (NrpText)(*share)[ COMPANYNAME ] );
			assert( cmp );
			if( cmp )
			{
				//запомним компанию дл€ держател€ акций
				(*share)[ PARENTCOMPANY ] = cmp;
				_stocks[ (NrpText)(*share)[ OWNER ] ][ cmp ] = share;
			}
		}
	}

	_self[ LASTTIMEUPDATE ] = _nrpApp[ CURRENTTIME ];
}

NrpText CNrpBridge::Save( const NrpText& saveFolder )
{
	//переданна€ директори€ должна существовать
	assert( OpFileSystem::IsExist( saveFolder ) );

	NrpText localFolder = OpFileSystem::CheckEndSlash( saveFolder );

	//если нет директории в которую надо сохран€ть данные
	OpFileSystem::CreateDirectory( localFolder );

	NrpText saveFile = localFolder + MY_SAVE;

	assert( !OpFileSystem::IsExist( saveFile ) );
	INrpConfig::Save( saveFile );

	IniFile sv( saveFile );

	int sequenceNum = 0;
	for( OWNER_MAP::iterator rIter=_stocks.begin(); rIter != _stocks.end(); rIter++ )
	{
		NrpText shareDir =  OpFileSystem::CheckEndSlash( localFolder + rIter->first );
		//если нет директории в которую надо сохран€ть данные
		OpFileSystem::CreateDirectory( shareDir );

		STOCK_MAP::iterator share = rIter->second.begin();
		for( ; share != rIter->second.end(); share++, sequenceNum++ )
		{
			 NrpText ret = share->second->Save( shareDir + rIter->first + (NrpText)(*(share->second))[ COMPANYNAME ] + ".share" );
			 sv.Set( SECTION_SHARES, CreateKeyItem( sequenceNum ), ret );
		}
	}

	sv.Set( SECTION_PROPERTIES, SHARES_NUMBER+":int", sequenceNum );

	sv.Save();
	return saveFile;
}

bool CNrpBridge::_BuyAvaible( INrpConfig& who, CNrpCompany& cmp, int shareNumber )
{
	float price = shareNumber * (float)cmp[ PIE_COST ];
	int shareToSell = (int)cmp[ PIE_NUMBER ] - (int)cmp[ SELF_PIE_NUMBER ];
	
	return ((int)who[ BALANCE ] > price && shareToSell > shareNumber );
}

bool CNrpBridge::_SellAvaible( INrpConfig& who, CShareholder& share, int shareNumber )
{
	return (int)share[ PIE_NUMBER ] > shareNumber;
}

int CNrpBridge::ChangeShares( INrpConfig& agent, CNrpCompany& cmp, int shareNumber )
{
	//игрок хочет купить акции
	if( shareNumber > 0 )
	{
		CShareholder* share = NULL;
		
		//у игрока есть возможность купить акции и у второй стороны есть столько акций на продажу
		if( _BuyAvaible( agent, cmp, shareNumber ) )
		{
			//возможно уже есть свз€и между игроком и компанией
			share = GetShares( agent[ NAME ], cmp );

			//у этого игрока нет акций данной компании, но он хочет их купить
			if( !share )
			{
				share = new CShareholder( agent[ NAME ], &cmp );
				_stocks[ agent[ NAME ] ][ &cmp ] = share;
			}
		}

		//теперь изменим количество акций у игрока
		if( share )		
		{
			(*share)[ PIE_NUMBER ] += shareNumber;
			int price = static_cast< int >( shareNumber * (float)cmp[ PIE_COST ] );
			agent[ BALANCE ] -= price;
            //стоимость совершени€ сделки
            agent[ BALANCE ] -= static_cast< int >( abs( price ) * 0.02 );
			cmp[ INVESTMENT_EXPECTATIONS ] += price;
		}
	}
	else //игрок хочет продать акции
	{
		//найдем владельца акций
		if( CShareholder* share = GetShares( agent[ NAME ], cmp ) )
		{
			if( _SellAvaible( agent, *share, shareNumber ) )
			{
				(*share)[ PIE_NUMBER ] -= shareNumber;
				int price = static_cast< int >( abs( shareNumber ) * (float)cmp[ PIE_COST ] );
				agent[ BALANCE ] += price;
                //стоимость совершени€ сделки
                agent[ BALANCE ] -= static_cast< int >( abs( price ) * 0.02 );
				cmp[ INVESTMENT_EXPECTATIONS ] -= price;
			}
		}
	}

	Update();
	return 0;
}

int CNrpBridge::GetAvaibleShares( CNrpCompany& cmp )
{
	int buyingShare = 0;
	OWNER_MAP::iterator owner = _stocks.begin();
	for( ; owner != _stocks.end(); owner++ )
	{
		//пройдемс€ по всем акционерам и подсчитаем количество купленных акций
		STOCK_MAP::const_iterator share = owner->second.find( &cmp );
		if( share != owner->second.end() )
		{
			buyingShare += (int)(*(share->second))[ PIE_NUMBER ];
		}
	}

	return ((int)cmp[ PIE_NUMBER ] - buyingShare );
}


CLASS_NAME CLASS_CSHAREHOLDER( L"CShareholder" );

void CShareholder::_InitOptions()
{
	Add<PNrpCompany>( PARENTCOMPANY, NULL );
	Add<NrpText>( COMPANYNAME, L"" );
	Add<NrpText>( OWNER, L"" );
	Add( PIE_NUMBER, 0 );
}

CShareholder::CShareholder( const NrpText& name, CNrpCompany* cmp ) : INrpConfig( CLASS_CSHAREHOLDER, "" )
{
	_InitOptions();
	_self[ PARENTCOMPANY ] = cmp;
	_self[ COMPANYNAME ] = (*cmp)[ NAME ];
	_self[ OWNER ] = name;
}

CShareholder::CShareholder( const NrpText& fileSave ) : INrpConfig( CLASS_CSHAREHOLDER, "" )
{
	_InitOptions();
	Load( fileSave );
}

NrpText CShareholder::Save( const NrpText& fileName )
{
	return INrpConfig::Save( fileName );
}

}//namespace nrp