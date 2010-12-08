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
	_self[ REKLAMETYPE ] = typeName;
	_self[ GAMENAME ] = gameName;
}

void CNrpReklameWork::InitializeOptions_()
{
	Add<int>( NUMBERDAY, 0 );
	Add<int>( LEVEL, 0 );
	Add<NrpText>( NAME, "" );
	Add<int>( DAYCOST, 0 );
	Add<float>( QUALITY, 0.f );
	Add<float>( MAXQUALITY, 0.f );
	Add<NrpText>( GAMENAME, "" );
	Add<bool>( FINISHED, false );
	Add<NrpText>( TEXTURENORMAL, "" );
	Add<int>( BALANCE, 0 );
	Add<NrpText>( COMPANYNAME, "" );
	Add<NrpText>( TYPEOBJECT, "" );

	SYSTEMTIME rt;
	memset( &rt, 0, sizeof( rt ) );
	Add<SYSTEMTIME>( STARTDATE, rt );
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
	_self[ REKLAMETYPE ] = p[ REKLAMETYPE ];
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
	Param( NUMBERDAY ) += (int)(*p)[ NUMBERDAY ];
	Param( FINISHED ) = (int)Param( NUMBERDAY ) > 0;
}

void CNrpReklameWork::BeginNewDay()
{
	CNrpCompany* cmp = _nrpApp.GetCompany( Text( COMPANYNAME ) );
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
			(*game)[ FAMOUS ] += (float)Param( QUALITY );
		}
	}

	Param( NUMBERDAY ) += (int)-1;
	//AddValue<int>( BALANCE, GetValue<int>( DAYCOST ) );
	Param( FINISHED ) = (int)Param( NUMBERDAY ) == 0;
}

NrpText CNrpReklameWork::Save( const NrpText& saveFolder )
{
	assert( OpFileSystem::IsExist( saveFolder ) );
	//должно получиться что-то вроде Компания_Продукт.Тип
	NrpText fileName  = OpFileSystem::CheckEndSlash( saveFolder ) + Text( COMPANYNAME ) + "_";
	fileName += (Text( GAMENAME ) + "." + Text( REKLAMETYPE ));
	INrpConfig::Save( fileName );

	return fileName;
}

bool CNrpReklameWork::Equale( const NrpText& type, const NrpText& gName )
{
	return ( Text( REKLAMETYPE ) == type && Text( GAMENAME ) == gName );
}

NrpText CNrpReklameWork::ClassName()
{
	return CLASS_REKLAMEWORK;
}

}//end namespace nrp