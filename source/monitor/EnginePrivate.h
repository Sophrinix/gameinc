/********************************************************************
	Create:	28:1:2010   14:37
	File  :	EnginePrivate.h
	Author:	Dalerank
	
	Desc  :	√лобальный массив указателей на системные объекты,
		    при наследовании от класса INrpObject, объекты автоматически
			размещаютс€ и удал€ютс€ из него.
*********************************************************************/
#pragma once

namespace nrp
{
	class INrpObject;
}

//*! добавление указател€ на объект в массив
void GInsertObjectToSystemMap( nrp::INrpObject* obj );

//*! удаление указател€ на объект из массива
void GRemoveObjectFromSystemMap(  nrp::INrpObject* obj );
