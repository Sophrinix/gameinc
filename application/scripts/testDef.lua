local base = _G

module( "testDef" )

local table = base.table
advtech = {} 
videotech = {}
soundtech = {}
engine = {}

defs = {}

defs[ base.PT_PLUGIN ] = engine
defs[ base.PT_VIDEOTECH ] = videotech
defs[ base.PT_SOUNDTECH ] = soundtech
defs[ base.PT_ADVTECH ] = advtech

table.insert( advtech, "���������� ����������������." )
table.insert( advtech, "��� ������ ���������� ������." )
table.insert( advtech, "����������� ����������." )
table.insert( advtech, "������� ��������." )
table.insert( advtech, "���������� ��������." )
table.insert( advtech, "��� ����� ��������." )
table.insert( advtech, "���������� ��� ��� ������� �������." )
table.insert( advtech, "����� ������ ��� ������� ��������?" )
table.insert( advtech, "���������� ���� ��������." )
table.insert( advtech, "�� �������� ��� ��������� ���������." )
table.insert( advtech, "�������� �������� ����������. ��� ��������� ���������, � ��� ����� ���������." )
table.insert( advtech, "�������� ������� ����������. ��� ������������������, � ���������� ����� �� � ����." )
table.insert( advtech, "���������� ����� �������. ����� ������ � ����-���� ����������." )
table.insert( advtech, "�������� ����������. ��������� �� ���������." )

table.insert( engine, "� ����� ������ �� ��������?." )
table.insert( engine, "��... ���� �������� ������������." )
table.insert( engine, "����� ������� ������� �������������." )
table.insert( engine, "������ ���������� ��������� �����." )
table.insert( engine, "����� �������� �������." )
table.insert( engine, "��������� �������� �������." )
table.insert( engine, "����������� � ������������ ������." )
table.insert( engine, "������� �������������� �����������." )
table.insert( engine, "������ ��� ������� ��������� ������ �������." )
table.insert( engine, "����� ���� ������� ����������� ������ �������." )
table.insert( engine, "������������ � �������������� ������ �������� ���� �����." )
table.insert( engine, "�� �������� � ������� ��� ������ ������������." )
table.insert( engine, "����������� ���� ����������� �������." )
table.insert( engine, "�� ����� ���� ������ ����� � �������." )

table.insert( videotech, "�� ��������� ��� ��������? �� ������� ��� �������!" )
table.insert( videotech, "������� � ���������� ����������� ���������." )
table.insert( videotech, "��� ����������� ��������� � ���������." )
table.insert( videotech, "����� ������ �������." )
table.insert( videotech, "� �������� ����� ���-�� ������." )
table.insert( videotech, "��� �������� ����� ��� ������� ����������. ��� ����� ���� �����." )
table.insert( videotech, "������� ���� ��������. ��� ����� ���� ������� �����." )
table.insert( videotech, "���������� �������. ��� ��������� ���������, � ��� ������ �������." )
table.insert( videotech, "�������� ������� �������. ��������� ����� ��������� ����������� ������." )
table.insert( videotech, "����� ������� �������. �������� ��������� ����� ��������� ��������� �������." )
table.insert( videotech, "������������ �������. �������� �� �����, ����� ��� ����." )

table.insert( soundtech, "�� ��������� ��� ��������� ������?" )
table.insert( soundtech, "�� ��� ����� ������� ������." )
table.insert( soundtech, "����������� ����." )
table.insert( soundtech, "��� ����� ��������." )
table.insert( soundtech, "�� ����� ������� ����." )
table.insert( soundtech, "��� ��� �������� � ��������." )
table.insert( soundtech, "���� ��� ������������ �����." )
table.insert( soundtech, "���������� ��� �������." )
table.insert( soundtech, "���� ���� ��������." )
table.insert( soundtech, "�� �������� �� �������." )
table.insert( soundtech, "���������� ����. ��� ��������� �������, � ��� ����� �������� ������." )
table.insert( soundtech, "�������� ����. ���� �� ��������� �� �������� ���������� ������� ���� �����." )
table.insert( soundtech, "������� ����. ������ ������� ������." )
table.insert( soundtech, "����������� ����. ��������� ������ �� ���������." )
