#include "StdAfx.h"
#include "NrpReklameWork.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "NrpGame.h"

namespace nrp
{

CNrpReklameWork::CNrpReklameWork( const std::string& typeName, 
								  const std::string& gameName )
				:INrpConfig( CLASS_REKLAMEWORK, "" )
{
	InitializeOptions_();
	SetValue( TECHTYPE, typeName );
	SetValue( GAMENAME, gameName );
}

void CNrpReklameWork::InitializeOptions_()
{
	CreateValue<int>( NUMBERDAY, 0 );
	CreateValue<int>( LEVEL, 0 );
	CreateValue<std::string>( NAME, "" );
	CreateValue<std::string>( TECHTYPE, "" );
	CreateValue<int>( DAYCOST, 0 );
	CreateValue<float>( QUALITY, 0 );
	CreateValue<float>( MAXQUALITY, 0 );
	CreateValue<std::string>( GAMENAME, "" );
	CreateValue<bool>( FINISHED, false );
	CreateValue<std::string>( TEXTURENORMAL, "" );
	CreateValue<int>( BALANCE, 0 );
	CreateValue<std::string>( COMPANYNAME, "" );

	SYSTEMTIME rt;
	memset( &rt, 0, sizeof( rt ) );
	CreateValue<SYSTEMTIME>( STARTDATE, rt );
}

CNrpReklameWork::CNrpReklameWork( CNrpReklameWork& p ) : INrpConfig( CLASS_REKLAMEWORK, "" )
{
	InitializeOptions_();
	SetValue<int>( NUMBERDAY, p.GetValue<int>( NUMBERDAY ) );
	SetValue<int>( LEVEL, p.GetValue<int>( LEVEL ) );
	SetValue<std::string>( NAME, p.GetValue<std::string>( NAME ) );
	SetValue<int>( DAYCOST, p.GetValue<int>( DAYCOST ) );
	SetValue<float>( QUALITY, p.GetValue<float>( QUALITY ) );
	SetValue<float>( MAXQUALITY, p.GetValue<float>( MAXQUALITY ) );
	SetValue<std::string>( GAMENAME, p.GetValue<std::string>( GAMENAME ) );
	SetValue<std::string>( TECHTYPE, p.GetValue<std::string>( TECHTYPE ) );
	SetValue<std::string>( TEXTURENORMAL, p.GetValue<std::string>( TEXTURENORMAL ) );
	SetValue<std::string>( COMPANYNAME, p.GetValue<std::string>( COMPANYNAME ) );
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
	assert( cmp != NULL );

	if( cmp != NULL )
	{
		const std::string name = GetValue<std::string>( GAMENAME );
		//сначала попробуем получить куказатель на существующую игру
		INrpConfig* game = reinterpret_cast< INrpConfig* >( cmp->GetGame( name ) );

		//если такой игры нет, может игрок рекламирует проект игры??? проверим
		if( game == NULL )
			game = reinterpret_cast< INrpConfig* >( cmp->GetDevelopProject( name ) );

		//к этому моменту уже должны определиться чего хотим рекламировать, иначе ошибка
		assert( game != NULL );
		if( game == NULL )
			return;

		if( GetValue<int>( NUMBERDAY ) > 0 && 
			GetValue<float>( MAXQUALITY ) > game->GetValue<float>( FAMOUS ) )
		{	
			//здесь надо учесть факторы конторы, которые могут влиять на
			//повышение или понижение этого параметра
			game->AddValue<float>( FAMOUS, GetValue<float>( QUALITY ) );
			cmp->AddValue<int>( BALANCE, -GetValue<int>( DAYCOST ) );
		}
	}

	AddValue<int>( NUMBERDAY, -1 );
	AddValue<int>( BALANCE, GetValue<int>( DAYCOST ) );
	SetValue<bool>( FINISHED, GetValue<int>( NUMBERDAY ) == 0 );
}

void CNrpReklameWork::Save( std::string sectionName, std::string fileName )
{
	INrpConfig::Save( sectionName, fileName );
}

}//end namespace nrp