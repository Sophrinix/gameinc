-- ������� ��������� ��� ������ � ������
-- �������� � ����� Logger.h 
--
-- ���������� ������ ���������

-- ��� ������� OutputDebugString
ODS 			= 1
-- � Quake-style �������
CON 			= 2
-- � ���� ������
NRPDB 			= 4
-- � ������ �������
NRPETL 			= 8
-- �����
ALL 			= ODS|CON|NRPDB|NRPETL


--
-- ����������, �� ������� ������ ���������
--
-- ������� ������ � ��������������
AUTH 			= 0
-- ������� �����
LOG 			= 1
-- ����������� ������
VIDEO 			= 2
-- ��������� ���������������
AIM 			= 3
-- �������� ������
SOUND 			= 4
-- ���������� ���������� �� ����������
HW 				= 5
-- ������� ���������� ����� � ��������
NETWORK 		= 6
-- ����������(�), ���������� � �� � �����������
DB 				= 7
-- ������, ������������ �������
CONF 			= 8
-- ���������� ������
SCRIPT			= 9


--
-- ������� �������� ���������
--
INFO 			= 0
DEBUG 			= 1
WARNING 		= 2
FATAL 			= 3

--
-- ���� ����������� ��������� (��� ������� �������)
-- ���� �� ���. �� ��������� ������� Log ��������� NO_CODE (0),
-- ��� �� ������ ����� ���������� ������� ������ �������� ����.
-- NO_CODE			= 0


-- 
-- ��������������� ������� (��� ��������?)
--
function LogScript(...)
		Log({src=SCRIPT, dev=ALL}, ...)
end
function LogVideo(...)
		Log({src=VIDEO, dev=ALL}, ...)
end

