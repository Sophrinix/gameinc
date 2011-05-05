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
	Add<int>( POPULARITY, 0 );
	Add<NrpText>( INTERNAL_NAME, "" );
	Add( STARTDATE, NrpTime( 0. ) );
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
	_self[ GAMENAME ] = p[ GAMENAME ];
	_self[ INTERNAL_NAME ] = p[ INTERNAL_NAME ];
	_self[ TEXTURENORMAL ] = p[ TEXTURENORMAL ];
	_self[ COMPANYNAME ] = p[ COMPANYNAME ];
	
	//���� ��������� ������������ ����������� ������
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
		//������� ��������� �������� ��������� �� ������������ ����
		INrpConfig* game = reinterpret_cast< INrpConfig* >( cmp->GetGame( name ) );

		//���� ����� ���� ���, ����� ����� ����������� ������ ����??? ��������
		if( game == NULL )
			game = reinterpret_cast< INrpConfig* >( cmp->GetDevelopProject( name ) );

		//� ����� ������� ��� ������ ������������ ���� ����� �������������, ����� ������
		assert( game != NULL );
		if( game == NULL )
			return;

		if( (int)_self[ NUMBERDAY ] > 0 && 
			(float)_self[ MAXQUALITY ] > (float)(*game)[ FAMOUS ] )
		{	
			//����� ���� ������ ������� �������, ������� ����� ������ ��
			//��������� ��� ��������� ����� ���������
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
	//������ ���������� ���-�� ����� ��������_�������.���
	NrpText fileName  = OpFileSystem::CheckEndSlash( saveFolder ) + Text( COMPANYNAME ) + "_";
	fileName += (Text( GAMENAME ) + "." + Text( INTERNAL_NAME ));
	
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