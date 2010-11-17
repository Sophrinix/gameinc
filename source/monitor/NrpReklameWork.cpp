#include "StdAfx.h"
#include "NrpReklameWork.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "NrpGame.h"
#include "OpFileSystem.h"

namespace nrp
{
CLASS_NAME CLASS_REKLAMEWORK( "CNrpReklameWork" );

CNrpReklameWork::CNrpReklameWork( const NrpText& typeName, 
								  const NrpText& gameName )
				: INrpConfig( CLASS_REKLAMEWORK, "" )
{
	InitializeOptions_();
	SetValue( TECHTYPE, typeName );
	SetValue( GAMENAME, gameName );
}

void CNrpReklameWork::InitializeOptions_()
{
	Push<int>( NUMBERDAY, 0 );
	Push<int>( LEVEL, 0 );
	Push<NrpText>( NAME, "" );
	Push<NrpText>( TECHTYPE, "" );
	Push<int>( DAYCOST, 0 );
	Push<float>( QUALITY, 0 );
	Push<float>( MAXQUALITY, 0 );
	Push<NrpText>( GAMENAME, "" );
	Push<bool>( FINISHED, false );
	Push<NrpText>( TEXTURENORMAL, "" );
	Push<int>( BALANCE, 0 );
	Push<NrpText>( COMPANYNAME, "" );
	Push<NrpText>( TYPEOBJECT, "" );

	SYSTEMTIME rt;
	memset( &rt, 0, sizeof( rt ) );
	Push<SYSTEMTIME>( STARTDATE, rt );
}

CNrpReklameWork::CNrpReklameWork( CNrpReklameWork& p ) : INrpConfig( CLASS_REKLAMEWORK, "" )
{
	InitializeOptions_();
	SetValue<int>( NUMBERDAY, p.GetValue<int>( NUMBERDAY ) );
	SetValue<int>( LEVEL, p.GetValue<int>( LEVEL ) );
	SetString( NAME, p.GetString( NAME ) );
	SetValue<int>( DAYCOST, p.GetValue<int>( DAYCOST ) );
	SetValue<float>( QUALITY, p.GetValue<float>( QUALITY ) );
	SetValue<float>( MAXQUALITY, p.GetValue<float>( MAXQUALITY ) );
	SetString( GAMENAME, p.GetString( GAMENAME ) );
	SetString( TECHTYPE, p.GetString( TECHTYPE ) );
	SetString( TEXTURENORMAL, p.GetString( TEXTURENORMAL ) );
	SetString( COMPANYNAME, p.GetString( COMPANYNAME ) );
	
	//надо проверять корректность загруженных данных
	assert( GetString( GAMENAME ).size() );
}

CNrpReklameWork::CNrpReklameWork( const NrpText& fileName ) : INrpConfig( CLASS_REKLAMEWORK, "" )
{
	InitializeOptions_();
	Load( fileName );
}

CNrpReklameWork::~CNrpReklameWork(void)
{
}

void CNrpReklameWork::Load( const NrpText& fileName )
{
	INrpConfig::Load( fileName );
}

void CNrpReklameWork::Update( const CNrpReklameWork* p )
{
	AddValue<int>( NUMBERDAY, p->GetValue<int>( NUMBERDAY ) );
	SetValue<bool>( FINISHED, GetValue<int>( NUMBERDAY ) > 0 );
}

void CNrpReklameWork::BeginNewDay()
{
	CNrpCompany* cmp = CNrpApplication::Instance().GetCompany( GetString( COMPANYNAME ) );
	assert( cmp != NULL );

	if( cmp != NULL )
	{
		NrpText name = GetString( GAMENAME );
		//сначала попробуем получить указатель на существующую игру
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
		}
	}

	AddValue<int>( NUMBERDAY, -1 );
	//AddValue<int>( BALANCE, GetValue<int>( DAYCOST ) );
	SetValue<bool>( FINISHED, GetValue<int>( NUMBERDAY ) == 0 );
}

NrpText CNrpReklameWork::Save( const NrpText& saveFolder )
{
	assert( OpFileSystem::IsExist( saveFolder ) );
	//должно получиться что-то вроде Компания_Продукт.Тип
	NrpText fileName  = OpFileSystem::CheckEndSlash( saveFolder ) + GetString( COMPANYNAME ) + "_";
	fileName += (GetString( GAMENAME ) + "." + GetString( TECHTYPE ));
	INrpConfig::Save( fileName );

	return fileName;
}

bool CNrpReklameWork::Equale( const NrpText& type, const NrpText& gName )
{
	return ( GetString( TECHTYPE ) == type && GetString( GAMENAME ) == gName );
}

NrpText CNrpReklameWork::ClassName()
{
	return CLASS_REKLAMEWORK;
}

}//end namespace nrp