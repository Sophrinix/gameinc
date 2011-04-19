local base = _G

module( "testDef" )

local table = base.table
advtech = {} 
videotech = {}
soundtech = {}
engine = {}

function GetText( type, quality )
	if quality == nil then 
		return "������ �����������"
	end
	
	local prc = quality / 100;
	local text = nil
	if type == base.PT_GAMEENGINE then
		text = engine[ prc * #engine ]	
	elseif type == base.PT_VIDEOTECH then
		text = videotech[ prc * #videotech]
	elseif type == base.PT_SOUNDTECH then
		text = soundtech[ prc * #soundtech]
	elseif type == base.PT_ADVTECH then
		text = advtech[ prc * #advtech]
	end
	
	if text == nil then
		return "������ �����������"
	else
		return text
	end
end

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