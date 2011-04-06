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

namespace nrp 
{
CLASS_NAME CLASS_NRPBRIDGE( L"CNrpBridge" );

CNrpBridge::CNrpBridge(void) : INrpConfig( CLASS_NRPBRIDGE, CLASS_NRPBRIDGE )
{
	Add<int>( COMPANIESNUMBER, 0 );
	Add<int>( SHARES_NUMBER, 0 );
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
			if( CNrpCompany* cmp = (*sh)[ PARENTCOMPANY ].As<CNrpCompany*>() )
			{
				sum += (int)(*sh)[ PIE_NUMBER ] * (float)(*cmp)[ PIE_COST ];
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

void CNrpBridge::_CheckCompany( CNrpCompany& company )
{
	//������� + ������
	int selfCapital = _GetSelfCapital( company );

	//��������� ����� ������ ��������
	int otherCapital = _GetAdditionCapital( company );
	int investMoney = selfCapital + otherCapital;

	//�����
	int loanMoney = CNrpBank::Instance().GetLoanMoney( company[ NAME ] );
	
	//�������������� ����� �� ���� �������
	float selfDividend = 0;

	//����� ���� ��� ��������� ����� ��� �� ������������� ����� ����� ������� ���
	//����� ���� �������
	if( (float)company[ PIE_COST ] > 0 )
	   selfDividend = (float)company[ MONEY_ON_PIE ] / (float)company[ PIE_COST ];

	float koeff1 = ( selfCapital - loanMoney ) / investMoney * selfDividend;

	//�������������� ����� �� ���������� �������
	float middleDividend = _GetMiddlePercent( (NrpText)company[ NAME ] );
	float koeff2 = otherCapital / investMoney * middleDividend;

	//�������� ��������� ����� ������������� ������ �� �������� �������
	float piePrice = (1 + koeff1 + koeff2) * investMoney / (int)company[ PIE_NUMBER ];

	company[ PIE_COST ] = piePrice;

	CNrpHistory* history = GetHistory( company[ NAME ] );
	CNrpHistoryStep* step = history->AddStep( _nrpApp[ CURRENTTIME ].As<NrpTime>() );
	step->AddValue( PIE_COST, piePrice );
}

void CNrpBridge::Update()
{
	for( int i=0; i < (int)_nrpApp[ COMPANIESNUMBER ]; i++ )
		if( CNrpCompany* cmp = _nrpApp.GetCompany( i ) )
		{
			if( (int)(*cmp)[ PIE_NUMBER ] > 0 )
			{
				_CheckCompany( *cmp );

				 //��� �������� � ������� ������ ���������� � ������	
				 CShareholder* share = GetShares( (*cmp)[ NAME], cmp );
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

CShareholder* CNrpBridge::GetShares( const NrpText& name, CNrpCompany* cmp )
{
	OWNER_MAP::iterator owner = _stocks.find( name );
	if( owner != _stocks.end() )
	{
		STOCK_MAP::iterator share = owner->second.find( cmp );
		if( share != owner->second.end() )
			return share->second;
	}

	return NULL;
}

void CNrpBridge::Load( const NrpText& fileName )
{
	if( OpFileSystem::IsExist( fileName) )
	{
		INrpConfig::Load( fileName );
		IniFile rv( fileName );
		int sharesNum = rv.Get( SECTION_PROPERTIES, SHARES_NUMBER, (int)0 );
		for( int j=0; j < sharesNum; j++ )
		{
			NrpText shareSaveFile = rv.Get( SECTION_PROPERTIES, CreateKeyItem( j ), NrpText("") );
			CShareholder* share = new CShareholder( shareSaveFile );
			//��������� ����� �������� �� �����
			CNrpCompany* cmp = _nrpApp.GetCompany( (NrpText)(*share)[ COMPANYNAME ] );
			assert( cmp );
			if( cmp )
			{
				//�������� �������� ��� ��������� �����
				(*share)[ PARENTCOMPANY ] = cmp;
				_stocks[ (NrpText)(*share)[ OWNER ] ][ cmp ] = share;
			}
		}
	}
}

NrpText CNrpBridge::Save( const NrpText& saveFolder )
{
	//���������� ���������� ������ ������������
	assert( OpFileSystem::IsExist( saveFolder ) );

	NrpText localFolder = OpFileSystem::CheckEndSlash( saveFolder + Text( NAME ) );

	//���� ��� ���������� � ������� ���� ��������� ������
	OpFileSystem::CreateDirectory( localFolder );

	NrpText saveFile = localFolder + "bridge.ini";

	assert( !OpFileSystem::IsExist( saveFile ) );
	INrpConfig::Save( saveFile );

	IniFile sv( saveFile );

	int sequenceNum = 0;
	for( OWNER_MAP::iterator rIter=_stocks.begin(); rIter != _stocks.end(); rIter++ )
	{
		NrpText shareDir =  OpFileSystem::CheckEndSlash( localFolder + rIter->first );
		//���� ��� ���������� � ������� ���� ��������� ������
		OpFileSystem::CreateDirectory( shareDir );

		STOCK_MAP::iterator share = rIter->second.begin();
		for( ; share != rIter->second.end(); share++, sequenceNum++ )
		{
			 NrpText ret = share->second->Save( shareDir + rIter->first + (NrpText)(*(share->second))[ COMPANYNAME ] + ".share" );
			 sv.Set( SECTION_PROPERTIES, CreateKeyItem( sequenceNum ), ret );
		}
	}

	sv.Set( SECTION_PROPERTIES, SHARES_NUMBER, sequenceNum );
	return saveFile;
}

bool CNrpBridge::_BuyAvaible( INrpConfig* who, CNrpCompany* cmp, int shareNumber )
{
	float price = shareNumber * (float)(*cmp)[ PIE_COST ];
	int shareToSell = (int)(*cmp)[ PIE_NUMBER ] - (int)(*cmp)[ SELF_PIE_NUMBER ];
	
	return ((int)(*who)[ BALANCE ] > price && shareToSell > shareNumber );
}

bool CNrpBridge::_SellAvaible( INrpConfig* who, CShareholder* share, int shareNumber )
{
	return (int)(*share)[ PIE_NUMBER ] > shareNumber;
}

int CNrpBridge::ChangeShares( INrpConfig* agent, CNrpCompany* cmp, int shareNumber )
{
	//����� ����� ������ �����
	if( shareNumber > 0 )
	{
		CShareholder* share = NULL;
		
		//� ������ ���� ����������� ������ ����� � � ������ ������� ���� ������� ����� �� �������
		if( _BuyAvaible( agent, cmp, shareNumber ) )
		{
			//�������� ��� ���� ����� ����� ������� � ���������
			share = GetShares( (*agent)[ NAME ], cmp );

			//� ����� ������ ��� ����� ������ ��������, �� �� ����� �� ������
			if( !share )
				share = new CShareholder( (*agent)[ NAME ], cmp );
		}

		//������ ������� ���������� ����� � ������
		if( share )		
		{
			(*share)[ PIE_NUMBER ] += shareNumber;
			(*agent)[ BALANCE ] -= shareNumber * (float)(*cmp)[ PIE_COST ];
		}
	}
	else //����� ����� ������� �����
	{
		//������ ��������� �����
		if( CShareholder* share = GetShares( (*agent)[ NAME ], cmp ) )
		{
			if( _SellAvaible( agent, share, shareNumber ) )
			{
				(*share)[ PIE_NUMBER ] -= shareNumber;
				(*agent)[ BALANCE ] += shareNumber * (float)(*cmp)[ PIE_COST ];
			}
		}
	}

	Update();
	return 0;
}

CLASS_NAME CLASS_CSHAREHOLDER( L"CShareholder" );

CShareholder::CShareholder( const NrpText& name, CNrpCompany* cmp ) : INrpConfig( CLASS_CSHAREHOLDER, "" )
{
	Add<PNrpCompany>( PARENTCOMPANY, cmp );
	Add( COMPANYNAME, (NrpText)(*cmp)[ NAME ] );
	Add( OWNER, name );
	Add( PIE_NUMBER, 0 );
}

CShareholder::CShareholder( const NrpText& fileSave ) : INrpConfig( CLASS_CSHAREHOLDER, "" )
{
	Load( fileSave );
}

NrpText CShareholder::Save( const NrpText& fileName )
{
	return INrpConfig::Save( fileName );
}

}//namespace nrp