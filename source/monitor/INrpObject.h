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

class INrpObject
{
private:
	const CLASS_NAME className_;
	SYSTEM_NAME systemName_;

	INrpObject(void)
	{

	}
public:
	//��� ������
	CLASS_NAME ClassName() { return className_; }
	//���������� ��� ������� � �������
	SYSTEM_NAME GetSystemName() { return systemName_; }

	//�������� ���� ������
	bool IsClassA( CLASS_NAME name )
	{
		return name == className_;
	}

	//���������� ������� � �������
	INrpObject( CLASS_NAME className, SYSTEM_NAME sysName ) : 
					className_(className), systemName_(sysName)
	{
		if( systemName_.empty() )
			systemName_ = className_ + "_object" + IntToStr( rand() );

		GInsertObjectToSystemMap( this );
	}

	//�������� ������� �� �������	
	virtual ~INrpObject(void)
	{
		GRemoveObjectFromSystemMap( this );
	}
};

}//namespace nrp