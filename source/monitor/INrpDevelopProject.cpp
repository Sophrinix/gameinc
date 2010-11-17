#include "StdAfx.h"
#include "INrpDevelopProject.h"
#include "IUser.h"
#include "IniFile.h"

namespace nrp
{
CLASS_NAME CLASS_INRPDEVELOPPROJECT( L"INrpDevelopProject" );

INrpDevelopProject::INrpDevelopProject(void) : INrpProject( CLASS_INRPDEVELOPPROJECT, "" )
{
}

INrpDevelopProject::INrpDevelopProject( const NrpText& className, const NrpText& systemName ) 
: INrpProject( className, systemName )
{
	InitializeOptions_();
}

INrpDevelopProject::~INrpDevelopProject(void)
{
}

NrpText INrpDevelopProject::Save( const NrpText& amount )
{
	INrpProject::Save( amount );

	IniFile sv( amount );
	sv.Set( SECTION_USERS, CreateKeyUser, _developers );

	return amount;
}

void INrpDevelopProject::InitializeOptions_()
{
	Push<NrpText>( COMPANYNAME, "" );
	Push<int>( MODULE_NUMBER, 0 );
	Push<float>( READYWORKPERCENT, 0 );
	Push<bool>( PROJECTREADY, false );
	Push<int>( TECHTYPE, 0 );
	Push<NrpText>( TEXTURENORMAL, "" );
	Push<float>( FAMOUS, 0.f );
}

void INrpDevelopProject::SetDeveloper( IUser* user )
{
	for( u32 i=0; i < _developers.size(); i++ )
		if( _developers[ i ] == user->GetString( NAME ) )
			continue;

	_developers.push_back( user->GetString( NAME ) );
	SetValue<int>( USERNUMBER, _developers.size() );
}

void INrpDevelopProject::Load( const NrpText& pathTo )
{
	INrpProject::Load( pathTo );

	IniFile rv( pathTo );
	rv.Get( SECTION_USERS, CreateKeyUser, GetValue<int>( USERNUMBER ), _developers );
}

NrpText INrpDevelopProject::ClassName()
{
	return CLASS_INRPDEVELOPPROJECT;
}

}//end namespace name