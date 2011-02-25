#include "StdAfx.h"
#include "nrpPda.h"
#include "timeHelpers.h"
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
		assert( item->Text( MESSAGE ).size() > 0 );
		sv.Set( section, "message", item->Text( MESSAGE ) );
		sv.Set( section, "time", item->Param( STARTDATE ).As<NrpTime>() );
		sv.Set( section, "action", item->Text( ACTION ) );
	}	

	return fileName;
}

void CNrpPda::Load( const NrpText& fileName )
{
	Clear();

	NrpTime timeDef( 0. );
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

void CNrpPda::AddItem( const NrpText message, const NrpText& action, const NrpTime& lTime )
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

	return valid ? *_items[ _currentIndex ] : CPdaItem( "", "", NrpTime( 0. ) );
}

NrpText CNrpPda::ClassName()
{
	return CLASS_CNRPPDA;
}


NrpText CPdaItem::ClassName()
{
	return CLASS_PDAITEM;
}

CPdaItem::CPdaItem( const NrpText& m, const NrpText& a, const NrpTime& t ) : INrpConfig( CLASS_PDAITEM, "" )
{
	assert( m.size() > 0 && a.size() > 0 );

	Add<NrpText>( MESSAGE, m );
	Add<NrpText>( ACTION, a);
	Add( STARTDATE, t );
}

CPdaItem::CPdaItem() : INrpConfig( CLASS_PDAITEM, "" )
{

}

}//end namespace nrp