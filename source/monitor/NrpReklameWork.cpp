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
	_self[ INTERNAL_NAME ] = typeName;
	_self[ GAMENAME ] = gameName;
}

void CNrpReklameWork::InitializeOptions_()
{
	RegProperty<NrpText>( CLASSOBJECT, CLASS_REKLAMEWORK );
	RegProperty( NUMBERDAY, 0 );
	RegProperty( LEVEL, 0 );
	RegProperty<NrpText>( NAME, "" );
	RegProperty( DAYCOST, 0 );
	RegProperty( QUALITY, 0.f );
	RegProperty( MAXQUALITY, 0.f );
	RegProperty<NrpText>( GAMENAME, "" );
	RegProperty( FINISHED, false );
	RegProperty<NrpText>( TEXTURENORMAL, "" );
	RegProperty( BALANCE, 0 );
	RegProperty<NrpText>( COMPANYNAME, "" );
	RegProperty<NrpText>( TYPEOBJECT, "" );
	RegProperty( POPULARITY, 0 );
	RegProperty<NrpText>( INTERNAL_NAME, "" );
	RegProperty( STARTDATE, NrpTime( 0. ) );
}

CNrpReklameWork::CNrpReklameWork( const CNrpReklameWork& p ) : INrpConfig( CLASS_REKLAMEWORK, "" )
{
	InitializeOptions_();

	_self[ NUMBERDAY ] = p[ NUMBERDAY ];
	_self[ BALANCE ] = p[ BALANCE ];
	_self[ LEVEL ] = p[ LEVEL ];
	_self[ NAME ] = p[ NAME ];
	_self[ DAYCOST ] = p[ DAYCOST ];
	_self[ QUALITY ] = p[ QUALITY ];
	_self[ MAXQUALITY ] = p[ MAXQUALITY ];
	_self[ TYPEOBJECT ] = p[ TYPEOBJECT ];
	_self[ GAMENAME ] = p[ GAMENAME ];
	_self[ INTERNAL_NAME ] = p[ INTERNAL_NAME ];
	_self[ TEXTURENORMAL ] = p[ TEXTURENORMAL ];
	_self[ COMPANYNAME ] = p[ COMPANYNAME ];
	
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

void CNrpReklameWork::Update( const CNrpReklameWork& p )
{
	_self[ NUMBERDAY ] += (int)p[ NUMBERDAY ];
	_self[ FINISHED ] = (int)_self[ NUMBERDAY ] > 0;
}

void CNrpReklameWork::BeginNewDay()
{
	CNrpCompany* cmp = _nrpApp.GetCompany( Text( COMPANYNAME ) );
	assert( cmp != NULL );

	if( cmp != NULL )
	{
		NrpText name = _self[ GAMENAME ];
		//сначала попробуем получить указатель на существующую игру
		INrpConfig* game = reinterpret_cast< INrpConfig* >( cmp->GetGame( name ) );

		//если такой игры нет, может игрок рекламирует проект игры??? проверим
		if( game == NULL )
			game = reinterpret_cast< INrpConfig* >( cmp->GetDevelopProject( name ) );

		//к этому моменту уже должны определиться чего хотим рекламировать, иначе ошибка
		assert( game != NULL );
		if( game == NULL )
			return;

		if( (int)_self[ NUMBERDAY ] > 0 && 
			(float)_self[ MAXQUALITY ] > (float)(*game)[ FAMOUS ] )
		{	
			//здесь надо учесть факторы конторы, которые могут влиять на
			//повышение или понижение этого параметра
			(*game)[ FAMOUS ] += (float)_self[ QUALITY ];
		}
	}

	_self[ NUMBERDAY ] += (int)-1;
	_self[ BALANCE ] += (int)_self[ DAYCOST ];
	_self[ FINISHED ] = (int)_self[ NUMBERDAY ] == 0;
}

NrpText CNrpReklameWork::Save( const NrpText& saveFolder )
{
	assert( OpFileSystem::IsExist( saveFolder ) );
	//должно получиться что-то вроде Компания_Продукт.Тип
	NrpText fileName  = OpFileSystem::CheckEndSlash( saveFolder ) + Text( COMPANYNAME ) + "_";
	fileName += (Text( GAMENAME ).Translit() + "." + Text( INTERNAL_NAME ));
	
	INrpConfig::Save( fileName );

	return fileName;
}

bool CNrpReklameWork::Equale( const NrpText& type, const NrpText& gName )
{
	return ( Text( INTERNAL_NAME ) == type && Text( GAMENAME ) == gName );
}

NrpText CNrpReklameWork::ClassName()
{
	return CLASS_REKLAMEWORK;
}

}//end namespace nrp