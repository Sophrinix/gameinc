#include "StdAfx.h"
#include "NrpInvention.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "IUser.h"

#include <OleAuto.h>
#include <io.h>


namespace nrp
{

int CNrpInvention::_GetRealPrice( CNrpTechnology* tech )
{
	double stime, curtime;
	int errSTime = SystemTimeToVariantTime( &tech->GetValue<SYSTEMTIME>( STARTDATE ), &stime );
	int errCurrTime = SystemTimeToVariantTime( &CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME ), &curtime );
	assert( errSTime > 0 && errCurrTime > 0 );
	
	int month = static_cast<int>( stime - curtime ) / 30; //�������� ���������� ������� �� ��������� ����� ��������� ���������� �� �����
	float price = static_cast< float >( tech->GetValue<int>( BALANCE ) );
	for( int k=0; k < month; k++ )//������ ������ ����� ��������� ���������� �� 10% �� ������� ���������( ������� ������� )
		 price *= 1.1f;

	return static_cast<int>( price );
}

CNrpInvention::CNrpInvention( CNrpTechnology* pTech, CNrpCompany* pCmp ) 
			  : IWorkingModule( pTech->GetValue<PROJECT_TYPE>( TECHGROUP ), CLASS_INVENTION )
{
	InitializeOptions_();

	if( pTech && pCmp )
	{
		SetValue<int>( REALPRICE, _GetRealPrice( pTech ) );
		SetValue<PNrpCompany>( PARENTCOMPANY, pCmp );
		SetValue<std::string>( COMPANYNAME, pCmp->GetValue<std::string>( NAME ) );
		SetValue<TECH_STATUS>( STATUS, TS_INDEVELOP );
		SetValue<std::string>( NAME, pTech->GetValue<std::string>( NAME ) );
		SetValue<std::string>( INTERNAL_NAME, pTech->GetValue<std::string>( INTERNAL_NAME ) );
		SetValue<int>( TECHGROUP, pTech->GetValue<int>( TECHGROUP ) );
		SetValue<PROJECT_TYPE>( TECHTYPE, pTech->GetValue<PROJECT_TYPE>( TECHTYPE ) );
		SetValue<float>( BASE_CODE, pTech->GetValue<float>( BASE_CODE ) );
		SetValue<float>( ENGINE_CODE, pTech->GetValue<float>( ENGINE_CODE ) );
		SetValue<std::string>( TEXTURENORMAL, pTech->GetValue<std::string>( TEXTURENORMAL ) );
		SetValue<int>( LEVEL, pTech->GetValue<int>( LEVEL ) );
		SetValue<int>( QUALITY, pTech->GetValue<int>( QUALITY ) );

		SYSTEMTIME time;
		memset( &time, 0, sizeof(SYSTEMTIME) );
		CreateValue<SYSTEMTIME>( PROGNOSEDATEFINISH, time );
		CheckParams();

		techRequires_ = pTech->GetTechRequires();
		skillRequires_ = pTech->GetSkillRequires();
	}
}

CNrpInvention::CNrpInvention( const std::string& fileName ) 
			  : IWorkingModule( PROJECT_TYPE(0), CLASS_INVENTION )
{
	InitializeOptions_();
	Load( fileName );
}

void CNrpInvention::InitializeOptions_()
{
	CreateValue<int>( REALPRICE, 0 );
	CreateValue<int>( PASSEDPRICE, 0 );
	CreateValue<int>( INVESTIMENT, 1000 );
	CreateValue<int>( DAYLEFT, 0 );
	CreateValue<int>( INVENTIONSPEED, 0 );
	CreateValue<int>( USERNUMBER, 0 );
	CreateValue<std::string>( COMPANYNAME, "" );
}

void CNrpInvention::CheckParams()
{
	int needMoney = GetValue<int>( REALPRICE ) - GetValue<int>( PASSEDPRICE);
	int dayToFinish = ( needMoney / GetValue<int>( INVESTIMENT )) * 30;

	double time;
	int errCurrTime = SystemTimeToVariantTime( &CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME ), &time );
	assert( errCurrTime > 0 );
	time += dayToFinish;

	VariantTimeToSystemTime( time, &GetValue<SYSTEMTIME>( PROGNOSEDATEFINISH ) );//������� ���������� �����
	SetValue<int>( DAYLEFT, dayToFinish );//������� ���� �������� �� ���������� �����, ��� ������ �������� ��������������
	
	int speed = ( GetValue<int>( INVESTIMENT ) * 3 ) / GetValue<int>( REALPRICE ) * 100;
	SetValue<int>( INVENTIONSPEED, speed );
}

