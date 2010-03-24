/********************************************************************
	created:	2010/01/28
	created:	28:1:2010   10:09
	file base:	INrpObject
	file ext:	h
	author:		Dalerank
	
	purpose:	Базовый класс описания объекта в системе
*********************************************************************/
#pragma once

#include <string>
#include "StrConversation.h"
#include "EnginePrivate.h"

namespace nrp
{

class INrpObject
{
private:
	const CLASS_NAME className_;
	SYSTEM_NAME systemName_;

	INrpObject(void)
	{

	}
public:
	//имя класса
	CLASS_NAME ClassName() { return className_; }
	//уникальное имя объекта в массиве
	SYSTEM_NAME GetSystemName() { return systemName_; }

	//проверка типа класса
	bool IsClassA( CLASS_NAME name )
	{
		return name == className_;
	}

	//размещение объекта в массиве
	INrpObject( CLASS_NAME className, SYSTEM_NAME sysName ) : 
					className_(className), systemName_(sysName)
	{
		if( systemName_.empty() )
			systemName_ = className_ + "_object" + IntToStr( rand() );

		GInsertObjectToSystemMap( this );
	}

	//удаление объекта из массива	
	virtual ~INrpObject(void)
	{
		GRemoveObjectFromSystemMap( this );
	}
};

}//namespace nrp