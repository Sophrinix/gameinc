#include "StdAfx.h"
#include "NrpInvention.h"
#include "NrpCompany.h"
#include "IUser.h"
#include "NrpTime.h"
#include "OpFileSystem.h"
#include "IniFile.h"
#include "NrpTime.h"

#include <OleAuto.h>

namespace nrp
{
CLASS_NAME CLASS_INVENTION( "CNrpInvention" );

int CNrpInvention::_GetRealPrice( const NrpTime& time )
{
	int month = _self[ STARTDATE ].As<NrpTime>().GetMonthToDate( time ); //�������� ���������� ������� �� ��������� ����� ��������� ���������� �� �����
	int price = _self[ BALANCE ];
	for( int k=0; k < month; k++ )//������ ������ ����� ��������� ���������� �� 10% �� ������� ���������( ������� ������� )
		 price = static_cast< int >( price * 1.1f );

	return price;
}

CNrpInvention::CNrpInvention( CNrpTechnology& refTech, CNrpCompany& pCmp, NrpTime time ) 
			  : IWorkingModule( static_cast< PROJECT_TYPE >( (int)refTech[ TECHGROUP ] ), CLASS_INVENTION )
{
	InitializeOptions_();

	_self[ BALANCE ] = refTech[ BALANCE ];
	_self[ PARENTCOMPANY ] = &pCmp;
	_self[ COMPANYNAME ] = pCmp[ NAME ];
	_self[ STATUS ] = static_cast< int >( TS_INDEVELOP );
	_self[ NAME ] = refTech[ NAME ];
	_self[ INTERNAL_NAME ] = refTech[ INTERNAL_NAME ];
	_self[ TECHGROUP ] = refTech[ TECHGROUP ];
	_self[ BASE_CODE ] = refTech[ BASE_CODE ];
	_self[ ENGINE_CODE ] = refTech[ ENGINE_CODE ];
	_self[ TEXTURENORMAL ] = refTech[ TEXTURENORMAL ];
	_self[ LEVEL ] = refTech[ LEVEL ];
	_self[ QUALITY ] = refTech[ QUALITY ];
	_self[ BASEFILE ] = refTech[ BASEFILE ];
	_self[ USERSTARTDATE ] = time;
	_self[ STARTDATE ] = refTech[ STARTDATE ];
	_self[ REALPRICE ] = _GetRealPrice( time );

	Add( PROGNOSEDATEFINISH, NrpTime( 0. ) );
	CheckParams( time );

		CopyMapTo( _techRequires, refTech.GetTechRequires() );
		CopyMapTo( _skillRequires,  refTech.GetSkillRequires() );
}

CNrpInvention::CNrpInvention( const NrpText& fileName ) 
	: IWorkingModule( PROJECT_TYPE(0), CLASS_INVENTION )
{
	InitializeOptions_();
	Load( fileName );
}

void CNrpInvention::InitializeOptions_()
{
	Add( REALPRICE, 0 );
	Add( PASSEDPRICE, 0 );
	Add( INVESTIMENT, 1000 );
	Add( DAYLEFT, 0 );
	Add( INVENTIONSPEED, 0 );
	Add( USERNUMBER, 0 );
	Add( COMPANYNAME, NrpText( "" ) );
	Add( PROGNOSEDATEFINISH, NrpTime( 0. ) );
	Add( USERSTARTDATE, NrpTime( 0. ) );
	Add( MONEY_TODECREASE, 0 );
}

void CNrpInvention::CheckParams( NrpTime time )
{
	int dayFromStart = _self[ USERSTARTDATE ].As<NrpTime>().GetDaysToDate( time ); 

	if( dayFromStart == 0 )
		dayFromStart++;

	//������� ����� ������ � ���� �� ������������
	int moneyInDay = (int)_self[ PASSEDPRICE ] / dayFromStart;
	//�������� ������� ���� �������� �� ����� ��� �������� ��������
	assert( moneyInDay > 0 );
	if( moneyInDay == 0 )
		moneyInDay = 1;
	int dayToFinish = ( (int)_self[ REALPRICE ] - (int)_self[ PASSEDPRICE ] ) / moneyInDay;
	
	_self[ PROGNOSEDATEFINISH ] = time.AppendDay( dayToFinish );
	_self[ DAYLEFT ] = dayToFinish;//������� ���� �������� �� ���������� �����, ��� ������ �������� ��������������
	_self[ INVENTIONSPEED ] = moneyInDay;
}

CNrpInvention::~CNrpInvention(void)
{
}

void CNrpInvention::Update( CNrpUser& ptrUser, const NrpTime& time )
{
	PNrpCompany company = _self[ PARENTCOMPANY ].As<PNrpCompany>();
	assert( company );

	if( !company )
		return;

	if( (int)_self[ PASSEDPRICE ] < (int)_self[ REALPRICE ] )
	{
		float koeff = (int)_self[ INVESTIMENT ] / static_cast< float >( (int)_self[ REALPRICE ] );

		float sumSkill = (std::max)( (int)ptrUser[ ALL_SKILL_SUMM ] * ( koeff + 1 ), (float)10 );

		//��� �������� ������� ����� �� �������� ����� ����������
		_self[ PASSEDPRICE ] += static_cast<int>( sumSkill );
		//��� ����� ���� ������� � ������� � ����� ������
		_self[ MONEY_TODECREASE ] += (int)_self[ INVESTIMENT ] / ( 30 * 8 );
	
		_self[ READYWORKPERCENT ] = (int)_self[PASSEDPRICE] / static_cast< float >( (int)_self[ REALPRICE ] );
		_self[ QUALITY ] = ( (int)_self[ QUALITY ] + (int)ptrUser[ WORK_QUALITY ] ) / 2;
	}
}

CNrpUser* CNrpInvention::GetUser( u32 index )
{
	assert( index < _users.size() );
	return index < _users.size() ? _users[ index ] : NULL;
}

int CNrpInvention::IsMyUser( CNrpUser& user )
{
	for( u32 i=0; i < _users.size(); i++ )
		if( *(_users[ i ]) == user )
			return true;

	return false;
}

int CNrpInvention::AddUser( CNrpUser& user )
{
	if( IsMyUser( user ) )
		return 1;
	
	_users.push_back( &user );
	_self[ USERNUMBER ] = static_cast< int >(_users.size() );

	user.AddWork( *this, true );
	return 0;
}

int CNrpInvention::RemoveUser( const NrpText& userName )
{
	for( u32 i=0; i < _users.size(); i++ )
		if( _users[ i ]->Equale( userName ) )
			_users.erase( i );

	return 1;
}

void CNrpInvention::Load( const NrpText& fileName )
{
	INrpProject::Load( fileName );
	IniFile rv( fileName );

	rv.Get( SECTION_REQUIRE_TECH, _techRequires );
	rv.Get( SECTION_REQUIRE_SKILL, _skillRequires );
}

NrpText CNrpInvention::_GetFileName()
{
	return (NrpText)Text( INTERNAL_NAME ) + ".invent";
}

NrpText CNrpInvention::Save( const NrpText& saveFolder, bool k )
{
	NrpText myFolder = OpFileSystem::CheckEndSlash( saveFolder );

	OpFileSystem::CreateDirectory( myFolder );

	NrpText fileName = myFolder + _GetFileName();

	assert( !OpFileSystem::IsExist( fileName ) );
	INrpProject::Save( fileName );

	IniFile sv( fileName );
	assert( OpFileSystem::IsExist( fileName ) ); 
	
	sv.Set( SECTION_REQUIRE_TECH, _techRequires );
	sv.Set( SECTION_REQUIRE_SKILL, _skillRequires );
	sv.Set( SECTION_USERS, _users, CreateKeyUser, NAME );

	sv.Save();
	return fileName;
}

bool CNrpInvention::Equale( const NrpText& name, const NrpText& company )
{
	return ( Text( NAME ) == name || Text( INTERNAL_NAME ) == name ) && ( Text( COMPANYNAME ) == company );
}

NrpText CNrpInvention::ClassName()
{
	return CLASS_INVENTION;
}

void CNrpInvention::BeginNewMonth( const NrpTime& time )
{
	_self[ REALPRICE ] = _GetRealPrice( time );
}

}//end namespace nrp