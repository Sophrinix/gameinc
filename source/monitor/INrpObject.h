/********************************************************************
	created:	2010/01/28
	created:	28:1:2010   10:09
	file base:	INrpObject
	file ext:	h
	author:		Dalerank
	
	purpose:	������� ����� �������� ������� � �������
*********************************************************************/
#pragma once

#include <string>
#include "StrConversation.h"
#include "EnginePrivate.h"

namespace nrp
{

typedef const std::string CLASS_NAME; 

class INrpObject
{
private:
	SYSTEM_NAME systemName_, objectName_;

	INrpObject(void)
	{

	}
public:
	//���������� ��� ������� � �������
	SYSTEM_NAME SystemName() { return systemName_; }
	const std::string& ObjectTypeName() { return objectName_; }

	//���������� ������� � �������
	INrpObject( CLASS_NAME objectName, SYSTEM_NAME sysName ):
			objectName_( objectName ), systemName_( sysName )
	{
		if( systemName_.empty() )
			systemName_ = objectName + "_object" + conv::ToStr( rand() );

		GInsertObjectToSystemMap( this );
	}

	//�������� ������� �� �������	
	virtual ~INrpObject(void)
	{
		GRemoveObjectFromSystemMap( this );
	}
};

}//namespace nrp