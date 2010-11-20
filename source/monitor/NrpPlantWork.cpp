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
	Push<NrpText>( NAME, "" );
	Push<PNrpDiskMachine>( PRODUCETYPE, NULL );
	Push<int>( NUMBERMACHINE, 0 );
	Push<int>( NUMBERDAY, 0 );
	Push<int>( DISKNUMBER, 0 );
	Push<float>( DISKPRICE, 0 );
	Push<int>( FINALPRICE, 0 );
	Push<PNrpGame>( PARENT, NULL );
	Push<NrpText>( COMPANYNAME, "" );
	Push<NrpText>( GAMENAME, "" );
	Push<NrpText>( DISKMACHINENAME, "" );
	Push<int>( DISKINDAY, 0 );
	Push<int>( LEFTPRODUCEDISK, 0 );
	Push<int>( DAYCOST, 0 );
	Push<bool>( FINISHED, false );
	Push<int>( RENTPRICE, 0 );
}

CNrpPlantWork::CNrpPlantWork( const CNrpPlantWork& p ) : INrpConfig( CLASS_NRPPLANTWORK, "" )
{
	_InitializeOptions();
	Param( NAME ) = p[ NAME ];
	Param( PRODUCETYPE ) = p[ PRODUCETYPE ];
	Param( NUMBERMACHINE ) = p[ NUMBERMACHINE ];
	Param( NUMBERDAY ) = p[ NUMBERDAY ];
	Param( DISKNUMBER ) =  p[ DISKNUMBER ];
	Param( DISKPRICE ) = p[ DISKPRICE ];
	Param( FINALPRICE ) = p[  FINALPRICE ];
	Param( PARENT ) = p[ PARENT ];
	Param( COMPANYNAME ) = p[ COMPANYNAME ];
	Param( GAMENAME ) = p[ GAMENAME ];
	Param( DISKMACHINENAME ) = p[ DISKMACHINENAME ];
	Param( DISKINDAY ) = p[ DISKINDAY ];
	Param( LEFTPRODUCEDISK ) = p[ LEFTPRODUCEDISK ];
	Param( DAYCOST ) = p[ DAYCOST ];
}

CNrpPlantWork::CNrpPlantWork( const NrpText& fileName, bool load ) : INrpConfig( CLASS_NRPPLANTWORK, "" )
{
	_InitializeOptions();
	Load( fileName );
}

void CNrpPlantWork::Load( const NrpText& fileName )
{
	INrpConfig::Load( fileName );
	CNrpGame* pGame = CNrpApplication::Instance().GetGame( Text( GAMENAME ) );
	assert( pGame != NULL );
	SetValue<PNrpGame>( PARENT, pGame );

	CNrpDiskMachine* dm = CNrpPlant::Instance().GetDiskMachine( Text( DISKMACHINENAME ) );
	assert( dm != NULL );
	SetValue<PNrpDiskMachine>( PRODUCETYPE, dm );

	CalcParams_();
}

void CNrpPlantWork::CalcParams_()
{
	CNrpGame* game = Param( PARENT ).As<CNrpGame*>();
	PNrpDiskMachine dm = Param( PRODUCETYPE ).As<PNrpDiskMachine>();
	assert( game && dm );
	if( game && game->Param( GBOX ).As<PNrpGameBox>() && dm )
	{	
		const CNrpDiskMachine& refDm = *dm;
		assert( game->Param( GBOX ).As<PNrpGameBox>() );
		CNrpGameBox& box = *(game->Param( GBOX ).As<PNrpGameBox>());

		int nDay = Param( NUMBERDAY );
		int nM = Param( NUMBERMACHINE );
		int dskNum = (int)refDm[ DISKPERHOUR ] * 24 * nDay * nM;
		INrpConfig::Param( DISKNUMBER ) = dskNum;
		int price = (int)refDm[ RENTPRICE ] * nM; //цена за размещение заказа
		price += nM * (int)refDm[ PRICEPERHOUR ] * 24 * nDay; //стоимость работы 
		price += (int)( dskNum * box.GetBoxAddonsPrice()); //стоимость дополнительных материалов
		int priceInDay = (int)refDm[ PRICEPERHOUR ] * 24 * nM;//плата за аренду машин в сутки
		priceInDay += static_cast<int>( (int)refDm[ DISKPERHOUR ] * 24 * box.GetBoxAddonsPrice() );//плата за покупку аддонов дл€ коробки
			
		float dskPrice = price / (float)dskNum;
		INrpConfig::Param( NAME ) = (*game)[ NAME ];
		INrpConfig::Param( DISKINDAY ) = (int)refDm[ DISKPERHOUR ] * 24;
		INrpConfig::Param( DISKPRICE ) = dskPrice;
		INrpConfig::Param( FINALPRICE ) = price;
		INrpConfig::Param( DISKMACHINENAME ) = refDm[ NAME ]; 
		INrpConfig::Param( GAMENAME ) = (*game)[ NAME ];
		INrpConfig::Param( DAYCOST ) = priceInDay;
		INrpConfig::Param( RENTPRICE ) = (int)refDm[ RENTPRICE ] * nM;
		INrpConfig::Param( COMPANYNAME ) = (*game)[ PARENTCOMPANY ].As<PNrpCompany>()->Text( NAME );
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
	Param( NUMBERDAY ) += -1;
	PNrpGame game = Param( PARENT ).As<PNrpGame>();
	assert( game != NULL );

	if( game != NULL )
	{
		PNrpGameBox box = (*game)[ GBOX ].As<PNrpGameBox>();
		PNrpCompany cmp = (*game)[ PARENTCOMPANY ].As<PNrpCompany>();
		CNrpDiskMachine* dm = Param( PRODUCETYPE ).As<PNrpDiskMachine>();

		assert( box != NULL && cmp != NULL && dm != NULL );

		if( box != NULL && cmp != NULL && dm != NULL )
		{
			int k = (*dm)[ DISKPERHOUR ] * 24;
			dm->AddProducedDisk( (*cmp)[ NAME ], k );

			(*box)[ BOXNUMBER ] += k;
			Log(HW) << "—делано " << k << " коробок с игрой " << game->Text( NAME ) << term;

			if( (int)Param( NUMBERDAY ) < (int)0 )
			{
				SetValue<bool>( FINISHED, true );
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