#include "StdAfx.h"
#include "EnginePrivate.h"
#include "INrpObject.h"
#include "Logger.h"


#include <irrMap.h>
//*! определение типа карты системное_имя<->указатель на элемент
typedef irr::core::map< SYSTEM_NAME, nrp::INrpObject* > NrpSystemMap; 

//массив указателей на объекты
static NrpSystemMap GSystemObjectsMap;	

void GInsertObjectToSystemMap( nrp::INrpObject* obj )
{
	NrpSystemMap::Node* pIter = GSystemObjectsMap.find( obj->SystemName() );

	if( !pIter )
	{
		nrp::Log(nrp::HW) << "ERROR: SystemMap duplicate object" << obj->ObjectTypeName() << obj->SystemName();
	}
	else
		GSystemObjectsMap[ obj->SystemName() ] = obj;

}

void GRemoveObjectFromSystemMap( nrp::INrpObject* obj )
{
	NrpSystemMap::Node* pIter = GSystemObjectsMap.find( obj->SystemName() );

	if( !pIter )
	{
		nrp::Log(nrp::HW) << "ERROR: SystemMap remove unlinked object: name " << obj->SystemName();
	}
	else
		GSystemObjectsMap.remove( obj->SystemName() );
}


/*void GGetListSystemObject( std::vector< stringw >& stringArray )
{
	NrpSystemMap::iterator pIter = GSystemObjectsMap.begin();

	for( ; pIter != GSystemObjectsMap.end(); ++pIter )
		stringArray.push_back( (*pIter).second->ObjectTypeName() + ":" + (*pIter).first );
}*/