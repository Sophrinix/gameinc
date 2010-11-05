#include "StdAfx.h"
#include <io.h>
#include "NrpGameBox.h"
#include "NrpGame.h"
#include "NrpTechnology.h"
#include "NrpApplication.h"

namespace nrp
{

CNrpGameBox::~CNrpGameBox(void)
{
}

CNrpGameBox::ADDON_LIST_ITERATOR CNrpGameBox::_FindAddon( const std::string& name )
{
	ADDON_LIST_ITERATOR aIter = _addons.begin();

	for( ; aIter != _addons.end(); aIter++ )
		if( (*aIter)->GetString(NAME) == name )
			return aIter;

	return _addons.end();
}

bool CNrpGameBox::IsMyAddon( const std::string& name )
{
	return _FindAddon( name ) != _addons.end();
}

void CNrpGameBox::RemoveAddon( const std::string& name )
{
	ADDON_LIST_ITERATOR& pIter = _FindAddon( name );
	if( pIter != _addons.end() )
	{
		_addons.erase( pIter );
		SetValue<int>( NUMBERADDON, _addons.size() );
		return;
	}

	Log(HW) << "Не нашел подходящего элемента = " << name << term;
}

int CNrpGameBox::_GetAddonSumLevel()
{
	int ret=0;
	for( ADDON_LIST::const_iterator pIter=_addons.begin();
		 pIter != _addons.end(); pIter++ )
		ret += (*pIter)->GetValue<int>( LEVEL );

	return ret;
}

bool CNrpGameBox::AddAddon( CNrpBoxAddon* tech )
{
	assert( tech != NULL );
	if( tech == NULL )
	{
		Log(HW) << "Нельзя добавить пустой аддон" << term;
		return false;
	}

	if( _GetAddonSumLevel() + tech->GetValue<int>( LEVEL ) < GetValue<int>( LEVEL ) )
	{
		_addons.push_back( tech );
		SetValue<int>( NUMBERADDON, _addons.size() );
		return true;
	}
	
	return false;
}

CNrpBoxAddon* CNrpGameBox::GetAddon( size_t index )
{
	return index < _addons.size() ? _addons[ index ] : NULL;
}

std::string CNrpGameBox::Save( const std::string& fileName )
{
	ADDON_LIST_ITERATOR pIter = _addons.begin();
	for( int k=0; pIter != _addons.end(); pIter++, k++ )
		IniFile::Write( SECTION_ADDONS, KEY_ADDON( k ), (*pIter)->GetString( NAME ), fileName );

	INrpConfig::Save( fileName );	

	return fileName;
}

void CNrpGameBox::Load( const std::string& fileName )
{
	INrpConfig::Load(  fileName );

	for( int k=0; k < GetValue<int>( NUMBERADDON ); k++ )
	{
		std::string addonName = IniFile::Read( SECTION_ADDONS, KEY_ADDON( k ), std::string(""), fileName );

		CNrpTechnology* tech = CNrpApplication::Instance().GetBoxAddon( addonName );

		if( tech )
			AddAddon( tech );
	}
}

float CNrpGameBox::GetBoxAddonsPrice()
{
	float sum = 0;
	ADDON_LIST_ITERATOR pIter = _addons.begin();
	for( ; pIter != _addons.end(); pIter++ )
		sum += (*pIter)->GetValue<float>( PRICE );

	return sum;
}

nrp::CNrpGameBox::CNrpGameBox( CNrpGame* ptrGame ) : INrpConfig( CLASS_GAMEBOX, "" )
{
	assert( ptrGame != NULL );
	CreateValue<std::string>( NAME, ptrGame ? ptrGame->GetString( NAME ) : "" );
	CreateValue<PNrpGame>( GAME, ptrGame );
	CreateValue<int>( NUMBERADDON, 0 );
	CreateValue<int>( LEVEL, 0 );
	CreateValue<int>( BOXNUMBER, 0 );
	CreateValue<int>( PRICE, 18 );
}

}//end of namespace nrp