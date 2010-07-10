#include "StdAfx.h"
#include "NrpPlantWork.h"
#include "nrpDiskMachine.h"
#include "NrpGame.h"
#include "NrpGameBox.h"
#include "NrpCompany.h"
#include "NrpApplication.h"

namespace nrp
{

CNrpPlantWork::CNrpPlantWork( std::string companyName ) : INrpConfig( CLASS_NRPPLANTWORK, "" )
{
	InitializeOptions_();
	SetValue<std::string>( NAME, companyName );
}

CNrpPlantWork::~CNrpPlantWork(void)
{
}

void CNrpPlantWork::InitializeOptions_()
{
	CreateValue<std::string>( NAME, "" );
	CreateValue<PNrpDiskMachine>( PRODUCETYPE, NULL );
	CreateValue<int>( NUMBERMACHINE, 0 );
	CreateValue<int>( NUMBERDAY, 0 );
	CreateValue<int>( DISKNUMBER, 0 );
	CreateValue<float>( DISKPRICE, 0 );
	CreateValue<int>( FINALPRICE, 0 );
	CreateValue<PNrpGame>( PARENT, NULL );
	CreateValue<std::string>( COMPANYNAME, "" );
	CreateValue<std::string>( GAMENAME, "" );
	CreateValue<std::string>( DISKMACHINENAME, "" );
	CreateValue<int>( DISKINDAY, 0 );
	CreateValue<int>( LEFTPRODUCEDISK, 0 );
	CreateValue<int>( PRICEINDAY, 0 );
	CreateValue<bool>( FINISHED, false );
	CreateValue<int>( RENTPRICE, 0 );
}

CNrpPlantWork::CNrpPlantWork( const CNrpPlantWork& p ) : INrpConfig( CLASS_NRPPLANTWORK, "" )
{
	InitializeOptions_();
	SetValue<std::string>( NAME, p.GetValue<std::string>( NAME ) );
	SetValue<PNrpDiskMachine>( PRODUCETYPE,  p.GetValue<PNrpDiskMachine>( PRODUCETYPE ) );
	SetValue<int>( NUMBERMACHINE, p.GetValue<int>( NUMBERMACHINE ) );
	SetValue<int>( NUMBERDAY, p.GetValue<int>( NUMBERDAY ) );
	SetValue<int>( DISKNUMBER, p.GetValue<int>( DISKNUMBER ) );
	SetValue<float>( DISKPRICE, p.GetValue<float>( DISKPRICE ) );
	SetValue<int>( FINALPRICE, p.GetValue<int>( FINALPRICE ) );
	SetValue<PNrpGame>( PARENT, p.GetValue<PNrpGame>( PARENT ) );
	SetValue<std::string>( COMPANYNAME, p.GetValue<std::string>( COMPANYNAME ) );
	SetValue<std::string>( GAMENAME, p.GetValue<std::string>( GAMENAME ) );
	SetValue<std::string>( DISKMACHINENAME, p.GetValue<std::string>( DISKMACHINENAME ) );
	SetValue<int>( DISKINDAY, p.GetValue<int>( DISKINDAY ) );
	SetValue<int>( LEFTPRODUCEDISK, p.GetValue<int>( LEFTPRODUCEDISK ) );
	SetValue<int>( PRICEINDAY, p.GetValue<int>( PRICEINDAY ) );
}

void CNrpPlantWork::Load( std::string sectionName, std::string fileName )
{
	INrpConfig::Load( sectionName, fileName );
	CNrpGame* pGame = CNrpApplication::Instance().GetGame( GetValue<std::string>( GAMENAME ) );
	assert( pGame != NULL );
	SetValue<PNrpGame>( PARENT, pGame );

	CNrpDiskMachine* dm = CNrpApplication::Instance().GetDiskMachine( GetValue<std::string>( DISKMACHINENAME ) );
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
		int price = dm->GetValue<int>( RENTPRICE ) * nM;
		price += nM * ( dm->GetValue<int>( PRICEPERHOUR ) * 24 * nDay );
		price += (int)(dskNum * box->GetBoxAddonsPrice());
		int priceInDay = dm->GetValue<int>( PRICEPERHOUR ) * 24 * nM;//плата за аренду машин в сутки
		priceInDay += dm->GetValue<int>( DISKPERHOUR ) * 24 * box->GetBoxAddonsPrice();//плата за покупку аддонов для коробки
			
		float dskPrice = price / (float)dskNum;
		INrpConfig::SetValue<std::string>( NAME, game->GetValue<std::string>( NAME ) );
		INrpConfig::SetValue<int>( DISKINDAY, dm->GetValue<int>( DISKPERHOUR ) * 24 );
		INrpConfig::SetValue<float>( DISKPRICE, dskPrice );
		INrpConfig::SetValue<int>( FINALPRICE, price );
		INrpConfig::SetValue<std::string>( DISKMACHINENAME, dm->GetValue<std::string>( NAME ) ); 
		INrpConfig::SetValue<std::string>( GAMENAME, game->GetValue<std::string>( NAME ) );
		INrpConfig::SetValue<int>( PRICEINDAY, priceInDay );
		INrpConfig::SetValue<int>( RENTPRICE, dm->GetValue<int>( RENTPRICE ) * nM );
		INrpConfig::SetValue<std::string>( COMPANYNAME, game->GetValue<PNrpCompany>( PARENTCOMPANY )->GetValue<std::string>( NAME ) );
	}
}

void CNrpPlantWork::Save( std::string sectionName, std::string fileName )
{
	INrpConfig::Save( sectionName, fileName );
}

void CNrpPlantWork::BeginNewDay()
{
	AddValue<int>( NUMBERDAY, -1 );
	PNrpGame game = GetValue<PNrpGame>( PARENT );
	PNrpGameBox box = game->GetValue<PNrpGameBox>( GBOX );
	PNrpCompany cmp = game->GetValue<PNrpCompany>( PARENTCOMPANY );
	
	if( cmp == NULL )
		return;

	cmp->AddValue<int>( BALANCE, -GetValue<int>( PRICEINDAY ) );
	int k = GetValue<PNrpDiskMachine>( PRODUCETYPE )->GetValue<int>( DISKPERHOUR ) * 24;
	box->AddValue<int>( BOXNUMBER, k );
	OutputDebugString( ("Сделано " + IntToStr( k ) + " коробок с игрой " + game->GetValue<std::string>( NAME ) + "\n" ).c_str() );

	if( GetValue<int>( NUMBERDAY ) < 0 )
	{
		SetValue<bool>( FINISHED, true );
		OutputDebugString( std::string( "Закончено производство коробок с игрой " + game->GetValue<std::string>( NAME ) + "\n").c_str() );
	}
}
}//end namespace nrp 