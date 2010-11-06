#include "StdAfx.h"
#include "NrpReklameWork.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "NrpGame.h"
#include "OpFileSystem.h"

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
	CreateValue<std::string>( TYPEOBJECT, "" );

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
	SetString( GAMENAME, p.GetString( GAMENAME ) );
	SetString( TECHTYPE, p.GetString( TECHTYPE ) );
	SetString( TEXTURENORMAL, p.GetString( TEXTURENORMAL ) );
	SetString( COMPANYNAME, p.GetString( COMPANYNAME ) );
	
	//надо проверять корректность загруженных данных
	assert( !GetString( GAMENAME ).empty() );
}

CNrpReklameWork::CNrpReklameWork( const std::string& fileName ) : INrpConfig( CLASS_REKLAMEWORK, "" )
{
	InitializeOptions_();
	Load( fileName );
}

CNrpReklameWork::~CNrpReklameWork(void)
{
}

void CNrpReklameWork::Load( const std::string& fileName )
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
	CNrpCompany* cmp = CNrpApplication::Instance().GetCompany( GetValue<std::string>( COMPANYNAME ) );
	assert( cmp != NULL );

	if( cmp != NULL )
	{
		const std::string name = GetValue<std::string>( GAMENAME );
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

std::string CNrpReklameWork::Save( const std::string& saveFolder )
{
	assert( OpFileSystem::IsExist( saveFolder ) );
	//должно получиться что-то вроде Компания_Продукт.Тип
	std::string fileName  = OpFileSystem::CheckEndSlash( saveFolder ) + GetString( COMPANYNAME ) + "_";
	fileName += (GetString( GAMENAME ) + "." + GetString( TECHTYPE ));
	INrpConfig::Save( fileName );

	return fileName;
}
}//end namespace nrp