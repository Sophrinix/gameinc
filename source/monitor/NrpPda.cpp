#include "StdAfx.h"
#include "nrpPda.h"
#include "IniFile.h"

namespace nrp
{
CLASS_NAME CLASS_PDAITEM( "CPdaItem" );
CLASS_NAME CLASS_CNRPPDA( "CNrpPda" );

CNrpPda::CNrpPda() : INrpConfig( CLASS_CNRPPDA, "" )
{

}


CNrpPda::~CNrpPda()
{
	Clear();
}

void CNrpPda::Clear()
{
	for( u32 i=0; i < _items.size(); i++ )
		delete _items[ i ];

	_items.clear();
}

NrpText CNrpPda::Save( const NrpText& fileName )
{
	IniFile sv( fileName );
	for( u32 k=0; k < _items.size(); k++ )
	{
		CPdaItem* item = _items[ k ];
		NrpText section = CreateKeyItem( k );
		assert( item->GetValue<NrpText>( MESSAGE ).size() > 0 );
		sv.Set( section, "message", item->GetValue<NrpText>( MESSAGE ) );
		sv.Set( section, "time", item->GetValue<SYSTEMTIME>( STARTDATE ) );
		sv.Set( section, "action", item->GetValue<NrpText>( ACTION ) );
	}	

	return fileName;
}

void CNrpPda::Load( const NrpText& fileName )
{
	Clear();

	SYSTEMTIME timeDef;
	memset( &timeDef, 0, sizeof( SYSTEMTIME ) );
	IniFile rv( fileName );

	for( int k=0; k < MAXDWORD; k++ )
	{
		NrpText section = CreateKeyItem( k );
		NrpText mess = rv.Get( section, "message", NrpText("") );
		//читаем до первого пустого сообщения))
		if( mess.size() > 0 )
		{
			AddItem( mess, 
					rv.Get( section, "action", NrpText("") ),
					rv.Get( section, "time", timeDef ) );
		}
		else 
			break;
	}
}

void CNrpPda::AddItem( const NrpText message, const NrpText& action, const SYSTEMTIME& lTime )
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

NrpText CNrpPda::ClassName()
{
	return CLASS_CNRPPDA;
}


NrpText CPdaItem::ClassName()
{
	return CLASS_PDAITEM;
}

CPdaItem::CPdaItem( const NrpText& m, const NrpText& a, const SYSTEMTIME& t ) : INrpConfig( CLASS_PDAITEM, "" )
{
	assert( m.size() > 0 && a.size() > 0 );

	Push<NrpText>( MESSAGE, m );
	Push<NrpText>( ACTION, a);
	Push<SYSTEMTIME>( STARTDATE, t );
}

CPdaItem::CPdaItem() : INrpConfig( CLASS_PDAITEM, "" )
{

}

}//end namespace nrp