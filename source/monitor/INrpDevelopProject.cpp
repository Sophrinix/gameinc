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

	sv.Save();
	return amount;
}

void INrpDevelopProject::InitializeOptions_()
{
	Add<NrpText>( COMPANYNAME, "" );
	Add<int>( MODULE_NUMBER, 0 );
	Add<float>( READYWORKPERCENT, 0 );
	Add<bool>( PROJECTREADY, false );
	Add<NrpText>( TEXTURENORMAL, "" );
	Add<float>( FAMOUS, 0.f );
}

void INrpDevelopProject::SetDeveloper( CNrpUser& user )
{
	for( u32 i=0; i < _developers.size(); i++ )
		if( _developers[ i ] == (NrpText)user[ NAME ] )
			continue;

	_developers.push_back( (NrpText)user[ NAME ] );
	_self[ USERNUMBER ] = static_cast< int >( _developers.size() );
}

void INrpDevelopProject::Load( const NrpText& pathTo )
{
	INrpProject::Load( pathTo );

	IniFile rv( pathTo );
	rv.Get( SECTION_USERS, CreateKeyUser, (int)_self[ USERNUMBER ], _developers );
}

NrpText INrpDevelopProject::ClassName()
{
	return CLASS_INRPDEVELOPPROJECT;
}

}//end namespace name