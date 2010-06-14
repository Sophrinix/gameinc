/********************************************************************
	Create:	28:1:2010   14:37
	File  :	EnginePrivate.h
	Author:	Dalerank
	
	Desc  :	√лобальный массив указателей на системные объекты,
		    при наследовании от класса INrpObject, объекты автоматически
			размещаютс€ и удал€ютс€ из него.
*********************************************************************/
#pragma once
#include <map>
#include <vector>

namespace nrp
{
	class INrpObject;
}

//*! определение типа карты системное_им€<->указатель на элемент
typedef std::map< SYSTEM_NAME, nrp::INrpObject* > NrpSystemMap; 

//*! добавление указател€ на объект в массив
void GInsertObjectToSystemMap( nrp::INrpObject* obj );

//*! удаление указател€ на объект из массива
void GRemoveObjectFromSystemMap(  nrp::INrpObject* obj );

//*! вывод списка размещенных обхектов
void GGetListSystemObject( std::vector< std::string >& stringArray );