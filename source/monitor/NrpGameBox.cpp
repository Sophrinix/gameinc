#include "StdAfx.h"
#include "IniFile.h"
#include "NrpGameBox.h"
#include "NrpGame.h"
#include "NrpTechnology.h"
#include "NrpApplication.h"

#include <io.h>

namespace nrp
{
const NrpText CNrpGameBox::saveTemplate = L"box.ini";

CLASS_NAME CLASS_GAMEBOX( "CNrpGameBox" );

CNrpGameBox::~CNrpGameBox(void)
{
}

int CNrpGameBox::_FindAddon( const NrpText& name )
{
	for( u32 i=0; i < _addons.size(); i++ )
		if( (*_addons[ i ])[NAME] == name || (*_addons[ i ])[INTERNAL_NAME] == name )
			return i;

	return -1;
}

bool CNrpGameBox::IsMyAddon( const NrpText& name )
{
	return _FindAddon( name ) != -1;
}

void CNrpGameBox::RemoveAddon( const NrpText& name )
{
	int pos = _FindAddon( name );
	if( pos != -1 )
	{
		_addons.erase( pos );
		_self[ NUMBERADDON ] = static_cast< int >( _addons.size() );
	}
	else
		Log(HW) << "Не нашел подходящего элемента = " << name << term;
}

void CNrpGameBox::RemoveAddon( const CNrpTechnology& tech )
{
	int pos = _FindAddon( tech[ INTERNAL_NAME ] );
	if( pos != -1 )
	{
		_addons.erase( pos );
		_self[ NUMBERADDON ] = static_cast< int >( _addons.size() );
	}
	else
		Log(HW) << "Не нашел подходящего элемента = " << (NrpText)tech[ INTERNAL_NAME ] << term;
}

int CNrpGameBox::_GetAddonSumLevel()
{
	int ret = 0;
	for( u32 i=0; i < _addons.size(); i++ )
		ret += (int)(*_addons[ i ])[ LEVEL ];

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

	if( (int)(*tech)[ LEVEL ] + _GetAddonSumLevel() < (int)_self[ LEVEL ] )
	{
		_addons.push_back( tech );
		_self[ NUMBERADDON ] = static_cast< int >( _addons.size() );
		return true;
	}
	
	return false;
}

CNrpBoxAddon* CNrpGameBox::GetAddon( size_t index )
{
	return index < _addons.size() ? _addons[ index ] : NULL;
}

NrpText CNrpGameBox::Save( const NrpText& fileName )
{
	INrpConfig::Save( fileName );	

	IniFile sv( fileName );
	for( u32 k=0; k < _addons.size(); k++ )
		 sv.Set( SECTION_ADDONS, CreateKeyAddon( k ), (NrpText)(*_addons[ k ])[ INTERNAL_NAME ] );

	sv.Save();

	return fileName;
}

void CNrpGameBox::Load( const NrpText& fileName )
{
	INrpConfig::Load(  fileName );
	IniFile rv( fileName );

	for( int k=0; k < (int)_self[ NUMBERADDON ]; k++ )
	{
		NrpText addonName = rv.Get( SECTION_ADDONS, CreateKeyAddon( k ), NrpText("") );

		CNrpTechnology* tech = _nrpApp.GetBoxAddon( addonName );

		assert( tech );
		if( tech )
			AddAddon( tech );
	}
}

float CNrpGameBox::GetBoxAddonsPrice()
{
	float sum = 0;
	for( u32 i=0; i <_addons.size(); i++ )
		sum += (*_addons[ i ])[ PRICE ].As<float>();

	return sum;
}

NrpText CNrpGameBox::ClassName()
{
	return CLASS_GAMEBOX;
}

nrp::CNrpGameBox::CNrpGameBox( CNrpGame* ptrGame ) : INrpConfig( CLASS_GAMEBOX, "" )
{
	assert( ptrGame != NULL );
	RegProperty<NrpText>( NAME, ptrGame ? ptrGame->Text( NAME ) : "" );
	RegProperty<PNrpGame>( GAME, ptrGame );
	RegProperty( NUMBERADDON, 0 );
	RegProperty( LEVEL, 0 );
	RegProperty( BOXNUMBER, 0 );
	RegProperty( PRICE, 18 );
}

}//end of namespace nrp