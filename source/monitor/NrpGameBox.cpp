#include "StdAfx.h"
#include "IniFile.h"
#include "NrpGameBox.h"
#include "NrpGame.h"
#include "NrpTechnology.h"
#include "NrpApplication.h"

#include <io.h>

namespace nrp
{
CLASS_NAME CLASS_GAMEBOX( "CNrpGameBox" );

CNrpGameBox::~CNrpGameBox(void)
{
}

int CNrpGameBox::_FindAddon( const NrpText& name )
{
	for( u32 i=0; i < _addons.size(); i++ )
		if( (*_addons[ i ])[NAME] == name )
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
		Param( NUMBERADDON ) = static_cast< int >( _addons.size() );
	}
	else
		Log(HW) << "Не нашел подходящего элемента = " << name << term;
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

	if( (int)(*tech)[ LEVEL ] + _GetAddonSumLevel() < (int)Param( LEVEL ) )
	{
		_addons.push_back( tech );
		Param( NUMBERADDON ) = static_cast< int >( _addons.size() );
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
	IniFile sv( fileName );
	for( u32 k=0; k < _addons.size(); k++ )
		 sv.Set( SECTION_ADDONS, CreateKeyAddon( k ), _addons[ k ]->Text( NAME ) );

	INrpConfig::Save( fileName );	

	return fileName;
}

void CNrpGameBox::Load( const NrpText& fileName )
{
	INrpConfig::Load(  fileName );
	IniFile rv( fileName );

	for( int k=0; k < (int)Param( NUMBERADDON ); k++ )
	{
		NrpText addonName = rv.Get( SECTION_ADDONS, CreateKeyAddon( k ), NrpText("") );

		CNrpTechnology* tech = _nrpApp.GetBoxAddon( addonName );

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
	Add<NrpText>( NAME, ptrGame ? ptrGame->Text( NAME ) : "" );
	Add<PNrpGame>( GAME, ptrGame );
	Add<int>( NUMBERADDON, 0 );
	Add<int>( LEVEL, 0 );
	Add<int>( BOXNUMBER, 0 );
	Add<int>( PRICE, 18 );
}

}//end of namespace nrp