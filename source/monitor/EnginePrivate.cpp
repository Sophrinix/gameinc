#include "StdAfx.h"
#include "EnginePrivate.h"
#include "INrpObject.h"

//массив указателей на объекты
static NrpSystemMap GSystemObjectsMap;	

void GInsertObjectToSystemMap( nrp::INrpObject* obj )
{
	NrpSystemMap::iterator pIter = GSystemObjectsMap.find( obj->GetSystemName() );

	if( pIter != GSystemObjectsMap.end() )
	{
		char text[ MAX_PATH ];
		snprintf( text, MAX_PATH - 1, "ERROR: SystemMap duplicate object %s: name %s\n", obj->GetClassName().c_str(), obj->GetSystemName().c_str() );
		OutputDebugString( text );
	}
	else
		GSystemObjectsMap[ obj->GetSystemName() ] = obj;

}

void GRemoveObjectFromSystemMap( nrp::INrpObject* obj )
{
	NrpSystemMap::iterator pIter = GSystemObjectsMap.find( obj->GetSystemName() );

	if( pIter == GSystemObjectsMap.end() )
	{
		char text[ MAX_PATH ];
		snprintf( text, MAX_PATH - 1, "ERROR: SystemMap remove unlinked object: name %s\n", obj->GetSystemName().c_str() );
		OutputDebugString( text );
	}
	else
		GSystemObjectsMap.erase( pIter );
}


void GGetListSystemObject( std::vector< std::string >& stringArray )
{
	NrpSystemMap::iterator pIter = GSystemObjectsMap.begin();

	for( ; pIter != GSystemObjectsMap.end(); ++pIter )
		stringArray.push_back( (*pIter).second->GetClassName() + ":" + (*pIter).first );
}