#include "StdAfx.h"
#include "NrpReklameWork.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "NrpGame.h"

namespace nrp
{

CNrpReklameWork::CNrpReklameWork( std::string typeName, std::string gameName ):INrpConfig( CLASS_REKLAMEWORK, "" )
{
	InitializeOptions_();
	SetValue<std::string>( TECHTYPE, typeName );
	SetValue<std::string>( GAMENAME, gameName );
}

void CNrpReklameWork::InitializeOptions_()
{
	CreateValue<int>( NUMBERDAY, 0 );
	CreateValue<int>( LEVEL, 0 );
	CreateValue<std::string>( NAME, "" );
	CreateValue<std::string>( TECHTYPE, "" );
	CreateValue<int>( PRICEINDAY, 0 );
	CreateValue<float>( QUALITY, 0 );
	CreateValue<float>( MAXQUALITY, 0 );
	CreateValue<std::string>( GAMENAME, "" );
	CreateValue<bool>( FINISHED, false );
}

CNrpReklameWork::CNrpReklameWork( CNrpReklameWork& p ) : INrpConfig( CLASS_REKLAMEWORK, "" )
{
	InitializeOptions_();
	SetValue<int>( NUMBERDAY, p.GetValue<int>( NUMBERDAY ) );
	SetValue<int>( LEVEL,  p.GetValue<int>( LEVEL ) );
	SetValue<std::string>( NAME,  p.GetValue<std::string>( NAME ) );
	SetValue<int>( PRICEINDAY,  p.GetValue<int>( PRICEINDAY ) );
	SetValue<float>( QUALITY,  p.GetValue<float>( QUALITY ) );
	SetValue<float>( MAXQUALITY,  p.GetValue<float>( MAXQUALITY ) );
	SetValue<std::string>( GAMENAME,  p.GetValue<std::string>( GAMENAME ) );
	SetValue<std::string>( TECHTYPE, p.GetValue<std::string>( TECHTYPE ) );
}

CNrpReklameWork::~CNrpReklameWork(void)
{
}

void CNrpReklameWork::Load( std::string sectionName, std::string fileName )
{
	INrpConfig::Load( sectionName, fileName );
}

void CNrpReklameWork::Update( CNrpReklameWork* p )
{
	AddValue<int>( NUMBERDAY, p->GetValue<int>( NUMBERDAY ) );
	SetValue<bool>( FINISHED, GetValue<int>( NUMBERDAY ) > 0 );
}

void CNrpReklameWork::BeginNewDay()
{
	CNrpCompany* cmp = CNrpApplication::Instance().GetCompany( GetValue<std::string>( COMPANYNAME ) );

	if( cmp != NULL )
	{
		CNrpGame* game = cmp->GetGame( GetValue<std::string>( GAMENAME ) );
		if( GetValue<int>( NUMBERDAY ) > 0 && 
			GetValue<float>( MAXQUALITY ) > game->GetValue<float>( FAMOUS ) )
		{	
			//здесь надо учесть факторы конторы, которые могут влиять на
			//повышение или понижение этого параметра
			game->AddValue<float>( FAMOUS, GetValue<float>( QUALITY ) );
			cmp->AddValue<int>( BALANCE, -GetValue<int>( PRICEINDAY ) );
		}
	}

	AddValue<int>( NUMBERDAY, -1 );
	SetValue<bool>( FINISHED, GetValue<int>( NUMBERDAY ) > 0 );
}
}//end namespace nrp