CNrpInvention::~CNrpInvention(void)
{
}

void CNrpInvention::Update( IUser* ptrUser )
{
	PNrpCompany company = GetValue<PNrpCompany>( PARENTCOMPANY );
	assert( company != NULL );

	if( GetValue<int>( PASSEDPRICE ) < GetValue<int>( REALPRICE ) )
	{
		int reqSkill = 0;
		REQUIRE_MAP::iterator sIter = skillRequires_.begin();
		for( ; sIter != skillRequires_.end(); sIter++ )
			reqSkill += ptrUser->GetSkill( sIter->first );
		assert( reqSkill != 0 );

		float genreSkill = ptrUser->GetGenreExperience( GetValue<PROJECT_TYPE>( TECHTYPE ) ) / 100.f;

		if( genreSkill < 0.1f )
			genreSkill = 0.1f;
		float genrePref = ptrUser->GetGenrePreferences( GetValue<PROJECT_TYPE>( TECHTYPE ) ) / 100.f;
		if( genrePref < 0.1f )
			genrePref = 0.1f;

		float updateMoney = reqSkill * (genrePref + genreSkill) / ( 30.f * 8 );
		float maxUpdateMoney = ptrUser->GetValue<int>( SALARY ) / 8.f;
		float money = updateMoney * GetValue<int>( INVESTIMENT ) / ( 30.f * 8 );
		//������� ����� ����������� ���������� � ���� �� ����� �� ������ ����� �������� ��������
		money = min( money, maxUpdateMoney );

		//��� �������� ������� ����� �� �������� ����� ����������
		AddValue<int>( PASSEDPRICE, static_cast<int>( money ) );
	
		SetValue<float>( READYWORKPERCENT, GetValue<int>(PASSEDPRICE) / static_cast< float >( GetValue<int>( REALPRICE ) ) );
		AddValue<int>( QUALITY, (GetValue<int>( QUALITY ) + ptrUser->GetValue<int>( CODE_QUALITY )) / 2 );
	}

	if( GetValue<float>( READYWORKPERCENT ) >= 1 )
		CNrpApplication::Instance().InventionFinished( this );
}

IUser* CNrpInvention::GetUser( size_t index )
{
	assert( index < _users.size() );
	return index < _users.size() ? _users[ index ] : NULL;
}

int CNrpInvention::AddUser( IUser* user )
{
	USERS_LIST::iterator pIter = _users.begin();
	for( ; pIter != _users.end(); pIter++ )
		if( *pIter == user )
			return 1;
	
	_users.push_back( user );
	SetValue<int>( USERNUMBER, _users.size() );
	user->AddWork( this, false );
	return 0;
}

int CNrpInvention::RemoveUser( const std::string& userName )
{
	USERS_LIST::iterator pIter = _users.begin();
	for( ; pIter != _users.end(); pIter++ )
		if( (*pIter)->GetValue<std::string>( NAME ) == userName )
		{
			_users.erase( pIter );
			return 0;
		}

	return 1;
}

void CNrpInvention::Load( std::string fileName )
{
	INrpProject::Load( SECTION_PROPERTIES, fileName );
	LoadRequries_( fileName );
}

void CNrpInvention::Save( std::string saveFolder )
{
	if( _access( saveFolder.c_str(), 0 ) == -1 )
		CreateDirectory( saveFolder.c_str(), NULL );

	std::string fileName = saveFolder + GetValue<std::string>( NAME ) + ".invent";

	DeleteFile( fileName.c_str() );
	INrpProject::Save( SECTION_PROPERTIES, fileName );
	SaveRequires_( fileName );

	for( size_t pos=0; pos < _users.size(); pos++ )
		IniFile::Write( SECTIONS_USERS, "user_" + IntToStr( pos ), _users[ pos ]->GetValue<std::string>( NAME ), fileName );
}
}//end namespace nrp