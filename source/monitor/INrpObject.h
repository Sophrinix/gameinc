/********************************************************************
	created:	2010/01/28
	created:	28:1:2010   10:09
	file base:	INrpObject
	file ext:	h
	author:		Dalerank
	
	purpose:	������� ����� �������� ������� � �������
*********************************************************************/
#pragma once

#include "NrpText.h"
#include "EnginePrivate.h"

namespace nrp
{

typedef NrpText CLASS_NAME; 

class INrpObject
{
private:
	SYSTEM_NAME systemName_;
	CLASS_NAME objectName_;

	INrpObject(void)
	{

	}
public:
	//���������� ��� ������� � �������
	SYSTEM_NAME SystemName() { return systemName_; }
	CLASS_NAME& ObjectTypeName() { return objectName_; }

	//���������� ������� � �������
	INrpObject( CLASS_NAME objectName, SYSTEM_NAME sysName ):
			objectName_( objectName ), systemName_( sysName )
	{
		if( !systemName_.size() )
			systemName_ = objectName + NrpText("_object") + NrpText( rand() );

		GInsertObjectToSystemMap( this );
	}

	//�������� ������� �� �������	
	virtual ~INrpObject(void)
	{
		GRemoveObjectFromSystemMap( this );
	}
};

}//namespace nrp