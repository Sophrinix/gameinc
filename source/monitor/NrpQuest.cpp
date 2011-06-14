#include "StdAfx.h"
#include "NrpQuest.h"
#include "nrpScript.h"

namespace nrp
{
CLASS_NAME CLASS_NRPQUEST( "CNrpQuest" );
	
CNrpQuest::CNrpQuest(void) : INrpConfig( CLASS_NRPQUEST, "" )
{
	Add<NrpText>( CLASSOBJECT, CLASS_NRPQUEST );
	Add( POSX, 0 );
	Add( POSY, 0 );
	Add<NrpText>( INTERNAL_NAME, "" );
	Add<NrpText>( NAME, "" );
    Add<bool>( OBSOLETE, false );
	Add( STARTFUNCREF, -1 );
	Add<NrpText>( BASEFILE, "" );
    Add<bool>( ACTIVE, false );
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