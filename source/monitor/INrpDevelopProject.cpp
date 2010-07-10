#include "StdAfx.h"
#include "INrpDevelopProject.h"
#include "IUser.h"

namespace nrp
{

INrpDevelopProject::INrpDevelopProject(void) : INrpProject( CLASS_INRPDEVELOPPROJECT, "" )
{
	InitializeOptions_();
}

INrpDevelopProject::~INrpDevelopProject(void)
{
}

void INrpDevelopProject::Save( std::string amount )
{
	INrpProject::Save( PROPERTIES, amount );

	for( size_t i=0; i < developers_.size(); i++ )
		IniFile::Write( "developers", "user_" + IntToStr( i ), developers_[ i ], amount );
}

void INrpDevelopProject::InitializeOptions_()
{
	CreateValue<std::string>( COMPANYNAME, "" );
	CreateValue<int>( MODULE_NUMBER, 0 );
	CreateValue<bool>( PROJECTREADY, false );
	CreateValue<int>( TECHTYPE, 0 );
}

void INrpDevelopProject::SetDeveloper( IUser* user )
{
	for( size_t i=0; i < developers_.size(); i++ )
		if( developers_[ i ] == user->GetValueA<std::string>( NAME ) )
			return;

	developers_.push_back( user->GetValueA<std::string>( NAME ) );
}
}//end namespace name