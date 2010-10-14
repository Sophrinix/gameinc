#pragma once

#include "nrpConfig.h"

namespace nrp
{

OPTION_NAME MESSAGE( "message" );
OPTION_NAME ACTION( "action" );

const std::string CLASS_PDAITEM( "CPdaItem" );
const std::string CLASS_CNRPPDA( "CNrpPda" );

class CPdaItem : public INrpConfig
{
public:
	CPdaItem( const std::string& m, 
		const std::string& a, 
		const SYSTEMTIME& t ) : INrpConfig( CLASS_PDAITEM, "" )
	{
		assert( m.size() > 0 && a.size() > 0 );

		CreateValue<std::string>( MESSAGE, m );
		CreateValue<std::string>( ACTION, a);
		CreateValue<SYSTEMTIME>( STARTDATE, t );
	}

private:
	CPdaItem() : INrpConfig( CLASS_PDAITEM, "" ) {}
	void Load_( char* file_name ) {}
};

class CNrpPda : INrpConfig
{
public:
	CNrpPda();
	~CNrpPda();

	void Save( const std::string& fileName );
	void Load( const std::string& fileName );

	void AddItem( const std::string message, const std::string& action, const SYSTEMTIME& lTime );
	const CPdaItem& Next();
	const CPdaItem& Prev();
	const CPdaItem& Current();
	void Clear();

private:

	void Load_( char* file_name ) {}
	void _ClearOldItems();
	typedef std::vector< CPdaItem* > LIST_ITEM;
	LIST_ITEM _items;
	int _currentIndex;
};

}//end namespace nrp