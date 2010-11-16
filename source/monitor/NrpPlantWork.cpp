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
	InitializeOptions_();
	SetString( NAME, companyName );
}

CNrpPlantWork::~CNrpPlantWork(void)
{
}

void CNrpPlantWork::InitializeOptions_()
{
	CreateValue<NrpText>( NAME, "" );
	CreateValue<PNrpDiskMachine>( PRODUCETYPE, NULL );
	CreateValue<int>( NUMBERMACHINE, 0 );
	CreateValue<int>( NUMBERDAY, 0 );
	CreateValue<int>( DISKNUMBER, 0 );
	CreateValue<float>( DISKPRICE, 0 );
	CreateValue<int>( FINALPRICE, 0 );
	CreateValue<PNrpGame>( PARENT, NULL );
	CreateValue<NrpText>( COMPANYNAME, "" );
	CreateValue<NrpText>( GAMENAME, "" );
	CreateValue<NrpText>( DISKMACHINENAME, "" );
	CreateValue<int>( DISKINDAY, 0 );
	CreateValue<int>( LEFTPRODUCEDISK, 0 );
	CreateValue<int>( DAYCOST, 0 );
	CreateValue<bool>( FINISHED, false );
	CreateValue<int>( RENTPRICE, 0 );
}

CNrpPlantWork::CNrpPlantWork( const CNrpPlantWork& p ) : INrpConfig( CLASS_NRPPLANTWORK, "" )
{
	InitializeOptions_();
	SetString( NAME, p.GetValue<NrpText>( NAME ) );
	SetValue<PNrpDiskMachine>( PRODUCETYPE,  p.GetValue<PNrpDiskMachine>( PRODUCETYPE ) );
	SetValue<int>( NUMBERMACHINE, p.GetValue<int>( NUMBERMACHINE ) );
	SetValue<int>( NUMBERDAY, p.GetValue<int>( NUMBERDAY ) );
	SetValue<int>( DISKNUMBER, p.GetValue<int>( DISKNUMBER ) );
	SetValue<float>( DISKPRICE, p.GetValue<float>( DISKPRICE ) );
	SetValue<int>( FINALPRICE, p.GetValue<int>( FINALPRICE ) );
	SetValue<PNrpGame>( PARENT, p.GetValue<PNrpGame>( PARENT ) );
	SetString( COMPANYNAME, p.GetValue<NrpText>( COMPANYNAME ) );
	SetString( GAMENAME, p.GetValue<NrpText>( GAMENAME ) );
	SetString( DISKMACHINENAME, p.GetValue<NrpText>( DISKMACHINENAME ) );
	SetValue<int>( DISKINDAY, p.GetValue<int>( DISKINDAY ) );
	SetValue<int>( LEFTPRODUCEDISK, p.GetValue<int>( LEFTPRODUCEDISK ) );
	SetValue<int>( DAYCOST, p.GetValue<int>( DAYCOST ) );
}

CNrpPlantWork::CNrpPlantWork( const NrpText& fileName, bool load ) : INrpConfig( CLASS_NRPPLANTWORK, "" )
{
	InitializeOptions_();
	Load( fileName );
}

void CNrpPlantWork::Load( const NrpText& fileName )
{
	INrpConfig::Load( fileName );
	CNrpGame* pGame = CNrpApplication::Instance().GetGame( GetString( GAMENAME ) );
	assert( pGame != NULL );
	SetValue<PNrpGame>( PARENT, pGame );

	CNrpDiskMachine* dm = CNrpPlant::Instance().GetDiskMachine( GetString( DISKMACHINENAME ) );
	assert( dm != NULL );
	SetValue<PNrpDiskMachine>( PRODUCETYPE, dm );

	CalcParams_();
}

void CNrpPlantWork::CalcParams_()
{
	CNrpGame* game = GetValue<PNrpGame>( PARENT );
	PNrpDiskMachine dm = GetValue<PNrpDiskMachine>( PRODUCETYPE );
	if( game && game->GetValue<PNrpGameBox>( GBOX ) && dm )
	{	
		PNrpGameBox box = game->GetValue<PNrpGameBox>( GBOX );
		int nDay = GetValue<int>( NUMBERDAY );
		int nM = GetValue<int>( NUMBERMACHINE );
		int dskNum = dm->GetValue<int>( DISKPERHOUR ) * 24 * nDay * nM;
		INrpConfig::SetValue<int>( DISKNUMBER, dskNum );
		int price = dm->GetValue<int>( RENTPRICE ) * nM; //цена за размещение заказа
		price += nM * ( dm->GetValue<int>( PRICEPERHOUR ) * 24 * nDay ); //стоимость работы 
		price += (int)(dskNum * box->GetBoxAddonsPrice()); //стоимость дополнительных материалов
		int priceInDay = dm->GetValue<int>( PRICEPERHOUR ) * 24 * nM;//плата за аренду машин в сутки
		priceInDay += static_cast<int>( dm->GetValue<int>( DISKPERHOUR ) * 24 * box->GetBoxAddonsPrice() );//плата за покупку аддонов дл€ коробки
			
		float dskPrice = price / (float)dskNum;
		INrpConfig::SetString( NAME, game->GetString( NAME ) );
		INrpConfig::SetValue<int>( DISKINDAY, dm->GetValue<int>( DISKPERHOUR ) * 24 );
		INrpConfig::SetValue<float>( DISKPRICE, dskPrice );
		INrpConfig::SetValue<int>( FINALPRICE, price );
		INrpConfig::SetString( DISKMACHINENAME, dm->GetString( NAME ) ); 
		INrpConfig::SetString( GAMENAME, game->GetString( NAME ) );
		INrpConfig::SetValue<int>( DAYCOST, priceInDay );
		INrpConfig::SetValue<int>( RENTPRICE, dm->GetValue<int>( RENTPRICE ) * nM );
		INrpConfig::SetString( COMPANYNAME, game->GetValue<PNrpCompany>( PARENTCOMPANY )->GetString( NAME ) );
	}
}

NrpText CNrpPlantWork::Save( const NrpText& folder )
{
	assert( OpFileSystem::IsExist( folder ) );
	NrpText saveFile = OpFileSystem::CheckEndSlash( folder ) + GetString( NAME ) + ".work";
	return INrpConfig::Save( saveFile );
}

void CNrpPlantWork::BeginNewDay()
{
	AddValue<int>( NUMBERDAY, -1 );
	PNrpGame game = GetValue<PNrpGame>( PARENT );
	assert( game != NULL );

	if( game != NULL )
	{
		PNrpGameBox box = game->GetValue<PNrpGameBox>( GBOX );
		PNrpCompany cmp = game->GetValue<PNrpCompany>( PARENTCOMPANY );
		CNrpDiskMachine* dm = GetValue<PNrpDiskMachine>( PRODUCETYPE );

		assert( box != NULL && cmp != NULL && dm != NULL );

		if( box != NULL && cmp != NULL && dm != NULL )
		{
			int k = dm->GetValue<int>( DISKPERHOUR ) * 24;
			dm->AddProducedDisk( cmp->GetString( NAME ), k );

			box->AddValue<int>( BOXNUMBER, k );
			Log(HW) << "—делано " << k << " коробок с игрой " << game->GetString( NAME ) << term;

			if( GetValue<int>( NUMBERDAY ) < 0 )
			{
				SetValue<bool>( FINISHED, true );
				Log(HW) << "«акончено производство коробок с игрой " << game->GetString( NAME ) << term;
			}
		}
	}
}

NrpText CNrpPlantWork::ClassName()
{
	return CLASS_NRPPLANTWORK;
}

}//end namespace nrp 