#include "StdAfx.h"
#include "NrpGameBox.h"
#include "NrpGame.h"
#include "NrpTechnology.h"
#include "NrpApplication.h"

namespace nrp
{

CNrpGameBox::~CNrpGameBox(void)
{
}

CNrpGameBox::ADDON_LIST_ITERATOR CNrpGameBox::FindAddon_( const std::string& name )
{
	ADDON_LIST_ITERATOR aIter = addons_.begin();

	for( ; aIter != addons_.end(); aIter++ )
		if( (*aIter)->GetValue<std::string>(NAME) == name )
			return aIter;

	return addons_.end();
}

bool CNrpGameBox::IsMyBoxAddon( std::string name )
{
	return FindAddon_( name ) != addons_.end();
}

void CNrpGameBox::RemoveMyBoxAddon( std::string name )
{
	ADDON_LIST_ITERATOR& pIter = FindAddon_( name );
	if( pIter != addons_.end() )
	{
		addons_.erase( pIter );
		SetValue<int>( NUMBERADDON, addons_.size() );
		return;
	}

	OutputDebugString( ("Не нашел подходящего элемента = " + name).c_str() );
}

void CNrpGameBox::AddBoxAddon( CNrpTechnology* tech )
{
	if( tech == NULL )
	{
		OutputDebugString( "Нельзя добавить пустой аддон" );
		return;
	}
	addons_.push_back( tech );
	SetValue<int>( NUMBERADDON, addons_.size() );
}

CNrpTechnology* CNrpGameBox::GetAddon( int index )
{
	return index < addons_.size() ? addons_[ index ] : NULL;
}

void CNrpGameBox::Save( std::string scetionName, std::string fileName )
{
	ADDON_LIST_ITERATOR pIter = addons_.begin();
	for( int k=0; pIter != addons_.end(); pIter++, k++ )
		IniFile::Write( GAMEBOX, GAMEBOX + IntToStr( k ), (*pIter)->GetValue<std::string>( NAME ), fileName );

	INrpConfig::Save( scetionName, fileName );	
}

void CNrpGameBox::Load( std::string sectionName, std::string fileName )
{
	INrpConfig::Load( sectionName, fileName );

	for( int k=0; k < GetValue<int>( NUMBERADDON ); k++ )
	{
		std::string addonName = IniFile::Read( GAMEBOX, GAMEBOX + IntToStr( k ), std::string(""), fileName );
		CNrpTechnology* tech = CNrpApplication::Instance().GetBoxAddon( addonName );

		if( tech )
			AddBoxAddon( tech );
	}
}

nrp::CNrpGameBox::CNrpGameBox( CNrpGame* ptrGame ) : INrpConfig( "CNrpGameBox", "" )
{
	CreateValue<std::string>( NAME, ptrGame->GetValue<std::string>( NAME ) );
	CreateValue<PNrpGame>( BOXGAME, ptrGame );
	CreateValue<int>( NUMBERADDON, 0 );
	CreateValue<int>( LEVEL, 0 );
}

}//end of namespace nrp