#include "StdAfx.h"
#include "NrpGameBox.h"
#include "NrpGame.h"
#include "NrpTechnology.h"

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
		addons_.erase( pIter );

	OutputDebugString( ("Не нашел подходящего элемента = " + name).c_str() );
}

void CNrpGameBox::AddBoxAddon( CNrpTechnology* tech )
{
	addons_.push_back( tech );
}

nrp::CNrpGameBox::CNrpGameBox( CNrpGame* ptrGame ) : INrpConfig( "CNrpGameBox", "" )
{
	CreateValue<std::string>( NAME, ptrGame->GetValue<std::string>( NAME ) );
	CreateValue<PNrpGame>( BOXGAME, ptrGame );
}

}//end of namespace nrp