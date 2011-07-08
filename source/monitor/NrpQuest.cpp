#include "StdAfx.h"
#include "NrpQuest.h"
#include "nrpScript.h"

namespace nrp
{
CLASS_NAME CLASS_NRPQUEST( "CNrpQuest" );
	
CNrpQuest::CNrpQuest(void) : INrpConfig( CLASS_NRPQUEST, "" )
{
	RegProperty<NrpText>( CLASSOBJECT, CLASS_NRPQUEST );
	RegProperty( POSX, 0 );
	RegProperty( POSY, 0 );
	RegProperty<NrpText>( INTERNAL_NAME, "" );
	RegProperty<NrpText>( NAME, "" );
    RegProperty<bool>( OBSOLETE, false );
	RegProperty( STARTFUNCREF, -1 );
	RegProperty<NrpText>( BASEFILE, "" );
    RegProperty<bool>( ACTIVE, false );
}

CNrpQuest::~CNrpQuest(void)
{
}

void CNrpQuest::Start()
{
	if( (int)_self[ STARTFUNCREF ] < 1 )
	{
		assert( "Can't find start function for quest" );
		Log( HW ) << "Can't find start function for quest name:" << (NrpText)_self[ NAME ] << "  file:" << (NrpText)_self[ BASEFILE ] << term;
		return;
	}

	CNrpScript::Instance().CallFunction( (int)_self[ STARTFUNCREF ], this );
}

}