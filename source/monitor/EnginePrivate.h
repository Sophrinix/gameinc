/********************************************************************
	Create:	28:1:2010   14:37
	File  :	EnginePrivate.h
	Author:	Dalerank
	
	Desc  :	���������� ������ ���������� �� ��������� �������,
		    ��� ������������ �� ������ INrpObject, ������� �������������
			����������� � ��������� �� ����.
*********************************************************************/
#pragma once

namespace nrp
{
	class INrpObject;
}

//*! ���������� ��������� �� ������ � ������
void GInsertObjectToSystemMap( nrp::INrpObject* obj );

//*! �������� ��������� �� ������ �� �������
void GRemoveObjectFromSystemMap(  nrp::INrpObject* obj );
