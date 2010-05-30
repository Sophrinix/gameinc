#include "StdAfx.h"
#include "NrpReklameWork.h"

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
}
}//end namespace nrp