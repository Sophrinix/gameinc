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
	Param( TECHTYPE ) = typeName;
	Param( GAMENAME ) = gameName;
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

CNrpReklameWork::CNrpReklameWork( const CNrpReklameWork& p ) : INrpConfig( CLASS_REKLAMEWORK, "" )
{
	InitializeOptions_();
	Param( NUMBERDAY ) = p[ NUMBERDAY ];
	Param( LEVEL ) = p[ LEVEL ];
	Param( NAME ) = p[ NAME ];
	Param( DAYCOST ) = p[ DAYCOST ];
	Param( QUALITY ) = p[ QUALITY ];
	Param( MAXQUALITY ) = p[ MAXQUALITY ];
	Param( GAMENAME ) = p[ GAMENAME ];
	Param( TECHTYPE ) = p[ TECHTYPE ];
	Param( TEXTURENORMAL ) = p[ TEXTURENORMAL ];
	Param( COMPANYNAME ) = p[ COMPANYNAME ];
	
	//надо проверять корректность загруженных данных
	assert( Text( GAMENAME ).size() );
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
	Param( NUMBERDAY ).As<int>() += (int)(*p)[ NUMBERDAY ];
	Param( FINISHED ) = (int)Param( NUMBERDAY ) > 0;
}

void CNrpReklameWork::BeginNewDay()
{
	CNrpCompany* cmp = CNrpApplication::Instance().GetCompany( Text( COMPANYNAME ) );
	assert( cmp != NULL );

	if( cmp != NULL )
	{
		NrpText name = Text( GAMENAME );
		//сначала попробуем получить указатель на существующую игру
		INrpConfig* game = reinterpret_cast< INrpConfig* >( cmp->GetGame( name ) );

		//если такой игры нет, может игрок рекламирует проект игры??? проверим
		if( game == NULL )
			game = reinterpret_cast< INrpConfig* >( cmp->GetDevelopProject( name ) );

		//к этому моменту уже должны определиться чего хотим рекламировать, иначе ошибка
		assert( game != NULL );
		if( game == NULL )
			return;

		if( (int)Param( NUMBERDAY ) > 0 && 
			(float)Param( MAXQUALITY ) > (float)(*game)[ FAMOUS ] )
		{	
			//здесь надо учесть факторы конторы, которые могут влиять на
			//повышение или понижение этого параметра
			(*game)[ FAMOUS ].As<float>() += (float)Param( QUALITY );
		}
	}

	Param( NUMBERDAY ) += (int)-1;
	//AddValue<int>( BALANCE, GetValue<int>( DAYCOST ) );
	Param( FINISHED ) = Param( NUMBERDAY ).As<int>() == 0;
}

NrpText CNrpReklameWork::Save( const NrpText& saveFolder )
{
	assert( OpFileSystem::IsExist( saveFolder ) );
	//должно получиться что-то вроде Компания_Продукт.Тип
	NrpText fileName  = OpFileSystem::CheckEndSlash( saveFolder ) + Text( COMPANYNAME ) + "_";
	fileName += (Text( GAMENAME ) + "." + Text( TECHTYPE ));
	INrpConfig::Save( fileName );

	return fileName;
}

bool CNrpReklameWork::Equale( const NrpText& type, const NrpText& gName )
{
	return ( Text( TECHTYPE ) == type && Text( GAMENAME ) == gName );
}

NrpText CNrpReklameWork::ClassName()
{
	return CLASS_REKLAMEWORK;
}

}//end namespace nrp