#include "StdAfx.h"
#include "NrpPlantWork.h"
#include "nrpDiskMachine.h"
#include "NrpGame.h"
#include "NrpGameBox.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "NrpPlant.h"
#include "OpFileSystem.h"

namespace nrp
{
CLASS_NAME CLASS_NRPPLANTWORK( "CNrpPlantWork" );

CNrpPlantWork::CNrpPlantWork( const NrpText& companyName ) : INrpConfig( CLASS_NRPPLANTWORK, "" )
{
	_InitializeOptions();
	Param( NAME ) = companyName;
}

CNrpPlantWork::~CNrpPlantWork(void)
{
}

void CNrpPlantWork::_InitializeOptions()
{
	Add<NrpText>( NAME, "" );
	Add<PNrpDiskMachine>( PRODUCETYPE, NULL ); _self[ PRODUCETYPE ].AddNotification( "calc", this, &CNrpPlantWork::_CalcParams );
	Add<int>( NUMBERMACHINE, 0 );			   _self[ NUMBERMACHINE ].AddNotification( "calc", this, &CNrpPlantWork::_CalcParams );
	Add<int>( NUMBERDAY, 0 );				   _self[ NUMBERDAY ].AddNotification( "calc", this, &CNrpPlantWork::_CalcParams );
	Add<int>( DISKNUMBER, 0 );
	Add<float>( DISKPRICE, 0 );
	Add<int>( FINALPRICE, 0 );
	Add<PNrpGame>( PARENT, NULL );
	Add<NrpText>( COMPANYNAME, "" );
	Add<NrpText>( GAMENAME, "" );
	Add<NrpText>( DISKMACHINENAME, "" );
	Add<int>( DISKINDAY, 0 );
	Add<int>( LEFTPRODUCEDISK, 0 );
	Add<int>( DAYCOST, 0 );
	Add<bool>( FINISHED, false );
	Add<int>( RENTPRICE, 0 );
}

CNrpPlantWork::CNrpPlantWork( const CNrpPlantWork& p ) : INrpConfig( CLASS_NRPPLANTWORK, "" )
{
	_InitializeOptions();
	_self[ NAME ] = p[ NAME ];
	_self[ PRODUCETYPE ] = p[ PRODUCETYPE ];
	_self[ NUMBERMACHINE ] = p[ NUMBERMACHINE ];
	_self[ NUMBERDAY ] = p[ NUMBERDAY ];
	_self[ DISKNUMBER ]=  p[ DISKNUMBER ];
	_self[ DISKPRICE ] = p[ DISKPRICE ];
	_self[ FINALPRICE ] = p[  FINALPRICE ];
	_self[ PARENT ] = p[ PARENT ];
	_self[ COMPANYNAME ] = p[ COMPANYNAME ];
	_self[ GAMENAME ] = p[ GAMENAME ];
	_self[ DISKMACHINENAME ] = p[ DISKMACHINENAME ];
	_self[ DISKINDAY ] = p[ DISKINDAY ];
	_self[ LEFTPRODUCEDISK ] = p[ LEFTPRODUCEDISK ];
	_self[ DAYCOST ] = p[ DAYCOST ];
}

CNrpPlantWork::CNrpPlantWork( const NrpText& fileName, bool load ) : INrpConfig( CLASS_NRPPLANTWORK, "" )
{
	_InitializeOptions();
	Load( fileName );
}

void CNrpPlantWork::Load( const NrpText& fileName )
{
	INrpConfig::Load( fileName );
	CNrpGame* pGame = _nrpApp.GetGame( Text( GAMENAME ) );
	assert( pGame != NULL );
	_self[ PARENT ] = pGame;

	CNrpDiskMachine* dm = CNrpPlant::Instance().GetDiskMachine( Text( DISKMACHINENAME ) );
	assert( dm != NULL );
	_self[ PRODUCETYPE ] = dm;
}

void CNrpPlantWork::_CalcParams( NParam& )
{
	CNrpGame* game = _self[ PARENT ].As<CNrpGame*>();
	PNrpDiskMachine dm = _self[ PRODUCETYPE ].As<PNrpDiskMachine>();
	assert( game && dm );
	if( game && game->Param( GBOX ).As<PNrpGameBox>() && dm )
	{	
		const CNrpDiskMachine& refDm = *dm;
		assert( game->Param( GBOX ).As<PNrpGameBox>() );
		CNrpGameBox& box = *(game->Param( GBOX ).As<PNrpGameBox>());

		int nDay = _self[ NUMBERDAY ];
		int nM = _self[ NUMBERMACHINE ];
		int dskNum = (int)refDm[ DISKPERHOUR ] * 24 * nDay * nM;
		_self[ DISKNUMBER ] = dskNum;
		int price = (int)refDm[ RENTPRICE ] * nM; //цена за размещение заказа
		price += nM * (int)refDm[ PRICEPERHOUR ] * 24 * nDay; //стоимость работы 
		price += (int)( dskNum * box.GetBoxAddonsPrice()); //стоимость дополнительных материалов
		int priceInDay = (int)refDm[ PRICEPERHOUR ] * 24 * nM;//плата за аренду машин в сутки
		priceInDay += static_cast<int>( (int)refDm[ DISKPERHOUR ] * 24 * box.GetBoxAddonsPrice() );//плата за покупку аддонов дл€ коробки
			
		float dskPrice = price / (float)dskNum;
		_self[ NAME ] = (*game)[ NAME ];
		_self[ DISKINDAY ] = (int)refDm[ DISKPERHOUR ] * 24;
		_self[ DISKPRICE ] = dskPrice;
		_self[ FINALPRICE ] = price;
		_self[ DISKMACHINENAME ] = refDm[ NAME ]; 
		_self[ GAMENAME ] = (*game)[ NAME ];
		_self[ DAYCOST ] = priceInDay;
		_self[ RENTPRICE ] = (int)refDm[ RENTPRICE ] * nM;
		_self[ COMPANYNAME ] = (*game)[ COMPANYNAME ];
	}
}

NrpText CNrpPlantWork::Save( const NrpText& folder )
{
	assert( OpFileSystem::IsExist( folder ) );
	NrpText saveFile = OpFileSystem::CheckEndSlash( folder ) + Text( NAME ) + ".work";
	return INrpConfig::Save( saveFile );
}

void CNrpPlantWork::BeginNewDay()
{
	_self[ NUMBERDAY ] += -1;
	PNrpGame game = _self[ PARENT ].As<PNrpGame>();
	assert( game != NULL );

	if( game != NULL )
	{
		PNrpGameBox box = (*game)[ GBOX ].As<PNrpGameBox>();
		PNrpCompany cmp = (*game)[ PARENTCOMPANY ].As<PNrpCompany>();
		CNrpDiskMachine* dm = _self[ PRODUCETYPE ].As<PNrpDiskMachine>();

		assert( box != NULL && cmp != NULL && dm != NULL );

		if( box != NULL && cmp != NULL && dm != NULL )
		{
			int k = (*dm)[ DISKPERHOUR ] * 24;
			dm->AddProducedDisk( (*cmp)[ NAME ], k );

			(*box)[ BOXNUMBER ] += k;
			Log(HW) << "—делано " << k << " коробок с игрой " << game->Text( NAME ) << term;

			if( (int)_self[ NUMBERDAY ] < 0 )
			{
				_self[ FINISHED ] = true;
				Log(HW) << "«акончено производство коробок с игрой " << game->Text( NAME ) << term;
			}
		}
	}
}

NrpText CNrpPlantWork::ClassName()
{
	return CLASS_NRPPLANTWORK;
}

}//end namespace nrp 