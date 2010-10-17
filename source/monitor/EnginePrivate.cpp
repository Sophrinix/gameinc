#include "StdAfx.h"
#include "EnginePrivate.h"
#include "INrpObject.h"

//массив указателей на объекты
static NrpSystemMap GSystemObjectsMap;	

void GInsertObjectToSystemMap( nrp::INrpObject* obj )
{
	NrpSystemMap::iterator pIter = GSystemObjectsMap.find( obj->SystemName() );

	if( pIter != GSystemObjectsMap.end() )
	{
		char text[ MAX_PATH ];
		snprintf( text, MAX_PATH - 1, "ERROR: SystemMap duplicate object %s: name %s\n", obj->ObjectName().c_str(), obj->SystemName().c_str() );
		OutputDebugString( text );
	}
	else
		GSystemObjectsMap[ obj->SystemName() ] = obj;

}

void GRemoveObjectFromSystemMap( nrp::INrpObject* obj )
{
	NrpSystemMap::iterator pIter = GSystemObjectsMap.find( obj->SystemName() );

	if( pIter == GSystemObjectsMap.end() )
	{
		char text[ MAX_PATH ];
		snprintf( text, MAX_PATH - 1, "ERROR: SystemMap remove unlinked object: name %s\n", obj->SystemName().c_str() );
		OutputDebugString( text );
	}
	else
		GSystemObjectsMap.erase( pIter );
}


void GGetListSystemObject( std::vector< std::string >& stringArray )
{
	NrpSystemMap::iterator pIter = GSystemObjectsMap.begin();

	for( ; pIter != GSystemObjectsMap.end(); ++pIter )
		stringArray.push_back( (*pIter).second->ObjectName() + ":" + (*pIter).first );
}