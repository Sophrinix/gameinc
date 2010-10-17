#include "StdAfx.h"
#include "nrpPda.h"

namespace nrp
{

CNrpPda::CNrpPda() : INrpConfig( CLASS_CNRPPDA, "" )
{

}


CNrpPda::~CNrpPda()
{
	Clear();
}

void CNrpPda::Clear()
{
	LIST_ITEM::iterator pItem = _items.begin();
	for( ; pItem != _items.end(); pItem++ )
		delete *pItem;

	_items.clear();
}

void CNrpPda::Save( const std::string& fileName )
{
	LIST_ITEM::iterator pItem = _items.begin();
	for( int k=0; pItem != _items.end(); pItem++, k++ )
	{
		std::string section = "item_"+IntToStr( k );
		assert( (*pItem)->GetValue<std::string>( MESSAGE ).size() > 0 );
		IniFile::Write( section, "message", (*pItem)->GetValue<std::string>( MESSAGE ), fileName );
		IniFile::Write( section, "time", (*pItem)->GetValue<SYSTEMTIME>( STARTDATE ), fileName );
		IniFile::Write( section, "action", (*pItem)->GetValue<std::string>( ACTION ), fileName );
	}	
}

void CNrpPda::Load( const std::string& fileName )
{
	Clear();

	SYSTEMTIME timeDef;
	memset( &timeDef, 0, sizeof( SYSTEMTIME ) );

	for( int k=0; k < MAXDWORD; k++ )
	{
		std::string section = "item_"+IntToStr( k );
		std::string mess = IniFile::Read( section, "message", std::string(""), fileName );
		//читаем до первого пустого сообщения))
		if( mess.size() > 0 )
		{
			AddItem( mess, 
					IniFile::Read( section, "action", std::string(""), fileName ),
					IniFile::Read( section, "time", timeDef, fileName ) );
		}
		else 
			break;
	}
}

void CNrpPda::AddItem( const std::string message, const std::string& action, const SYSTEMTIME& lTime )
{
	_items.push_back( new CPdaItem( message, action, lTime ) );
	
	_currentIndex = _items.size() - 1;
}

const CPdaItem& CNrpPda::Next()
{	
	if( _currentIndex + 1 < static_cast< int >( _items.size() ) )
		_currentIndex++;

	return *_items[ _currentIndex ];
}

const CPdaItem& CNrpPda::Prev()
{
	if( _items.size() > 0 && _currentIndex - 1 >= 0 )
		_currentIndex--;

	return *_items[ _currentIndex ];
}

const CPdaItem& CNrpPda::Current()
{
	bool valid =  _currentIndex >= 0 && _currentIndex < static_cast< int >( _items.size() );
	assert( valid );

	return valid ? *_items[ _currentIndex ] : CPdaItem( "", "", SYSTEMTIME() );
}
}//end namespace nrp