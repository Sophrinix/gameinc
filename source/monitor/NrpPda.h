#pragma once

#include "nrpConfig.h"
#include "NrpTime.h"

namespace nrp
{
OPTION_NAME MESSAGE( "message" );
OPTION_NAME ACTION( "action" );

class CPdaItem : public INrpConfig
{
public:
    static const NrpText message;
    static const NrpText action;
    static const NrpText time;

	CPdaItem( const NrpText& m, const NrpText& a, const NrpTime& t );

	static NrpText ClassName();
private:
	CPdaItem();
};

class CNrpPda : INrpConfig
{
public:
    static const CPdaItem invalidItem;

	CNrpPda();
	~CNrpPda();

	NrpText Save( const NrpText& fileName );
	void Load( const NrpText& fileName );

	void AddItem( const NrpText& message, const NrpText& action, const NrpTime& lTime );
	const CPdaItem& Next();
	const CPdaItem& Prev();
	const CPdaItem& Current();
	void Clear();

	static NrpText ClassName();
private:

	void _ClearOldItems();
	typedef core::array< CPdaItem* > LIST_ITEM;
	LIST_ITEM _items;
	int _currentIndex;
};

}//end namespace nrp