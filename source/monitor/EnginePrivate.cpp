#include "StdAfx.h"
#include "EnginePrivate.h"
#include "INrpObject.h"
#include "Logger.h"

//массив указателей на объекты
static NrpSystemMap GSystemObjectsMap;	

void GInsertObjectToSystemMap( nrp::INrpObject* obj )
{
	NrpSystemMap::iterator pIter = GSystemObjectsMap.find( obj->SystemName() );

	if( pIter != GSystemObjectsMap.end() )
	{
		nrp::Log(nrp::HW) << "ERROR: SystemMap duplicate object" << obj->ObjectTypeName() << obj->SystemName();
	}
	else
		GSystemObjectsMap[ obj->SystemName() ] = obj;

}

void GRemoveObjectFromSystemMap( nrp::INrpObject* obj )
{
	NrpSystemMap::iterator pIter = GSystemObjectsMap.find( obj->SystemName() );

	if( pIter == GSystemObjectsMap.end() )
	{
		nrp::Log(nrp::HW) << "ERROR: SystemMap remove unlinked object: name " << obj->SystemName();
	}
	else
		GSystemObjectsMap.erase( pIter );
}


void GGetListSystemObject( std::vector< std::string >& stringArray )
{
	NrpSystemMap::iterator pIter = GSystemObjectsMap.begin();

	for( ; pIter != GSystemObjectsMap.end(); ++pIter )
		stringArray.push_back( (*pIter).second->ObjectTypeName() + ":" + (*pIter).first );
}