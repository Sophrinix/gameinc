/********************************************************************
	Create:	28:1:2010   14:37
	File  :	EnginePrivate.h
	Author:	Dalerank
	
	Desc  :	���������� ������ ���������� �� ��������� �������,
		    ��� ������������ �� ������ INrpObject, ������� �������������
			����������� � ��������� �� ����.
*********************************************************************/
#pragma once
#include <map>
#include <vector>

namespace nrp
{
	class INrpObject;
}

//*! ����������� ���� ����� ���������_���<->��������� �� �������
typedef std::map< SYSTEM_NAME, nrp::INrpObject* > NrpSystemMap; 

//*! ���������� ��������� �� ������ � ������
void GInsertObjectToSystemMap( nrp::INrpObject* obj );

//*! �������� ��������� �� ������ �� �������
void GRemoveObjectFromSystemMap(  nrp::INrpObject* obj );

//*! ����� ������ ����������� ��������
void GGetListSystemObject( std::vector< std::string >& stringArray );