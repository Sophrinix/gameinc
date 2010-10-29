#include "StdAfx.h"
#include "INrpDevelopProject.h"
#include "IUser.h"

namespace nrp
{

INrpDevelopProject::INrpDevelopProject(void) : INrpProject( CLASS_INRPDEVELOPPROJECT, "" )
{
}

INrpDevelopProject::INrpDevelopProject( const std::string& className, const std::string& systemName ) : INrpProject( className, systemName )
{
	InitializeOptions_();
}

INrpDevelopProject::~INrpDevelopProject(void)
{
}

std::string INrpDevelopProject::Save( const std::string& amount )
{
	INrpProject::Save( amount );

	for( size_t i=0; i < developers_.size(); i++ )
		IniFile::Write( SECTION_USERS, KEY_USER( i ), developers_[ i ], amount );

	return amount;
}

void INrpDevelopProject::InitializeOptions_()
{
	CreateValue<std::string>( COMPANYNAME, "" );
	CreateValue<int>( MODULE_NUMBER, 0 );
	CreateValue<float>( READYWORKPERCENT, 0 );
	CreateValue<bool>( PROJECTREADY, false );
	CreateValue<int>( TECHTYPE, 0 );
	CreateValue<std::string>( TEXTURENORMAL, "" );
	CreateValue<float>( FAMOUS, 0.f );
}

void INrpDevelopProject::SetDeveloper( IUser* user )
{
	for( size_t i=0; i < developers_.size(); i++ )
		if( developers_[ i ] == user->GetString( NAME ) )
			continue;

	developers_.push_back( user->GetString( NAME ) );
	SetValue<int>( USERNUMBER, developers_.size() );
}

void INrpDevelopProject::Load( const std::string& pathTo )
{
	INrpProject::Load( pathTo );

	for( int i=0; i < GetValue<int>( USERNUMBER ); i++ )
		developers_.push_back( IniFile::Read( SECTION_USERS, KEY_USER( i ), std::string(""), pathTo ) );
}

}//end namespace name